#include "Graphics.h"
#include <d3dcompiler.h> // �V�F�[�_�[�R���p�C���p
#include "GameConfig.h"  // �Q�[���ݒ�t�@�C�����C���N���[�h

// �K�v�ȃ��C�u�����������N
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

/**
 * @struct ConstantBuffer
 * @brief �V�F�[�_�[�ɓn���萔�o�b�t�@�̃f�[�^�\����
 * �I�u�W�F�N�g�̃I�t�Z�b�g�i�ړ��ʁj���i�[���܂��B
 */
struct ConstantBuffer
{
    float x_offset; // X�����̃I�t�Z�b�g
    float y_offset; // Y�����̃I�t�Z�b�g
    float padding1; // 16�o�C�g�A���C�����g�̂��߂̃p�f�B���O
    float padding2; // 16�o�C�g�A���C�����g�̂��߂̃p�f�B���O
};

/**
 * @struct SimpleVertex
 * @brief ���_�f�[�^���i�[����\����
 * 3D��ԏ�̈ʒu(Pos)�ƐF(Color)�������܂��B
 */
struct SimpleVertex {
    float Pos[3];   // ���W (x, y, z)
    float Color[4]; // �F (r, g, b, a)
};

/**
 * @brief Graphics�N���X�̃R���X�g���N�^
 * ���ׂẴ����o�ϐ��� nullptr �ŏ��������܂��B
 */
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
    m_pBulletVertexBuffer(nullptr),
    m_pObstacleVertexBuffer(nullptr)
{
}

/**
 * @brief Graphics�N���X�̃f�X�g���N�^
 * Shutdown() ���Ăяo���ă��\�[�X��������܂��B
 */
Graphics::~Graphics() {
    Shutdown();
}

/**
 * @brief DirectX�̏��������s���܂��B
 * �f�o�C�X�A�X���b�v�`�F�[���A�����_�[�^�[�Q�b�g�A�V�F�[�_�[�A���_�o�b�t�@�Ȃǂ𐶐����܂��B
 * @param hWnd �`��Ώۂ̃E�B���h�E�n���h��
 * @return HRESULT �����̐���
 */
