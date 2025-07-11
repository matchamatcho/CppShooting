#include "Graphics.h"
#include <d3dcompiler.h> // �V�F�[�_�[�R���p�C���p
#include "GameConfig.h"  // �Q�[���ݒ�t�@�C�����C���N���[�h

// �K�v�ȃ��C�u�����������N
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

/**
 * @struct ConstantBuffer
 * @brief �V�F�[�_�[�ɓn���萔�o�b�t�@�̃f�[�^�\����
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
 */
struct SimpleVertex {
    float Pos[3];   // ���W (x, y, z)
    float Color[4]; // �F (r, g, b, a)
};

/**
 * @brief Graphics�N���X�̃R���X�g���N�^
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
    m_pSquareBulletVertexBuffer(nullptr),
    m_pTriangleBulletVertexBuffer(nullptr),
    m_pPentagonBulletVertexBuffer(nullptr),
    m_pObstacleVertexBuffer(nullptr)
{
}

/**
 * @brief Graphics�N���X�̃f�X�g���N�^
 */
Graphics::~Graphics() {
    Shutdown();
}

/**
 * @brief DirectX�̏��������s���܂��B
 */
HRESULT Graphics::Initialize(HWND hWnd) {
    HRESULT hr = S_OK;

    // --- �X���b�v�`�F�[���A�f�o�C�X�A�����_�[�^�[�Q�b�g���̏����� ---
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
    vp.Width = 800.0f; vp.Height = 600.0f; vp.MinDepth = 0.0f; vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0; vp.TopLeftY = 0;
    m_pImmediateContext->RSSetViewports(1, &vp);

    // --- �V�F�[�_�[�̍쐬 ---
    ID3DBlob* pVSBlob = nullptr;
    const char* vsShaderCode =
        "cbuffer ConstantBuffer : register(b0) { float2 offset; };\n"
        "struct VS_INPUT { float4 Pos : POSITION; float4 Color : COLOR; };\n"
        "struct PS_INPUT { float4 Pos : SV_POSITION; float4 Color : COLOR; };\n"
        "PS_INPUT VS(VS_INPUT input) {\n"
        "    PS_INPUT output = (PS_INPUT)0;\n"
        "    output.Pos = input.Pos; output.Pos.xy += offset;\n"
        "    output.Color = input.Color; return output;\n"
        "}\n";
    D3DCompile(vsShaderCode, strlen(vsShaderCode), nullptr, nullptr, nullptr, "VS", "vs_4_0", 0, 0, &pVSBlob, nullptr);
    m_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &m_pVertexShader);

    ID3DBlob* pPSBlob = nullptr;
    const char* psShaderCode =
        "struct PS_INPUT { float4 Pos : SV_POSITION; float4 Color : COLOR; };\n"
        "float4 PS(PS_INPUT input) : SV_Target { return input.Color; }\n";
    D3DCompile(psShaderCode, strlen(psShaderCode), nullptr, nullptr, nullptr, "PS", "ps_4_0", 0, 0, &pPSBlob, nullptr);
    m_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &m_pPixelShader);

    // --- ���̓��C�A�E�g�̍쐬 ---
    D3D11_INPUT_ELEMENT_DESC layout[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    m_pd3dDevice->CreateInputLayout(layout, 2, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &m_pVertexLayout);
    pVSBlob->Release();
    pPSBlob->Release();

    // --- �v���C���[�̒��_�o�b�t�@�쐬 (�����琳���`�A���O�p�`�A���܊p�`) ---
    {
        SimpleVertex vertices[] = {
            // 1. ���̐����` (4���_) - �F
            { { -0.14f, -0.04f, 0.5f }, { 0.2f, 0.2f, 0.8f, 1.0f } }, // ����
            { { -0.14f,  0.04f, 0.5f }, { 0.2f, 0.2f, 0.8f, 1.0f } }, // ����
            { { -0.06f, -0.04f, 0.5f }, { 0.2f, 0.2f, 0.8f, 1.0f } }, // �E��
            { { -0.06f,  0.04f, 0.5f }, { 0.2f, 0.2f, 0.8f, 1.0f } }, // �E��

            // 2. �����̐��O�p�` (3���_) - �ԐF
            { { -0.04f, -0.04f, 0.5f }, { 0.8f, 0.2f, 0.2f, 1.0f } }, // ����
            { {  0.0f,   0.08f, 0.5f }, { 0.8f, 0.2f, 0.2f, 1.0f } }, // ��
            { {  0.04f, -0.04f, 0.5f }, { 0.8f, 0.2f, 0.2f, 1.0f } }, // �E��

            // 3. �E�̐��܊p�` (3�̎O�p�`�ŕ\�����邽��9���_) - �ΐF
            { { 0.07f, -0.04f, 0.5f }, { 0.2f, 0.8f, 0.2f, 1.0f } }, { { 0.05f,  0.02f, 0.5f }, { 0.2f, 0.8f, 0.2f, 1.0f } }, { { 0.1f,   0.08f, 0.5f }, { 0.2f, 0.8f, 0.2f, 1.0f } },
            { { 0.07f, -0.04f, 0.5f }, { 0.2f, 0.8f, 0.2f, 1.0f } }, { { 0.1f,   0.08f, 0.5f }, { 0.2f, 0.8f, 0.2f, 1.0f } }, { { 0.15f,  0.02f, 0.5f }, { 0.2f, 0.8f, 0.2f, 1.0f } },
            { { 0.07f, -0.04f, 0.5f }, { 0.2f, 0.8f, 0.2f, 1.0f } }, { { 0.15f,  0.02f, 0.5f }, { 0.2f, 0.8f, 0.2f, 1.0f } }, { { 0.13f, -0.04f, 0.5f }, { 0.2f, 0.8f, 0.2f, 1.0f } },
        };
        D3D11_BUFFER_DESC bd = {};
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(vertices);
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        D3D11_SUBRESOURCE_DATA InitData = { vertices };
        hr = m_pd3dDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer);
        if (FAILED(hr)) return hr;
    }

    // --- �e�̒��_�o�b�t�@�쐬 (�`�󂲂�) ---
    // 1. �l�p�`�̒e (����ԂɑΉ� -> �n�̒e)
    {
        SimpleVertex vertices[] = {
            { { -BULLET_HALF_WIDTH, -BULLET_HALF_HEIGHT, 0.5f }, { 0.5f, 0.8f, 1.0f, 1.0f } },
            { { -BULLET_HALF_WIDTH,  BULLET_HALF_HEIGHT, 0.5f }, { 0.5f, 0.8f, 1.0f, 1.0f } },
            { {  BULLET_HALF_WIDTH, -BULLET_HALF_HEIGHT, 0.5f }, { 0.5f, 0.8f, 1.0f, 1.0f } },
            { {  BULLET_HALF_WIDTH,  BULLET_HALF_HEIGHT, 0.5f }, { 0.5f, 0.8f, 1.0f, 1.0f } },
        };
        D3D11_BUFFER_DESC bd = {};
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(vertices);
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        D3D11_SUBRESOURCE_DATA InitData = { vertices };
        hr = m_pd3dDevice->CreateBuffer(&bd, &InitData, &m_pSquareBulletVertexBuffer);
        if (FAILED(hr)) return hr;
    }
    // 2. �O�p�`�̒e (�Ԃ���ԂɑΉ� -> �Ԍn�̒e)
    {
        SimpleVertex vertices[] = {
            { { -0.015f, -0.015f, 0.5f }, { 1.0f, 0.6f, 0.6f, 1.0f } },
            { {  0.0f,    0.025f, 0.5f }, { 1.0f, 0.6f, 0.6f, 1.0f } },
            { {  0.015f, -0.015f, 0.5f }, { 1.0f, 0.6f, 0.6f, 1.0f } },
        };
        D3D11_BUFFER_DESC bd = {};
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(vertices);
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        D3D11_SUBRESOURCE_DATA InitData = { vertices };
        hr = m_pd3dDevice->CreateBuffer(&bd, &InitData, &m_pTriangleBulletVertexBuffer);
        if (FAILED(hr)) return hr;
    }
    // 3. �܊p�`�̒e (�΂̐�ԂɑΉ� -> �Όn�̒e)
    {
        SimpleVertex pentagonVerts[] = {
            { { 0.0f,    -0.02f, 0.5f }, { 0.6f, 1.0f, 0.6f, 1.0f } },
            { {-0.019f, -0.006f, 0.5f }, { 0.6f, 1.0f, 0.6f, 1.0f } },
            { {-0.012f,  0.016f, 0.5f }, { 0.6f, 1.0f, 0.6f, 1.0f } },
            { { 0.012f,  0.016f, 0.5f }, { 0.6f, 1.0f, 0.6f, 1.0f } },
            { { 0.019f, -0.006f, 0.5f }, { 0.6f, 1.0f, 0.6f, 1.0f } },
        };
        SimpleVertex vertices[] = { // TRIANGLELIST�p�ɕ���
             pentagonVerts[0], pentagonVerts[1], pentagonVerts[2],
             pentagonVerts[0], pentagonVerts[2], pentagonVerts[3],
             pentagonVerts[0], pentagonVerts[3], pentagonVerts[4]
        };
        D3D11_BUFFER_DESC bd = {};
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(vertices);
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        D3D11_SUBRESOURCE_DATA InitData = { vertices };
        hr = m_pd3dDevice->CreateBuffer(&bd, &InitData, &m_pPentagonBulletVertexBuffer);
        if (FAILED(hr)) return hr;
    }


    // --- ��Q���̒��_�o�b�t�@�쐬 (�`��͓��I�ɕς�邽�߁A�����ł̓_�~�[�Ƃ��č쐬) ---
    {
        SimpleVertex vertices[] = {
            { { -0.05f, -0.05f, 0.5f }, { 0.5f, 0.5f, 0.5f, 1.0f } },
            { { -0.05f,  0.05f, 0.5f }, { 0.5f, 0.5f, 0.5f, 1.0f } },
            { {  0.05f, -0.05f, 0.5f }, { 0.5f, 0.5f, 0.5f, 1.0f } },
            { {  0.05f,  0.05f, 0.5f }, { 0.5f, 0.5f, 0.5f, 1.0f } }
        };
        D3D11_BUFFER_DESC bd = {};
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(SimpleVertex) * 4;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        D3D11_SUBRESOURCE_DATA InitData = { vertices };
        // m_pObstacleVertexBuffer �͂����g��Ȃ����A��������̂��߂Ɏc���Ă���
        hr = m_pd3dDevice->CreateBuffer(&bd, &InitData, &m_pObstacleVertexBuffer);
        if (FAILED(hr)) return hr;
    }

    // --- �萔�o�b�t�@�̍쐬 ---
    {
        D3D11_BUFFER_DESC cbd = {};
        cbd.Usage = D3D11_USAGE_DYNAMIC;
        cbd.ByteWidth = sizeof(ConstantBuffer);
        cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        hr = m_pd3dDevice->CreateBuffer(&cbd, nullptr, &m_pConstantBuffer);
        if (FAILED(hr)) return hr;
    }

    return S_OK;
}

