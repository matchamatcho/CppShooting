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
    float scale;    // �I�u�W�F�N�g�̊g�嗦
    float padding1; // 16�o�C�g�A���C�����g�̂��߂̃p�f�B���O
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
 * @details ���ׂẴ����o�|�C���^��nullptr�ŏ�����
 */
Graphics::Graphics() :
    m_pSwapChain(nullptr),
    m_pd3dDevice(nullptr),
    m_pImmediateContext(nullptr),
    m_pRenderTargetView(nullptr),
    m_pVertexShader(nullptr),
    m_pPixelShader(nullptr),
    m_pVertexLayout(nullptr),
    m_pConstantBuffer(nullptr),
    m_pSquareVertexBuffer(nullptr),
    m_pTriangleVertexBuffer(nullptr),
    m_pPentagonVertexBuffer(nullptr),
    m_pObstacleVertexBuffer(nullptr)
{
}

/**
 * @brief Graphics�N���X�̃f�X�g���N�^
 * @details Shutdown()���Ăяo���ă��\�[�X�����
 */
Graphics::~Graphics() {
    Shutdown();
}

/**
 * @brief DirectX�̏��������s���܂��B
 * @param hWnd �E�B���h�E�n���h��
 * @return HRESULT ��������S_OK�A���s���̓G���[�R�[�h
 * @details �X���b�v�`�F�[���A�f�o�C�X�A�V�F�[�_�[�A���_�o�b�t�@�Ȃǂ��쐬
 */