HRESULT Graphics::Initialize(HWND hWnd) {
    HRESULT hr = S_OK;

    // --- �X���b�v�`�F�[���̃f�X�N���v�^�i�ݒ���j ---
    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferCount = 1;
    sd.BufferDesc.Width = 800; // �E�B���h�E��
    sd.BufferDesc.Height = 600; // �E�B���h�E����
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // �F�`��
    sd.BufferDesc.RefreshRate.Numerator = 60; // ���t���b�V�����[�g
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE; // �E�B���h�E���[�h

    // --- �f�o�C�X�ƃX���b�v�`�F�[���̍쐬 ---
    D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };
    hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
        featureLevels, 1, D3D11_SDK_VERSION, &sd,
        &m_pSwapChain, &m_pd3dDevice, nullptr, &m_pImmediateContext);
    if (FAILED(hr)) return hr;

    // --- �����_�[�^�[�Q�b�g�r���[�̍쐬 ---
    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
    if (FAILED(hr)) return hr;
    hr = m_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRenderTargetView);
    pBackBuffer->Release();
    if (FAILED(hr)) return hr;

    // --- �����_�[�^�[�Q�b�g�̐ݒ� ---
    m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, nullptr);

    // --- �r���[�|�[�g�̐ݒ� ---
    D3D11_VIEWPORT vp;
    vp.Width = 800.0f;
    vp.Height = 600.0f;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    m_pImmediateContext->RSSetViewports(1, &vp);

    // --- ���_�V�F�[�_�[�̍쐬 ---
    ID3DBlob* pVSBlob = nullptr;
    const char* vsShaderCode = // HLSL�V�F�[�_�[�R�[�h
        "cbuffer ConstantBuffer : register(b0) {\n"
        "    float2 offset;\n"
        "};\n"
        "struct VS_INPUT { float4 Pos : POSITION; float4 Color : COLOR; };\n"
        "struct PS_INPUT { float4 Pos : SV_POSITION; float4 Color : COLOR; };\n"
        "PS_INPUT VS(VS_INPUT input) {\n"
        "    PS_INPUT output = (PS_INPUT)0;\n"
        "    output.Pos = input.Pos;\n"
        "    output.Pos.xy += offset; \n" // ���_�ʒu���I�t�Z�b�g�ł��炷
        "    output.Color = input.Color;\n"
        "    return output;\n"
        "}\n";
    D3DCompile(vsShaderCode, strlen(vsShaderCode), nullptr, nullptr, nullptr, "VS", "vs_4_0", 0, 0, &pVSBlob, nullptr);
    m_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &m_pVertexShader);

    // --- �s�N�Z���V�F�[�_�[�̍쐬 ---
    ID3DBlob* pPSBlob = nullptr;
    const char* psShaderCode = // HLSL�V�F�[�_�[�R�[�h
        "struct PS_INPUT { float4 Pos : SV_POSITION; float4 Color : COLOR; };\n"
        "float4 PS(PS_INPUT input) : SV_Target {\n"
        "    return input.Color;\n" // ���_����n���ꂽ�F�����̂܂܏o��
        "}\n";
    D3DCompile(psShaderCode, strlen(psShaderCode), nullptr, nullptr, nullptr, "PS", "ps_4_0", 0, 0, &pPSBlob, nullptr);
    m_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &m_pPixelShader);

    // --- ���̓��C�A�E�g�̍쐬 (���_�f�[�^�̍\����D3D�ɋ�����) ---
    D3D11_INPUT_ELEMENT_DESC layout[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    m_pd3dDevice->CreateInputLayout(layout, 2, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &m_pVertexLayout);
    pVSBlob->Release(); // �V�F�[�_�[BLOB�͕s�v�ɂȂ����̂ŉ��
    pPSBlob->Release();

    // --- �v���C���[�̒��_�o�b�t�@�쐬 ---
    {
        // �v���C���[�̌`��i�O�p�`�j���`
        SimpleVertex vertices[] = {
            { { 0.0f, 0.05f, 0.5f }, { 1.0f, 0.0f, 0.0f, 1.0f } },   // ��̒��_ (��)
            { { 0.05f, -0.05f, 0.5f }, { 0.0f, 1.0f, 0.0f, 1.0f } },  // �E���̒��_ (��)
            { { -0.05f, -0.05f, 0.5f }, { 0.0f, 0.0f, 1.0f, 1.0f } } // �����̒��_ (��)
        };
        D3D11_BUFFER_DESC bd = {};
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(SimpleVertex) * 3;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        D3D11_SUBRESOURCE_DATA InitData = { vertices };
        hr = m_pd3dDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer);
        if (FAILED(hr)) return hr;
    }

    // --- �e�̒��_�o�b�t�@�쐬 ---
    {
        // �e�̌`��i�l�p�`�j���` (GameConfig.h�̒萔���g�p)
        SimpleVertex vertices[] = {
            { { -BULLET_HALF_WIDTH, -BULLET_HALF_HEIGHT, 0.5f }, { 1.0f, 1.0f, 0.0f, 1.0f } }, // ���� (��)
            { { -BULLET_HALF_WIDTH,  BULLET_HALF_HEIGHT, 0.5f }, { 1.0f, 1.0f, 0.0f, 1.0f } }, // ���� (��)
            { {  BULLET_HALF_WIDTH, -BULLET_HALF_HEIGHT, 0.5f }, { 1.0f, 1.0f, 0.0f, 1.0f } }, // �E�� (��)
            { {  BULLET_HALF_WIDTH,  BULLET_HALF_HEIGHT, 0.5f }, { 1.0f, 1.0f, 0.0f, 1.0f } }, // �E�� (��)
        };
        D3D11_BUFFER_DESC bd = {};
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(SimpleVertex) * 4;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        D3D11_SUBRESOURCE_DATA InitData = { vertices };
        hr = m_pd3dDevice->CreateBuffer(&bd, &InitData, &m_pBulletVertexBuffer);
        if (FAILED(hr)) return hr;
    }

    // --- ��Q���̒��_�o�b�t�@�쐬 ---
    {
        // ��Q���̌`��i�l�p�`�j���`
        SimpleVertex vertices[] = {
            { { -0.05f, -0.05f, 0.5f }, { 0.5f, 0.5f, 0.5f, 1.0f } }, // ���� (�D�F)
            { { -0.05f,  0.05f, 0.5f }, { 0.5f, 0.5f, 0.5f, 1.0f } }, // ���� (�D�F)
            { {  0.05f, -0.05f, 0.5f }, { 0.5f, 0.5f, 0.5f, 1.0f } }, // �E�� (�D�F)
            { {  0.05f,  0.05f, 0.5f }, { 0.5f, 0.5f, 0.5f, 1.0f } }  // �E�� (�D�F)
        };
        D3D11_BUFFER_DESC bd = {};
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(SimpleVertex) * 4;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        D3D11_SUBRESOURCE_DATA InitData = { vertices };
        hr = m_pd3dDevice->CreateBuffer(&bd, &InitData, &m_pObstacleVertexBuffer);
        if (FAILED(hr)) return hr;
    }

    // --- �萔�o�b�t�@�쐬 ---
    {
        D3D11_BUFFER_DESC cbd = {};
        cbd.Usage = D3D11_USAGE_DYNAMIC; // CPU���疈�t���[���X�V���邽�� DYNAMIC �ɐݒ�
        cbd.ByteWidth = sizeof(ConstantBuffer);
        cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // CPU����̏������݃A�N�Z�X������
        hr = m_pd3dDevice->CreateBuffer(&cbd, nullptr, &m_pConstantBuffer);
        if (FAILED(hr)) return hr;
    }

    return S_OK; // ����������
}

