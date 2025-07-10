#include "Graphics.h"
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

// �V�F�[�_�[�ɓn���f�[�^�\����
// ���_�̍��W���I�t�Z�b�g���邽�߂Ɏg�p
struct ConstantBuffer
{
    float x_offset;
    float y_offset;
    float padding1;
    float padding2;
};

// ���_�f�[�^�\����
struct SimpleVertex {
    float Pos[3];
    float Color[4];
};


// �R���X�g���N�^
Graphics::Graphics() :
    m_pSwapChain(nullptr),
    m_pd3dDevice(nullptr),
    m_pImmediateContext(nullptr),
    m_pRenderTargetView(nullptr),
    m_pVertexShader(nullptr),
    m_pPixelShader(nullptr),
    m_pVertexLayout(nullptr),
    m_pVertexBuffer(nullptr),
    m_pConstantBuffer(nullptr),
    m_pBulletVertexBuffer(nullptr), // �e�̒��_�o�b�t�@��������
    m_triangleX(0.0f),
    m_triangleY(0.0f),
    m_fireCooldown(0.0f) // ���˃N�[���_�E����������
{
}

// �f�X�g���N�^
Graphics::~Graphics() {
    Shutdown();
}

// ����������
HRESULT Graphics::Initialize(HWND hWnd) {
    HRESULT hr = S_OK;

    // ... (�X���b�v�`�F�[���A�f�o�C�X�A�����_�[�^�[�Q�b�g�r���[�A�r���[�|�[�g�̍쐬�͕ύX�Ȃ�) ...
    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferCount = 1;
    sd.BufferDesc.Width = 800;
    sd.BufferDesc.Height = 600;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;

    D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };
    hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
        featureLevels, 1, D3D11_SDK_VERSION, &sd,
        &m_pSwapChain, &m_pd3dDevice, nullptr, &m_pImmediateContext);
    if (FAILED(hr)) return hr;

    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
    if (FAILED(hr)) return hr;

    hr = m_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRenderTargetView);
    pBackBuffer->Release();
    if (FAILED(hr)) return hr;

    m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, nullptr);

    D3D11_VIEWPORT vp;
    vp.Width = 800.0f;
    vp.Height = 600.0f;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    m_pImmediateContext->RSSetViewports(1, &vp);

    // ... (�V�F�[�_�[�A���̓��C�A�E�g�̍쐬�͕ύX�Ȃ�) ...
    ID3DBlob* pVSBlob = nullptr;
    const char* vsShaderCode =
        "cbuffer ConstantBuffer : register(b0) {\n"
        "    float2 offset;\n"
        "};\n"
        "struct VS_INPUT { float4 Pos : POSITION; float4 Color : COLOR; };\n"
        "struct PS_INPUT { float4 Pos : SV_POSITION; float4 Color : COLOR; };\n"
        "PS_INPUT VS(VS_INPUT input) {\n"
        "    PS_INPUT output = (PS_INPUT)0;\n"
        "    output.Pos = input.Pos;\n"
        "    output.Pos.xy += offset; \n"
        "    output.Color = input.Color;\n"
        "    return output;\n"
        "}\n";
    D3DCompile(vsShaderCode, strlen(vsShaderCode), nullptr, nullptr, nullptr, "VS", "vs_4_0", 0, 0, &pVSBlob, nullptr);
    m_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &m_pVertexShader);

    ID3DBlob* pPSBlob = nullptr;
    const char* psShaderCode =
        "struct PS_INPUT { float4 Pos : SV_POSITION; float4 Color : COLOR; };\n"
        "float4 PS(PS_INPUT input) : SV_Target {\n"
        "    return input.Color;\n"
        "}\n";
    D3DCompile(psShaderCode, strlen(psShaderCode), nullptr, nullptr, nullptr, "PS", "ps_4_0", 0, 0, &pPSBlob, nullptr);
    m_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &m_pPixelShader);

    D3D11_INPUT_ELEMENT_DESC layout[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    m_pd3dDevice->CreateInputLayout(layout, 2, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &m_pVertexLayout);
    pVSBlob->Release();
    pPSBlob->Release();

    // �v���C���[�i�O�p�`�j�̒��_�o�b�t�@�쐬
    {
        SimpleVertex vertices[] = {
            { { 0.0f, 0.05f, 0.5f }, { 1.0f, 0.0f, 0.0f, 1.0f } }, // ��
            { { 0.05f, -0.05f, 0.5f }, { 0.0f, 1.0f, 0.0f, 1.0f } }, // �E��
            { { -0.05f, -0.05f, 0.5f }, { 0.0f, 0.0f, 1.0f, 1.0f } }  // ����
        };
        D3D11_BUFFER_DESC bd = {};
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(SimpleVertex) * 3;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        D3D11_SUBRESOURCE_DATA InitData = {};
        InitData.pSysMem = vertices;
        hr = m_pd3dDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer);
        if (FAILED(hr)) return hr;
    }

    // �������������������������������������������� �ǉ� ��������������������������������������������
    // �e�i�l�p�`�j�̒��_�o�b�t�@�쐬
    {
        // �l�p�`��2�̎O�p�`�ō\������邽�߁A6�̒��_���K�v
        SimpleVertex vertices[] = {
            { { -0.01f, -0.02f, 0.5f }, { 1.0f, 1.0f, 0.0f, 1.0f } }, // ����
            { { -0.01f,  0.02f, 0.5f }, { 1.0f, 1.0f, 0.0f, 1.0f } }, // ����
            { {  0.01f, -0.02f, 0.5f }, { 1.0f, 1.0f, 0.0f, 1.0f } }, // �E��
            { {  0.01f,  0.02f, 0.5f }, { 1.0f, 1.0f, 0.0f, 1.0f } }, // �E��
        };
        D3D11_BUFFER_DESC bd = {};
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(SimpleVertex) * 4; // ���_4��
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        D3D11_SUBRESOURCE_DATA InitData = {};
        InitData.pSysMem = vertices;
        hr = m_pd3dDevice->CreateBuffer(&bd, &InitData, &m_pBulletVertexBuffer);
        if (FAILED(hr)) return hr;
    }
    // �������������������������������������������� �ǉ� ��������������������������������������������

    // �R���X�^���g�o�b�t�@�̍쐬 (�ύX�Ȃ�)
    D3D11_BUFFER_DESC cbd = {};
    cbd.Usage = D3D11_USAGE_DYNAMIC;
    cbd.ByteWidth = sizeof(ConstantBuffer);
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    hr = m_pd3dDevice->CreateBuffer(&cbd, nullptr, &m_pConstantBuffer);
    if (FAILED(hr)) return hr;

    return S_OK;
}

