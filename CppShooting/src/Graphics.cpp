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

    // --- �v���C���[�̒��_�o�b�t�@�쐬 (�����琳���`�A���O�p�`�A���܊p�`) ---
    {
        SimpleVertex vertices[] = {
            // 1. ���̐����` (4���_) - �F
            { { -0.14f, -0.04f, 0.5f }, { 0.2f, 0.2f, 0.8f, 1.0f } }, // �������_
            { { -0.14f,  0.04f, 0.5f }, { 0.2f, 0.2f, 0.8f, 1.0f } }, // ���㒸�_
            { { -0.06f, -0.04f, 0.5f }, { 0.2f, 0.2f, 0.8f, 1.0f } }, // �E�����_
            { { -0.06f,  0.04f, 0.5f }, { 0.2f, 0.2f, 0.8f, 1.0f } }, // �E�㒸�_

            // 2. �����̐��O�p�` (3���_) - �ԐF
            { { -0.04f, -0.04f, 0.5f }, { 0.8f, 0.2f, 0.2f, 1.0f } }, // �������_
            { {  0.0f,   0.08f, 0.5f }, { 0.8f, 0.2f, 0.2f, 1.0f } }, // �㒸�_
            { {  0.04f, -0.04f, 0.5f }, { 0.8f, 0.2f, 0.2f, 1.0f } }, // �E�����_

            // 3. �E�̐��܊p�` (3�̎O�p�`�ŕ\�����邽��9���_) - �ΐF
            // �܊p�`��3�̎O�p�`�ɕ������ĕ\��
            { { 0.07f, -0.04f, 0.5f }, { 0.2f, 0.8f, 0.2f, 1.0f } }, { { 0.05f,  0.02f, 0.5f }, { 0.2f, 0.8f, 0.2f, 1.0f } }, { { 0.1f,   0.08f, 0.5f }, { 0.2f, 0.8f, 0.2f, 1.0f } },
            { { 0.07f, -0.04f, 0.5f }, { 0.2f, 0.8f, 0.2f, 1.0f } }, { { 0.1f,   0.08f, 0.5f }, { 0.2f, 0.8f, 0.2f, 1.0f } }, { { 0.15f,  0.02f, 0.5f }, { 0.2f, 0.8f, 0.2f, 1.0f } },
            { { 0.07f, -0.04f, 0.5f }, { 0.2f, 0.8f, 0.2f, 1.0f } }, { { 0.15f,  0.02f, 0.5f }, { 0.2f, 0.8f, 0.2f, 1.0f } }, { { 0.13f, -0.04f, 0.5f }, { 0.2f, 0.8f, 0.2f, 1.0f } },
        };

        // �v���C���[�p���_�o�b�t�@�̐ݒ�
        D3D11_BUFFER_DESC bd = {};
        bd.Usage = D3D11_USAGE_DEFAULT;        // �f�t�H���g�g�p�@
        bd.ByteWidth = sizeof(vertices);        // �o�b�t�@�T�C�Y
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // ���_�o�b�t�@�Ƃ��Ďg�p
        D3D11_SUBRESOURCE_DATA InitData = { vertices };
        hr = m_pd3dDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer);
        if (FAILED(hr)) {
            // �v���C���[���_�o�b�t�@�쐬�Ɏ��s
            return hr;
        }
    }

    // --- �e�̒��_�o�b�t�@�쐬 (�`�󂲂�) ---

    // 1. �l�p�`�̒e (����ԂɑΉ� -> �n�̒e)
    {
        SimpleVertex vertices[] = {
            // TRIANGLE_STRIP�`���Ŏl�p�`���`
            { { -BULLET_HALF_WIDTH, -BULLET_HALF_HEIGHT, 0.5f }, { 0.5f, 0.8f, 1.0f, 1.0f } }, // ����
            { { -BULLET_HALF_WIDTH,  BULLET_HALF_HEIGHT, 0.5f }, { 0.5f, 0.8f, 1.0f, 1.0f } }, // ����
            { {  BULLET_HALF_WIDTH, -BULLET_HALF_HEIGHT, 0.5f }, { 0.5f, 0.8f, 1.0f, 1.0f } }, // �E��
            { {  BULLET_HALF_WIDTH,  BULLET_HALF_HEIGHT, 0.5f }, { 0.5f, 0.8f, 1.0f, 1.0f } }, // �E��
        };

        D3D11_BUFFER_DESC bd = {};
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(vertices);
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        D3D11_SUBRESOURCE_DATA InitData = { vertices };
        hr = m_pd3dDevice->CreateBuffer(&bd, &InitData, &m_pSquareBulletVertexBuffer);
        if (FAILED(hr)) {
            // �l�p�`�e���_�o�b�t�@�쐬�Ɏ��s
            return hr;
        }
    }

    // 2. �O�p�`�̒e (�Ԃ���ԂɑΉ� -> �Ԍn�̒e)
    {
        SimpleVertex vertices[] = {
            // �O�p�`��3���_���`
            { { -0.015f, -0.015f, 0.5f }, { 1.0f, 0.6f, 0.6f, 1.0f } }, // ����
            { {  0.0f,    0.025f, 0.5f }, { 1.0f, 0.6f, 0.6f, 1.0f } }, // ��
            { {  0.015f, -0.015f, 0.5f }, { 1.0f, 0.6f, 0.6f, 1.0f } }, // �E��
        };

        D3D11_BUFFER_DESC bd = {};
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(vertices);
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        D3D11_SUBRESOURCE_DATA InitData = { vertices };
        hr = m_pd3dDevice->CreateBuffer(&bd, &InitData, &m_pTriangleBulletVertexBuffer);
        if (FAILED(hr)) {
            // �O�p�`�e���_�o�b�t�@�쐬�Ɏ��s
            return hr;
        }
    }

    // 3. �܊p�`�̒e (�΂̐�ԂɑΉ� -> �Όn�̒e)
    {
        // �܊p�`��5�̒��_���`
        SimpleVertex pentagonVerts[] = {
            { { 0.0f,    -0.02f, 0.5f }, { 0.6f, 1.0f, 0.6f, 1.0f } }, // ��
            { {-0.019f, -0.006f, 0.5f }, { 0.6f, 1.0f, 0.6f, 1.0f } }, // ����
            { {-0.012f,  0.016f, 0.5f }, { 0.6f, 1.0f, 0.6f, 1.0f } }, // ����
            { { 0.012f,  0.016f, 0.5f }, { 0.6f, 1.0f, 0.6f, 1.0f } }, // �E��
            { { 0.019f, -0.006f, 0.5f }, { 0.6f, 1.0f, 0.6f, 1.0f } }, // �E��
        };

        // TRIANGLE_LIST�p��3�̎O�p�`�ɕ���
        SimpleVertex vertices[] = {
             pentagonVerts[0], pentagonVerts[1], pentagonVerts[2], // ��1�O�p�`
             pentagonVerts[0], pentagonVerts[2], pentagonVerts[3], // ��2�O�p�`
             pentagonVerts[0], pentagonVerts[3], pentagonVerts[4]  // ��3�O�p�`
        };

        D3D11_BUFFER_DESC bd = {};
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(vertices);
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        D3D11_SUBRESOURCE_DATA InitData = { vertices };
        hr = m_pd3dDevice->CreateBuffer(&bd, &InitData, &m_pPentagonBulletVertexBuffer);
        if (FAILED(hr)) {
            // �܊p�`�e���_�o�b�t�@�쐬�Ɏ��s
            return hr;
        }
    }

    // --- ��Q���̒��_�o�b�t�@�쐬 ---
    // ����: ���ۂ̏�Q���͒e�̒��_�o�b�t�@���ė��p���邽�߁A����̓_�~�[
    {
        SimpleVertex vertices[] = {
            // �_�~�[�̎l�p�`�f�[�^�i�O���[�F�j
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
        if (FAILED(hr)) {
            // ��Q�����_�o�b�t�@�쐬�Ɏ��s
            return hr;
        }
    }

    // --- �萔�o�b�t�@�̍쐬 ---
    {
        D3D11_BUFFER_DESC cbd = {};
        cbd.Usage = D3D11_USAGE_DYNAMIC;           // ���I�g�p�@�iCPU�������݉\�j
        cbd.ByteWidth = sizeof(ConstantBuffer);     // �萔�o�b�t�@�̃T�C�Y
        cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // �萔�o�b�t�@�Ƃ��Ďg�p
        cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // CPU�������݃A�N�Z�X����
        hr = m_pd3dDevice->CreateBuffer(&cbd, nullptr, &m_pConstantBuffer);
        if (FAILED(hr)) {
            // �萔�o�b�t�@�쐬�Ɏ��s
            return hr;
        }
    }

    return S_OK;
}