HRESULT Graphics::Initialize(HWND hWnd) {
    HRESULT hr = S_OK;

    // --- �X���b�v�`�F�[���A�f�o�C�X�A�����_�[�^�[�Q�b�g���̏����� ---
    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferCount = 1;                                    // �o�b�N�o�b�t�@��
    sd.BufferDesc.Width = 800;                             // �E�B���h�E��
    sd.BufferDesc.Height = 600;                            // �E�B���h�E����
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // �s�N�Z���`��
    sd.BufferDesc.RefreshRate.Numerator = 60;              // ���t���b�V�����[�g�i���q�j
    sd.BufferDesc.RefreshRate.Denominator = 1;             // ���t���b�V�����[�g�i����j
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;       // �����_�[�^�[�Q�b�g�Ƃ��Ďg�p
    sd.OutputWindow = hWnd;                                 // �o�̓E�B���h�E
    sd.SampleDesc.Count = 1;                               // �}���`�T���v�����O��
    sd.SampleDesc.Quality = 0;                             // �}���`�T���v�����O�i��
    sd.Windowed = TRUE;                                    // �E�B���h�E���[�h

    // DirectX 11.0�̋@�\���x�����w��
    D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };

    // �f�o�C�X�ƃX���b�v�`�F�[���𓯎��ɍ쐬
    hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
        featureLevels, 1, D3D11_SDK_VERSION, &sd,
        &m_pSwapChain, &m_pd3dDevice, nullptr, &m_pImmediateContext);
    if (FAILED(hr)) {
        // DirectX�f�o�C�X�쐬�Ɏ��s
        return hr;
    }

    // �o�b�N�o�b�t�@���擾���ă����_�[�^�[�Q�b�g�r���[���쐬
    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
    if (FAILED(hr)) {
        // �o�b�N�o�b�t�@�擾�Ɏ��s
        return hr;
    }

    hr = m_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRenderTargetView);
    pBackBuffer->Release(); // �o�b�N�o�b�t�@�̎Q�ƃJ�E���g�����炷
    if (FAILED(hr)) {
        // �����_�[�^�[�Q�b�g�r���[�쐬�Ɏ��s
        return hr;
    }

    // �����_�[�^�[�Q�b�g��ݒ�
    m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, nullptr);

    // �r���[�|�[�g��ݒ�i�`��̈�̒�`�j
    D3D11_VIEWPORT vp;
    vp.Width = 800.0f;      // �r���[�|�[�g��
    vp.Height = 600.0f;     // �r���[�|�[�g����
    vp.MinDepth = 0.0f;     // �ŏ��[�x�l
    vp.MaxDepth = 1.0f;     // �ő�[�x�l
    vp.TopLeftX = 0;        // ����X���W
    vp.TopLeftY = 0;        // ����Y���W
    m_pImmediateContext->RSSetViewports(1, &vp);

    // --- �V�F�[�_�[�̍쐬 ---
    ID3DBlob* pVSBlob = nullptr;

    // ���_�V�F�[�_�[: �X�P�[���ƕ��s�ړ���K�p
    const char* vsShaderCode =
        "cbuffer ConstantBuffer : register(b0) { float2 offset; float scale; float padding; };\n"
        "struct VS_INPUT { float4 Pos : POSITION; float4 Color : COLOR; };\n"
        "struct PS_INPUT { float4 Pos : SV_POSITION; float4 Color : COLOR; };\n"
        "PS_INPUT VS(VS_INPUT input) {\n"
        "    PS_INPUT output = (PS_INPUT)0;\n"
        "    output.Pos = input.Pos;\n"
        "    output.Pos.xy *= scale;\n"    // �g��E�k����K�p
        "    output.Pos.xy += offset;\n"   // ���s�ړ���K�p
        "    output.Color = input.Color; return output;\n"
        "}\n";

    // ���_�V�F�[�_�[���R���p�C��
    D3DCompile(vsShaderCode, strlen(vsShaderCode), nullptr, nullptr, nullptr, "VS", "vs_4_0", 0, 0, &pVSBlob, nullptr);
    m_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &m_pVertexShader);

    ID3DBlob* pPSBlob = nullptr;

    // �s�N�Z���V�F�[�_�[: ���_�J���[�����̂܂܏o��
    const char* psShaderCode =
        "struct PS_INPUT { float4 Pos : SV_POSITION; float4 Color : COLOR; };\n"
        "float4 PS(PS_INPUT input) : SV_Target { return input.Color; }\n";

    // �s�N�Z���V�F�[�_�[���R���p�C��
    D3DCompile(psShaderCode, strlen(psShaderCode), nullptr, nullptr, nullptr, "PS", "ps_4_0", 0, 0, &pPSBlob, nullptr);
    m_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &m_pPixelShader);

    // --- ���̓��C�A�E�g�̍쐬 ---
    // ���_�f�[�^�̍\�����`
    D3D11_INPUT_ELEMENT_DESC layout[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },     // ���W�f�[�^
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },   // �J���[�f�[�^
    };
    m_pd3dDevice->CreateInputLayout(layout, 2, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &m_pVertexLayout);

    // �V�F�[�_�[�u���u�����
    pVSBlob->Release();
    pPSBlob->Release();

    // --- �e�Ə�Q���A�v���C���[�̊e�p�[�c�̒��_�o�b�t�@���쐬 ---

    // 1. �l�p�` (�F)
    {
        SimpleVertex vertices[] = {
            { { -0.04f, -0.04f, 0.5f }, { 0.2f, 0.2f, 0.8f, 1.0f } }, // ����
            { { -0.04f,  0.04f, 0.5f }, { 0.2f, 0.2f, 0.8f, 1.0f } }, // ����
            { {  0.04f, -0.04f, 0.5f }, { 0.2f, 0.2f, 0.8f, 1.0f } }, // �E��
            { {  0.04f,  0.04f, 0.5f }, { 0.2f, 0.2f, 0.8f, 1.0f } }  // �E��
        };
        D3D11_BUFFER_DESC bd = {};
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(vertices);
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        D3D11_SUBRESOURCE_DATA InitData = { vertices };
        hr = m_pd3dDevice->CreateBuffer(&bd, &InitData, &m_pSquareVertexBuffer);
        if (FAILED(hr)) return hr;
    }

    // 2. �O�p�` (�ԐF)
    {
        SimpleVertex vertices[] = {
            { { -0.04f, -0.04f, 0.5f }, { 0.8f, 0.2f, 0.2f, 1.0f } }, // ����
            { {  0.0f,   0.08f, 0.5f }, { 0.8f, 0.2f, 0.2f, 1.0f } }, // ��
            { {  0.04f, -0.04f, 0.5f }, { 0.8f, 0.2f, 0.2f, 1.0f } }  // �E��
        };
        D3D11_BUFFER_DESC bd = {};
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(vertices);
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        D3D11_SUBRESOURCE_DATA InitData = { vertices };
        hr = m_pd3dDevice->CreateBuffer(&bd, &InitData, &m_pTriangleVertexBuffer);
        if (FAILED(hr)) return hr;
    }

    // 3. �܊p�` (�ΐF)
    {
        SimpleVertex pentagonVerts[] = {
            { { 0.0f,    -0.08f, 0.5f }, { 0.2f, 0.8f, 0.2f, 1.0f } }, // ��
            { {-0.076f, -0.025f, 0.5f }, { 0.2f, 0.8f, 0.2f, 1.0f } }, // ����
            { {-0.047f,  0.065f, 0.5f }, { 0.2f, 0.8f, 0.2f, 1.0f } }, // ����
            { { 0.047f,  0.065f, 0.5f }, { 0.2f, 0.8f, 0.2f, 1.0f } }, // �E��
            { { 0.076f, -0.025f, 0.5f }, { 0.2f, 0.8f, 0.2f, 1.0f } }, // �E��
        };

        SimpleVertex vertices[] = {
             pentagonVerts[0], pentagonVerts[1], pentagonVerts[2],
             pentagonVerts[0], pentagonVerts[2], pentagonVerts[3],
             pentagonVerts[0], pentagonVerts[3], pentagonVerts[4]
        };
        D3D11_BUFFER_DESC bd = {};
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(vertices);
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        D3D11_SUBRESOURCE_DATA InitData = { vertices };
        hr = m_pd3dDevice->CreateBuffer(&bd, &InitData, &m_pPentagonVertexBuffer);
        if (FAILED(hr)) return hr;
    }

    // --- ��Q���p�̒��_�o�b�t�@�́A�e�̃o�b�t�@���ė��p���邽�߁A�����ł͍쐬���Ȃ� ---

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
 * @details ���ׂĂ�COM�C���^�[�t�F�[�X�����S�ɉ��
 */
void Graphics::Shutdown() {
    if (m_pImmediateContext) m_pImmediateContext->ClearState();
    if (m_pObstacleVertexBuffer) { m_pObstacleVertexBuffer->Release(); m_pObstacleVertexBuffer = nullptr; }
    if (m_pPentagonVertexBuffer) { m_pPentagonVertexBuffer->Release(); m_pPentagonVertexBuffer = nullptr; }
    if (m_pTriangleVertexBuffer) { m_pTriangleVertexBuffer->Release(); m_pTriangleVertexBuffer = nullptr; }
    if (m_pSquareVertexBuffer) { m_pSquareVertexBuffer->Release(); m_pSquareVertexBuffer = nullptr; }
    if (m_pConstantBuffer) { m_pConstantBuffer->Release(); m_pConstantBuffer = nullptr; }
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
 * @param game �Q�[����ԃI�u�W�F�N�g�̎Q��
 * @details ��ʃN���A�A�e�I�u�W�F�N�g�̕`��A��ʕ\�������s
 */
void Graphics::RenderFrame(const Game& game) {
    float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // R, G, B, A
    m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, ClearColor);

    m_pImmediateContext->IASetInputLayout(m_pVertexLayout);
    m_pImmediateContext->VSSetShader(m_pVertexShader, nullptr, 0);
    m_pImmediateContext->PSSetShader(m_pPixelShader, nullptr, 0);
    m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);

    const Player& player = game.getPlayer();
    RenderPlayer(player.GetX(), player.GetY(), player.GetLeftShape(), player.GetCenterShape(), player.GetRightShape());
    RenderBullets(game);
    RenderObstacles(game);

    m_pSwapChain->Present(1, 0);
}

/**
 * @brief �v���C���[��`�悵�܂��B
 * @param x �v���C���[��X���W
 * @param y �v���C���[��Y���W
 * @param leftShape ���̐�Ԃ̌`��
 * @param centerShape �^�񒆂̐�Ԃ̌`��
 * @param rightShape �E�̐�Ԃ̌`��
 */
void Graphics::RenderPlayer(float x, float y, BulletShape leftShape, BulletShape centerShape, BulletShape rightShape)
{
    // �e�p�[�c�̈ʒu�I�t�Z�b�g
    float leftX = x - 0.1f;
    float leftY = y + 0.06f;

    float centerX = x;
    float centerY = y + 0.1f;

    float rightX = x + 0.1f;
    float rightY = y + 0.06f;

    // �e�p�[�c��`��
    DrawShape(leftShape, leftX, leftY, 1.0f);
    DrawShape(centerShape, centerX, centerY, 1.0f);
    DrawShape(rightShape, rightX, rightY, 1.0f);
}

/**
 * @brief ���ׂĂ̗L���Ȓe��`�悵�܂��B
 * @param game �Q�[����ԃI�u�W�F�N�g�̎Q��
 */
void Graphics::RenderBullets(const Game& game)
{
    const Bullet* bullets = game.getBullets();
    for (int i = 0; i < game.getMaxBullets(); ++i)
    {
        if (bullets[i].IsActive())
        {
            DrawShape(bullets[i].GetShape(), bullets[i].GetX(), bullets[i].GetY(), 0.2f);
        }
    }
}

/**
 * @brief ���ׂĂ̗L���ȏ�Q����`�悵�܂��B
 * @param game �Q�[����ԃI�u�W�F�N�g�̎Q��
 */
void Graphics::RenderObstacles(const Game& game)
{
    const Obstacle* obstacles = game.getObstacles();
    for (int i = 0; i < game.getMaxObstacles(); ++i)
    {
        if (obstacles[i].IsActive())
        {
            DrawShape(obstacles[i].GetShape(), obstacles[i].GetX(), obstacles[i].GetY(), OBSTACLE_SCALE);
        }
    }
}

/**
 * @brief �w�肳�ꂽ�`���`�悵�܂��B
 * @param shape �`�悷��`��
 * @param xOffset X���W�̃I�t�Z�b�g
 * @param yOffset Y���W�̃I�t�Z�b�g
 * @param scale �g�嗦
 */
void Graphics::DrawShape(BulletShape shape, float xOffset, float yOffset, float scale)
{
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    m_pImmediateContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    ConstantBuffer* cb = (ConstantBuffer*)mappedResource.pData;
    cb->x_offset = xOffset;
    cb->y_offset = yOffset;
    cb->scale = scale;
    m_pImmediateContext->Unmap(m_pConstantBuffer, 0);

    UINT stride = sizeof(SimpleVertex);
    UINT offset = 0;

    switch (shape)
    {
    case BulletShape::Square:
        m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pSquareVertexBuffer, &stride, &offset);
        m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
        m_pImmediateContext->Draw(4, 0);
        break;
    case BulletShape::Triangle:
        m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pTriangleVertexBuffer, &stride, &offset);
        m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        m_pImmediateContext->Draw(3, 0);
        break;
    case BulletShape::Pentagon:
        m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pPentagonVertexBuffer, &stride, &offset);
        m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        m_pImmediateContext->Draw(9, 0);
        break;
    }
}