/**
 * @brief DirectX���\�[�X���N���[���A�b�v���܂��B
 */
void Graphics::Shutdown() {
    if (m_pImmediateContext) m_pImmediateContext->ClearState();

    if (m_pObstacleVertexBuffer) { m_pObstacleVertexBuffer->Release(); m_pObstacleVertexBuffer = nullptr; }
    // --- �e�̒��_�o�b�t�@�̉������ ---
    if (m_pPentagonBulletVertexBuffer) { m_pPentagonBulletVertexBuffer->Release(); m_pPentagonBulletVertexBuffer = nullptr; }
    if (m_pTriangleBulletVertexBuffer) { m_pTriangleBulletVertexBuffer->Release(); m_pTriangleBulletVertexBuffer = nullptr; }
    if (m_pSquareBulletVertexBuffer) { m_pSquareBulletVertexBuffer->Release(); m_pSquareBulletVertexBuffer = nullptr; }

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
 */
void Graphics::RenderFrame(const Game& game) {
    float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
    m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, ClearColor);

    m_pImmediateContext->IASetInputLayout(m_pVertexLayout);
    m_pImmediateContext->VSSetShader(m_pVertexShader, nullptr, 0);
    m_pImmediateContext->PSSetShader(m_pPixelShader, nullptr, 0);
    m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);

    const Player& player = game.getPlayer();
    RenderPlayer(player.GetX(), player.GetY());
    RenderBullets(game);
    RenderObstacles(game);

    m_pSwapChain->Present(1, 0);
}