/**
 * @brief DirectX���\�[�X���N���[���A�b�v���܂��B
 * @details ���ׂĂ�COM�C���^�[�t�F�[�X�����S�ɉ��
 */
void Graphics::Shutdown() {
    // �f�o�C�X�R���e�L�X�g�̏�Ԃ��N���A
    if (m_pImmediateContext) m_pImmediateContext->ClearState();

    // �e���\�[�X�����S�ɉ���inullptr�`�F�b�N�t���j
    if (m_pObstacleVertexBuffer) {
        m_pObstacleVertexBuffer->Release();
        m_pObstacleVertexBuffer = nullptr;
    }

    // --- �e�̒��_�o�b�t�@�̉������ ---
    if (m_pPentagonBulletVertexBuffer) {
        m_pPentagonBulletVertexBuffer->Release();
        m_pPentagonBulletVertexBuffer = nullptr;
    }
    if (m_pTriangleBulletVertexBuffer) {
        m_pTriangleBulletVertexBuffer->Release();
        m_pTriangleBulletVertexBuffer = nullptr;
    }
    if (m_pSquareBulletVertexBuffer) {
        m_pSquareBulletVertexBuffer->Release();
        m_pSquareBulletVertexBuffer = nullptr;
    }

    // ���̑��̃��\�[�X���
    if (m_pConstantBuffer) {
        m_pConstantBuffer->Release();
        m_pConstantBuffer = nullptr;
    }
    if (m_pVertexBuffer) {
        m_pVertexBuffer->Release();
        m_pVertexBuffer = nullptr;
    }
    if (m_pVertexLayout) {
        m_pVertexLayout->Release();
        m_pVertexLayout = nullptr;
    }
    if (m_pVertexShader) {
        m_pVertexShader->Release();
        m_pVertexShader = nullptr;
    }
    if (m_pPixelShader) {
        m_pPixelShader->Release();
        m_pPixelShader = nullptr;
    }
    if (m_pRenderTargetView) {
        m_pRenderTargetView->Release();
        m_pRenderTargetView = nullptr;
    }
    if (m_pSwapChain) {
        m_pSwapChain->Release();
        m_pSwapChain = nullptr;
    }
    if (m_pImmediateContext) {
        m_pImmediateContext->Release();
        m_pImmediateContext = nullptr;
    }
    if (m_pd3dDevice) {
        m_pd3dDevice->Release();
        m_pd3dDevice = nullptr;
    }
}