/**
 * @brief DirectX���\�[�X���N���[���A�b�v�i����j���܂��B
 * Initialize�ō쐬�����̂Ƌt�̏����ŉ�����Ă����܂��B
 */
void Graphics::Shutdown() {
    if (m_pImmediateContext) m_pImmediateContext->ClearState();
    if (m_pObstacleVertexBuffer) { m_pObstacleVertexBuffer->Release(); m_pObstacleVertexBuffer = nullptr; }
    if (m_pBulletVertexBuffer) { m_pBulletVertexBuffer->Release(); m_pBulletVertexBuffer = nullptr; }
    if (m_pConstantBuffer) { m_pConstantBuffer->Release(); m_pConstantBuffer = nullptr; }
    if (m_pVertexBuffer) { m_pVertexBuffer->Release(); m_pVertexBuffer = nullptr; }
    if (m_pVertexLayout) { m_pVertexLayout->Release(); m_pVertexLayout = nullptr; }
    if (m_pVertexShader) { m_pVertexShader->Release(); m_pVertexShader = nullptr; }
    if (m_pPixelShader) { m_pPixelShader->Release(); m_pPixelShader = nullptr; }
    if (m_pRenderTargetView) { m_pRenderTargetView->Release(); m_pRenderTargetView = nullptr; }
    if (m_pSwapChain) { m_pSwapChain->Release(); m_pSwapChain = nullptr; }
    if (m_pImmediateContext) { m_pImmediateContext->Release(); m_pImmediateContext = nullptr; }
    if (m_pd3dDevice) { m_pd3dDevice->Release(); m_pd3dDevice = nullptr; }
}

/**
 * @brief 1�t���[�����̕`�揈�����s���܂��B
 * @param game �`�悷��I�u�W�F�N�g�̏�������Game�I�u�W�F�N�g
 */
void Graphics::RenderFrame(const Game& game) {
    // �w�i�F�ŉ�ʂ��N���A
    float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // �Â��F
    m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, ClearColor);

    // --- ���ʂ̕`��ݒ� ---
    m_pImmediateContext->IASetInputLayout(m_pVertexLayout);
    m_pImmediateContext->VSSetShader(m_pVertexShader, nullptr, 0);
    m_pImmediateContext->PSSetShader(m_pPixelShader, nullptr, 0);
    m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);

    // --- �e�I�u�W�F�N�g�̕`�� ---
    RenderPlayer(game.getPlayerX(), game.getPlayerY());
    RenderBullets(game);
    RenderObstacles(game);

    // --- �o�b�N�o�b�t�@����ʂɕ\�� ---
    m_pSwapChain->Present(1, 0);
}