// �N���[���A�b�v����
void Graphics::Shutdown() {
    if (m_pImmediateContext) m_pImmediateContext->ClearState();

    // �������������������������������������������� �ǉ� ��������������������������������������������
    if (m_pBulletVertexBuffer) m_pBulletVertexBuffer->Release();
    // �������������������������������������������� �ǉ� ��������������������������������������������

    if (m_pConstantBuffer) m_pConstantBuffer->Release();
    if (m_pVertexBuffer) m_pVertexBuffer->Release();
    if (m_pVertexLayout) m_pVertexLayout->Release();
    if (m_pVertexShader) m_pVertexShader->Release();
    if (m_pPixelShader) m_pPixelShader->Release();
    if (m_pRenderTargetView) m_pRenderTargetView->Release();
    if (m_pSwapChain) m_pSwapChain->Release();
    if (m_pImmediateContext) m_pImmediateContext->Release();
    if (m_pd3dDevice) m_pd3dDevice->Release();

    // �|�C���^��nullptr�Ƀ��Z�b�g
    m_pSwapChain = nullptr;
    m_pd3dDevice = nullptr;
    m_pImmediateContext = nullptr;
    m_pRenderTargetView = nullptr;
    m_pVertexShader = nullptr;
    m_pPixelShader = nullptr;
    m_pVertexLayout = nullptr;
    m_pVertexBuffer = nullptr;
    m_pConstantBuffer = nullptr;
    m_pBulletVertexBuffer = nullptr; // ������ �ǉ� ������
}

// �t���[���S�̂̍X�V����
void Graphics::Update()
{
    // �v���C���[�̈ړ�����
    float moveSpeed = 0.02f;
    if (GetAsyncKeyState('W') & 0x8000) m_triangleY += moveSpeed;
    if (GetAsyncKeyState('S') & 0x8000) m_triangleY -= moveSpeed;
    if (GetAsyncKeyState('A') & 0x8000) m_triangleX -= moveSpeed;
    if (GetAsyncKeyState('D') & 0x8000) m_triangleX += moveSpeed;

    // �v���C���[����ʊO�ɏo�Ȃ��悤�ɐ���
    if (m_triangleX > 0.95f) m_triangleX = 0.95f;
    if (m_triangleX < -0.95f) m_triangleX = -0.95f;
    if (m_triangleY > 0.95f) m_triangleY = 0.95f;
    if (m_triangleY < -0.95f) m_triangleY = -0.95f;

    // �������������������������������������������� �ǉ� ��������������������������������������������
    // �e�̍X�V
    UpdateBullets();
    // �������������������������������������������� �ǉ� ��������������������������������������������
}