/**
 * @brief 1�t���[�����̕`�揈�����s���܂��B
 * @param game �Q�[����ԃI�u�W�F�N�g�̎Q��
 * @details ��ʃN���A�A�e�I�u�W�F�N�g�̕`��A��ʕ\�������s
 */
void Graphics::RenderFrame(const Game& game) {
    // ��ʂ�Z���F�ŃN���A
    float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // R, G, B, A
    m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, ClearColor);

    // �`��p�C�v���C���̐ݒ�
    m_pImmediateContext->IASetInputLayout(m_pVertexLayout);         // ���̓��C�A�E�g�ݒ�
    m_pImmediateContext->VSSetShader(m_pVertexShader, nullptr, 0);  // ���_�V�F�[�_�[�ݒ�
    m_pImmediateContext->PSSetShader(m_pPixelShader, nullptr, 0);   // �s�N�Z���V�F�[�_�[�ݒ�
    m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer); // �萔�o�b�t�@�ݒ�

    // �e�I�u�W�F�N�g��`��
    const Player& player = game.getPlayer();
    RenderPlayer(player.GetX(), player.GetY()); // �v���C���[�`��
    RenderBullets(game);                        // �e�`��
    RenderObstacles(game);                      // ��Q���`��

    // �o�b�N�o�b�t�@����ʂɕ\���i���������L���j
    m_pSwapChain->Present(1, 0);
}

/**
 * @brief �v���C���[��`�悵�܂��B
 * @param x �v���C���[��X���W
 * @param y �v���C���[��Y���W
 * @details �v���C���[��3�̃p�[�c�i�����`�A�O�p�`�A�܊p�`�j����\��
 */
void Graphics::RenderPlayer(float x, float y)
{
    // �萔�o�b�t�@���X�V���ăv���C���[�̍��W���V�F�[�_�[�ɓn��
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    m_pImmediateContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    ConstantBuffer* cb = (ConstantBuffer*)mappedResource.pData;
    cb->x_offset = x;    // X�����I�t�Z�b�g
    cb->y_offset = y;    // Y�����I�t�Z�b�g
    cb->scale = 1.0f;    // �v���C���[�̔{����1.0�ŌŒ�
    m_pImmediateContext->Unmap(m_pConstantBuffer, 0);

    // �v���C���[�̒��_�o�b�t�@��ݒ�
    UINT stride = sizeof(SimpleVertex); // ���_�T�C�Y
    UINT offset = 0;                    // �I�t�Z�b�g
    m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

    // �e�p�[�c��`��

    // 1. ���̐����` (���_�C���f�b�N�X0����4���_)
    m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    m_pImmediateContext->Draw(4, 0); // 4���_�A�J�n�C���f�b�N�X0

    // 2. �����̐��O�p�` (���_�C���f�b�N�X4����3���_)
    m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_pImmediateContext->Draw(3, 4); // 3���_�A�J�n�C���f�b�N�X4

    // 3. �E�̐��܊p�` (���_�C���f�b�N�X7����9���_ = 3�̎O�p�`)
    m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_pImmediateContext->Draw(9, 7); // 9���_�A�J�n�C���f�b�N�X7
}