/**
 * @brief �v���C���[��`�悵�܂��B
 * @param x �v���C���[��X���W
 * @param y �v���C���[��Y���W
 */
void Graphics::RenderPlayer(float x, float y)
{
    // �萔�o�b�t�@���X�V���ăv���C���[�̍��W���V�F�[�_�[�ɓn��
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    m_pImmediateContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    ConstantBuffer* cb = (ConstantBuffer*)mappedResource.pData;
    cb->x_offset = x;
    cb->y_offset = y;
    m_pImmediateContext->Unmap(m_pConstantBuffer, 0);

    // �v���C���[�̒��_�o�b�t�@��ݒ�
    UINT stride = sizeof(SimpleVertex);
    UINT offset = 0;
    m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
    // �v���~�e�B�u�g�|���W�[���O�p�`���X�g�ɐݒ�
    m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    // 3���_�ŕ`��
    m_pImmediateContext->Draw(3, 0);
}

/**
 * @brief ���ׂĂ̗L���Ȓe��`�悵�܂��B
 * @param game �e�̏�������Game�I�u�W�F�N�g
 */
void Graphics::RenderBullets(const Game& game)
{
    // �e�̒��_�o�b�t�@�ƃv���~�e�B�u�g�|���W�[��ݒ�
    UINT stride = sizeof(SimpleVertex);
    UINT offset = 0;
    m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pBulletVertexBuffer, &stride, &offset);
    m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); // �l�p�`�Ȃ̂Ńg���C�A���O���X�g���b�v

    const Bullet* bullets = game.getBullets();
    // ���ׂĂ̒e���`�F�b�N
    for (int i = 0; i < game.getMaxBullets(); ++i)
    {
        if (bullets[i].IsActive()) // �L���Ȓe�̂ݕ`��
        {
            // �萔�o�b�t�@���X�V���Ēe�̍��W���V�F�[�_�[�ɓn��
            D3D11_MAPPED_SUBRESOURCE mappedResource;
            m_pImmediateContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
            ConstantBuffer* cb = (ConstantBuffer*)mappedResource.pData;
            cb->x_offset = bullets[i].GetX();
            cb->y_offset = bullets[i].GetY();
            m_pImmediateContext->Unmap(m_pConstantBuffer, 0);

            // 4���_�ŕ`��
            m_pImmediateContext->Draw(4, 0);
        }
    }
}

/**
 * @brief ���ׂĂ̗L���ȏ�Q����`�悵�܂��B
 * @param game ��Q���̏�������Game�I�u�W�F�N�g
 */
void Graphics::RenderObstacles(const Game& game)
{
    // ��Q���̒��_�o�b�t�@�ƃv���~�e�B�u�g�|���W�[��ݒ�
    UINT stride = sizeof(SimpleVertex);
    UINT offset = 0;
    m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pObstacleVertexBuffer, &stride, &offset);
    m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); // �l�p�`�Ȃ̂Ńg���C�A���O���X�g���b�v

    const Obstacle* obstacles = game.getObstacles();
    // ���ׂĂ̏�Q�����`�F�b�N
    for (int i = 0; i < game.getMaxObstacles(); ++i)
    {
        if (obstacles[i].IsActive()) // �L���ȏ�Q���̂ݕ`��
        {
            // �萔�o�b�t�@���X�V���ď�Q���̍��W���V�F�[�_�[�ɓn��
            D3D11_MAPPED_SUBRESOURCE mappedResource;
            m_pImmediateContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
            ConstantBuffer* cb = (ConstantBuffer*)mappedResource.pData;
            cb->x_offset = obstacles[i].GetX();
            cb->y_offset = obstacles[i].GetY();
            m_pImmediateContext->Unmap(m_pConstantBuffer, 0);

            // 4���_�ŕ`��
            m_pImmediateContext->Draw(4, 0);
        }
    }
}