// �������������������������������������������� �ǉ� ��������������������������������������������
// �e�̍X�V�����i���˂ƈړ��j
void Graphics::UpdateBullets()
{
    // �e�̔��˃N�[���_�E���^�C�}�[�����炷
    m_fireCooldown -= 0.1f;

    // �N�[���_�E�����I��������V�����e�𔭎˂���
    if (m_fireCooldown <= 0.0f)
    {
        // ���̔��˂܂ł̎��Ԃ��Đݒ�
        m_fireCooldown = 1.0f;

        // ��A�N�e�B�u�Ȓe��T���Ĕ��˂���
        for (int i = 0; i < MAX_BULLETS; ++i)
        {
            if (!m_bullets[i].IsActive())
            {
                // �O�p�`�̐�[���甭�˂���
                m_bullets[i].Activate(m_triangleX, m_triangleY + 0.05f);
                break; // 1�t���[����1����������
            }
        }
    }

    // ���ׂĂ̒e�̈ʒu���X�V����
    for (int i = 0; i < MAX_BULLETS; ++i)
    {
        m_bullets[i].Update();
    }
}
// �������������������������������������������� �ǉ� ��������������������������������������������

// �t���[���S�̂̃����_�����O����
void Graphics::RenderFrame() {
    // �w�i�F�ŉ�ʂ��N���A
    float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
    m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, ClearColor);

    // �`��ݒ� (�V�F�[�_�[�Ɠ��̓��C�A�E�g)
    m_pImmediateContext->IASetInputLayout(m_pVertexLayout);
    m_pImmediateContext->VSSetShader(m_pVertexShader, nullptr, 0);
    m_pImmediateContext->PSSetShader(m_pPixelShader, nullptr, 0);
    m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);

    // ----- �v���C���[�i�O�p�`�j�̕`�� -----
    {
        // �R���X�^���g�o�b�t�@�Ƀv���C���[�̍��W����������
        D3D11_MAPPED_SUBRESOURCE mappedResource;
        m_pImmediateContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
        ConstantBuffer* cb = (ConstantBuffer*)mappedResource.pData;
        cb->x_offset = m_triangleX;
        cb->y_offset = m_triangleY;
        m_pImmediateContext->Unmap(m_pConstantBuffer, 0);

        // �v���C���[�̒��_�o�b�t�@���Z�b�g
        UINT stride = sizeof(SimpleVertex);
        UINT offset = 0;
        m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
        m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        // �v���C���[��`��
        m_pImmediateContext->Draw(3, 0);
    }

    // �������������������������������������������� �ǉ� ��������������������������������������������
    // ----- �e�̕`�� -----
    RenderBullets();
    // �������������������������������������������� �ǉ� ��������������������������������������������

    // ��ʂɕ\��
    m_pSwapChain->Present(1, 0);
}

// �������������������������������������������� �ǉ� ��������������������������������������������
// �e��`�悷��
void Graphics::RenderBullets()
{
    // �e�̒��_�o�b�t�@���Z�b�g
    UINT stride = sizeof(SimpleVertex);
    UINT offset = 0;
    m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pBulletVertexBuffer, &stride, &offset);
    // �e�͎l�p�`�Ȃ̂ŁA�g�|���W�[���g���C�A���O���X�g���b�v�ɕύX
    m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);


    // ���ׂĂ̒e��`��
    for (int i = 0; i < MAX_BULLETS; ++i)
    {
        // �A�N�e�B�u�Ȓe������`��
        if (m_bullets[i].IsActive())
        {
            // �R���X�^���g�o�b�t�@�ɒe�̍��W����������
            D3D11_MAPPED_SUBRESOURCE mappedResource;
            m_pImmediateContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
            ConstantBuffer* cb = (ConstantBuffer*)mappedResource.pData;
            cb->x_offset = m_bullets[i].GetX();
            cb->y_offset = m_bullets[i].GetY();
            m_pImmediateContext->Unmap(m_pConstantBuffer, 0);

            // �e��`��i�l�p�`�Ȃ̂Œ��_4�j
            m_pImmediateContext->Draw(4, 0);
        }
    }
}
// �������������������������������������������� �ǉ� ��������������������������������������������