/**
 * @brief ���ׂĂ̗L���Ȓe��`�悵�܂��B
 * @param game �Q�[����ԃI�u�W�F�N�g�̎Q��
 * @details �e�e�̌`��ɉ����ēK�؂Ȓ��_�o�b�t�@�ƕ`����@��I��
 */
void Graphics::RenderBullets(const Game& game)
{
    UINT stride = sizeof(SimpleVertex);
    UINT offset = 0;

    // �Q�[������e�̔z����擾
    const Bullet* bullets = game.getBullets();

    // ���ׂĂ̒e���`�F�b�N���āA�A�N�e�B�u�Ȃ��̂̂ݕ`��
    for (int i = 0; i < game.getMaxBullets(); ++i)
    {
        if (bullets[i].IsActive())
        {
            // �萔�o�b�t�@�ɒe�̍��W����������
            D3D11_MAPPED_SUBRESOURCE mappedResource;
            m_pImmediateContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
            ConstantBuffer* cb = (ConstantBuffer*)mappedResource.pData;
            cb->x_offset = bullets[i].GetX(); // �e��X���W
            cb->y_offset = bullets[i].GetY(); // �e��Y���W
            cb->scale = 1.0f;                 // �e�̔{����1.0�ŌŒ�
            m_pImmediateContext->Unmap(m_pConstantBuffer, 0);

            // �e�̌`��ɉ����āA�g�p���钸�_�o�b�t�@�ƕ`����@��؂�ւ���
            switch (bullets[i].GetShape())
            {
            case BulletShape::Square:
                // �l�p�`�̒e��`��
                m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pSquareBulletVertexBuffer, &stride, &offset);
                m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
                m_pImmediateContext->Draw(4, 0); // 4���_�Ŏl�p�`��`��
                break;

            case BulletShape::Triangle:
                // �O�p�`�̒e��`��
                m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pTriangleBulletVertexBuffer, &stride, &offset);
                m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
                m_pImmediateContext->Draw(3, 0); // 3���_�ŎO�p�`��`��
                break;

            case BulletShape::Pentagon:
                // �܊p�`�̒e��`��
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
 * @param game �Q�[����ԃI�u�W�F�N�g�̎Q��
 * @details ��Q���͒e�Ɠ����`����g�p���AOBSTACLE_SCALE�Ŋg�傳���
 */
void Graphics::RenderObstacles(const Game& game)
{
    UINT stride = sizeof(SimpleVertex);
    UINT offset = 0;

    // �Q�[�������Q���̔z����擾
    const Obstacle* obstacles = game.getObstacles();

    // ���ׂĂ̏�Q�����`�F�b�N���āA�A�N�e�B�u�Ȃ��̂̂ݕ`��
    for (int i = 0; i < game.getMaxObstacles(); ++i)
    {
        if (obstacles[i].IsActive())
        {
            // �萔�o�b�t�@�ɏ�Q���̍��W�ƃX�P�[������������
            D3D11_MAPPED_SUBRESOURCE mappedResource;
            m_pImmediateContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
            ConstantBuffer* cb = (ConstantBuffer*)mappedResource.pData;
            cb->x_offset = obstacles[i].GetX(); // ��Q����X���W
            cb->y_offset = obstacles[i].GetY(); // ��Q����Y���W
            cb->scale = OBSTACLE_SCALE;         // GameConfig.h����ǂݍ��񂾔{����ݒ�
            m_pImmediateContext->Unmap(m_pConstantBuffer, 0);

            // ��Q���̌`��ɉ����āA�g�p���钸�_�o�b�t�@�ƕ`����@��؂�ւ���
            // ����: ��Q���͒e�Ɠ������_�o�b�t�@���ė��p
            switch (obstacles[i].GetShape())
            {
            case BulletShape::Square:
                // �l�p�`�̏�Q����`��
                m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pSquareBulletVertexBuffer, &stride, &offset);
                m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
                m_pImmediateContext->Draw(4, 0);
                break;

            case BulletShape::Triangle:
                // �O�p�`�̏�Q����`��
                m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pTriangleBulletVertexBuffer, &stride, &offset);
                m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
                m_pImmediateContext->Draw(3, 0);
                break;

            case BulletShape::Pentagon:
                // �܊p�`�̏�Q����`��
                m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pPentagonBulletVertexBuffer, &stride, &offset);
                m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
                m_pImmediateContext->Draw(9, 0);
                break;
            }
        }
    }
}