/**
 * @brief �v���C���[��`�悵�܂��B
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

    // �e�p�[�c��`��
    // 1. ���̐����` (���_�C���f�b�N�X0����4���_)
    m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    m_pImmediateContext->Draw(4, 0);

    // 2. �����̐��O�p�` (���_�C���f�b�N�X4����3���_)
    m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_pImmediateContext->Draw(3, 4);

    // 3. �E�̐��܊p�` (���_�C���f�b�N�X7����9���_ = 3�̎O�p�`)
    m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_pImmediateContext->Draw(9, 7);
}

/**
 * @brief ���ׂĂ̗L���Ȓe��`�悵�܂��B
 */
void Graphics::RenderBullets(const Game& game)
{
    UINT stride = sizeof(SimpleVertex);
    UINT offset = 0;

    const Bullet* bullets = game.getBullets();
    for (int i = 0; i < game.getMaxBullets(); ++i)
    {
        if (bullets[i].IsActive())
        {
            // �萔�o�b�t�@�ɒe�̍��W����������
            D3D11_MAPPED_SUBRESOURCE mappedResource;
            m_pImmediateContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
            ConstantBuffer* cb = (ConstantBuffer*)mappedResource.pData;
            cb->x_offset = bullets[i].GetX();
            cb->y_offset = bullets[i].GetY();
            m_pImmediateContext->Unmap(m_pConstantBuffer, 0);

            // �e�̌`��ɉ����āA�g�p���钸�_�o�b�t�@�ƕ`����@��؂�ւ���
            switch (bullets[i].GetShape())
            {
            case BulletShape::Square:
                m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pSquareBulletVertexBuffer, &stride, &offset);
                m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
                m_pImmediateContext->Draw(4, 0); // 4���_�Ŏl�p�`��`��
                break;

            case BulletShape::Triangle:
                m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pTriangleBulletVertexBuffer, &stride, &offset);
                m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
                m_pImmediateContext->Draw(3, 0); // 3���_�ŎO�p�`��`��
                break;

            case BulletShape::Pentagon:
                m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pPentagonBulletVertexBuffer, &stride, &offset);
                m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
                m_pImmediateContext->Draw(9, 0); // 9���_(3�̎O�p�`)�Ō܊p�`��`��
                break;
            }
        }
    }
}

/**
 * @brief ���ׂĂ̗L���ȏ�Q����`�悵�܂��B
 */
void Graphics::RenderObstacles(const Game& game)
{
    UINT stride = sizeof(SimpleVertex);
    UINT offset = 0;

    const Obstacle* obstacles = game.getObstacles();
    for (int i = 0; i < game.getMaxObstacles(); ++i)
    {
        if (obstacles[i].IsActive())
        {
            D3D11_MAPPED_SUBRESOURCE mappedResource;
            m_pImmediateContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
            ConstantBuffer* cb = (ConstantBuffer*)mappedResource.pData;
            cb->x_offset = obstacles[i].GetX();
            cb->y_offset = obstacles[i].GetY();
            m_pImmediateContext->Unmap(m_pConstantBuffer, 0);

            // ��Q���̌`��ɉ����āA�g�p���钸�_�o�b�t�@�ƕ`����@��؂�ւ���
            switch (obstacles[i].GetShape())
            {
            case BulletShape::Square:
                m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pSquareBulletVertexBuffer, &stride, &offset);
                m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
                m_pImmediateContext->Draw(4, 0);
                break;

            case BulletShape::Triangle:
                m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pTriangleBulletVertexBuffer, &stride, &offset);
                m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
                m_pImmediateContext->Draw(3, 0);
                break;

            case BulletShape::Pentagon:
                m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pPentagonBulletVertexBuffer, &stride, &offset);
                m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
                m_pImmediateContext->Draw(9, 0);
                break;
            }
        }
    }
}