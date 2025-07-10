#include "Graphics.h"
#include <d3dcompiler.h>
#include <time.h> // for srand

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

// シェーダーに渡すデータ構造体
struct ConstantBuffer
{
    float x_offset;
    float y_offset;
    float padding1;
    float padding2;
};

// 頂点データ構造体
struct SimpleVertex {
    float Pos[3];
    float Color[4];
};

// コンストラクタ
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
    m_pObstacleVertexBuffer(nullptr), // 障害物の頂点バッファを初期化
    m_triangleX(0.0f),
    m_triangleY(0.0f),
    m_fireCooldown(0.0f),
    m_obstacleSpawnTimer(0.0f) // 障害物生成タイマーを初期化
{
    srand((unsigned int)time(NULL)); // 乱数の初期化
}

// デストラクタ
Graphics::~Graphics() {
    Shutdown();
}

// 初期化処理
HRESULT Graphics::Initialize(HWND hWnd) {
    HRESULT hr = S_OK;

    // スワップチェーン、デバイス、レンダーターゲットビューの作成
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

    // ビューポートの設定
    D3D11_VIEWPORT vp;
    vp.Width = 800.0f;
    vp.Height = 600.0f;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    m_pImmediateContext->RSSetViewports(1, &vp);

    // シェーダーの作成
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

    // 入力レイアウトの作成
    D3D11_INPUT_ELEMENT_DESC layout[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    m_pd3dDevice->CreateInputLayout(layout, 2, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &m_pVertexLayout);
    pVSBlob->Release();
    pPSBlob->Release();

    // プレイヤー（三角形）の頂点バッファ作成
    {
        SimpleVertex vertices[] = {
            { { 0.0f, 0.05f, 0.5f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
            { { 0.05f, -0.05f, 0.5f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
            { { -0.05f, -0.05f, 0.5f }, { 0.0f, 0.0f, 1.0f, 1.0f } }
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

    // 弾（四角形）の頂点バッファ作成
    {
        SimpleVertex vertices[] = {
            { { -0.01f, -0.02f, 0.5f }, { 1.0f, 1.0f, 0.0f, 1.0f } },
            { { -0.01f,  0.02f, 0.5f }, { 1.0f, 1.0f, 0.0f, 1.0f } },
            { {  0.01f, -0.02f, 0.5f }, { 1.0f, 1.0f, 0.0f, 1.0f } },
            { {  0.01f,  0.02f, 0.5f }, { 1.0f, 1.0f, 0.0f, 1.0f } },
        };
        D3D11_BUFFER_DESC bd = {};
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(SimpleVertex) * 4;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        D3D11_SUBRESOURCE_DATA InitData = {};
        InitData.pSysMem = vertices;
        hr = m_pd3dDevice->CreateBuffer(&bd, &InitData, &m_pBulletVertexBuffer);
        if (FAILED(hr)) return hr;
    }

    // 障害物（四角形）の頂点バッファ作成
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
        D3D11_SUBRESOURCE_DATA InitData = {};
        InitData.pSysMem = vertices;
        hr = m_pd3dDevice->CreateBuffer(&bd, &InitData, &m_pObstacleVertexBuffer);
        if (FAILED(hr)) return hr;
    }

    // コンスタントバッファの作成
    D3D11_BUFFER_DESC cbd = {};
    cbd.Usage = D3D11_USAGE_DYNAMIC;
    cbd.ByteWidth = sizeof(ConstantBuffer);
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    hr = m_pd3dDevice->CreateBuffer(&cbd, nullptr, &m_pConstantBuffer);
    if (FAILED(hr)) return hr;

    return S_OK;
}

// クリーンアップ処理
void Graphics::Shutdown() {
    if (m_pImmediateContext) m_pImmediateContext->ClearState();

    if (m_pObstacleVertexBuffer) m_pObstacleVertexBuffer->Release();
    if (m_pBulletVertexBuffer) m_pBulletVertexBuffer->Release();
    if (m_pConstantBuffer) m_pConstantBuffer->Release();
    if (m_pVertexBuffer) m_pVertexBuffer->Release();
    if (m_pVertexLayout) m_pVertexLayout->Release();
    if (m_pVertexShader) m_pVertexShader->Release();
    if (m_pPixelShader) m_pPixelShader->Release();
    if (m_pRenderTargetView) m_pRenderTargetView->Release();
    if (m_pSwapChain) m_pSwapChain->Release();
    if (m_pImmediateContext) m_pImmediateContext->Release();
    if (m_pd3dDevice) m_pd3dDevice->Release();

    m_pSwapChain = nullptr;
    m_pd3dDevice = nullptr;
    m_pImmediateContext = nullptr;
    m_pRenderTargetView = nullptr;
    m_pVertexShader = nullptr;
    m_pPixelShader = nullptr;
    m_pVertexLayout = nullptr;
    m_pVertexBuffer = nullptr;
    m_pConstantBuffer = nullptr;
    m_pBulletVertexBuffer = nullptr;
    m_pObstacleVertexBuffer = nullptr;
}

// フレーム全体の更新処理
void Graphics::Update()
{
    // プレイヤーの移動処理
    float moveSpeed = 0.02f;
    if (GetAsyncKeyState('W') & 0x8000) m_triangleY += moveSpeed;
    if (GetAsyncKeyState('S') & 0x8000) m_triangleY -= moveSpeed;
    if (GetAsyncKeyState('A') & 0x8000) m_triangleX -= moveSpeed;
    if (GetAsyncKeyState('D') & 0x8000) m_triangleX += moveSpeed;

    // プレイヤーが画面外に出ないように制限
    if (m_triangleX > 0.95f) m_triangleX = 0.95f;
    if (m_triangleX < -0.95f) m_triangleX = -0.95f;
    if (m_triangleY > 0.95f) m_triangleY = 0.95f;
    if (m_triangleY < -0.95f) m_triangleY = -0.95f;

    // 弾の更新
    UpdateBullets();
    // 障害物の更新
    UpdateObstacles();
}

// 弾の更新処理（発射と移動、当たり判定）
void Graphics::UpdateBullets()
{
    // 弾の発射クールダウンタイマーを減らす
    m_fireCooldown -= 0.1f;

    // スペースキーが押され、かつクールダウンが終了したら新しい弾を発射する
    if (m_fireCooldown <= 0.0f)
    {
        // 次の発射までの時間を再設定
        m_fireCooldown = 1.0f;

        // 非アクティブな弾を探して発射する
        for (int i = 0; i < MAX_BULLETS; ++i)
        {
            if (!m_bullets[i].IsActive())
            {
                // 三角形の先端から発射する
                m_bullets[i].Activate(m_triangleX, m_triangleY + 0.05f);
                break; // 1フレームに1発だけ発射
            }
        }
    }

    // すべての弾の処理
    for (int i = 0; i < MAX_BULLETS; ++i)
    {
        if (m_bullets[i].IsActive())
        {
            m_bullets[i].Update();

            // 弾と障害物の当たり判定
            for (int j = 0; j < MAX_OBSTACLES; ++j)
            {
                if (m_obstacles[j].IsActive())
                {
                    float dx = m_bullets[i].GetX() - m_obstacles[j].GetX();
                    float dy = m_bullets[i].GetY() - m_obstacles[j].GetY();
                    float dist_squared = dx * dx + dy * dy;

                    // 簡単な円形の当たり判定 (0.05は障害物の半径の目安)
                    if (dist_squared < 0.05f * 0.05f)
                    {
                        m_bullets[i].Deactivate(); // 弾を消す
                        m_obstacles[j].Hit();      // 障害物のHPを減らす
                        break; // この弾はもう他の障害物には当たらない
                    }
                }
            }
        }
    }
}

// 障害物の更新処理
void Graphics::UpdateObstacles()
{
    // 障害物生成タイマーを減らす
    m_obstacleSpawnTimer -= 0.05f;

    // タイマーが0になったら新しい障害物を生成
    if (m_obstacleSpawnTimer <= 0.0f)
    {
        m_obstacleSpawnTimer = 10.0f; // 次の生成までの時間

        for (int i = 0; i < MAX_OBSTACLES; ++i)
        {
            if (!m_obstacles[i].IsActive())
            {
                // X座標をランダムに決定 (-0.9から0.9の範囲)
                float x = (rand() / (float)RAND_MAX) * 1.8f - 0.9f;
                // Y座標は画面の上方に固定
                float y = 0.9f;
                // HPを3に設定
                int hp = 3;
                m_obstacles[i].Activate(x, y, hp);
                break; // 1つ生成したら抜ける
            }
        }
    }
}

// フレーム全体のレンダリング処理
void Graphics::RenderFrame() {
    // 背景色で画面をクリア
    float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
    m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, ClearColor);

    // 描画設定
    m_pImmediateContext->IASetInputLayout(m_pVertexLayout);
    m_pImmediateContext->VSSetShader(m_pVertexShader, nullptr, 0);
    m_pImmediateContext->PSSetShader(m_pPixelShader, nullptr, 0);
    m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);

    // ----- プレイヤー（三角形）の描画 -----
    {
        D3D11_MAPPED_SUBRESOURCE mappedResource;
        m_pImmediateContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
        ConstantBuffer* cb = (ConstantBuffer*)mappedResource.pData;
        cb->x_offset = m_triangleX;
        cb->y_offset = m_triangleY;
        m_pImmediateContext->Unmap(m_pConstantBuffer, 0);

        UINT stride = sizeof(SimpleVertex);
        UINT offset = 0;
        m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
        m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        m_pImmediateContext->Draw(3, 0);
    }

    // ----- 弾の描画 -----
    RenderBullets();

    // ----- 障害物の描画 -----
    RenderObstacles();

    // 画面に表示
    m_pSwapChain->Present(1, 0);
}

// 弾を描画する
void Graphics::RenderBullets()
{
    UINT stride = sizeof(SimpleVertex);
    UINT offset = 0;
    m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pBulletVertexBuffer, &stride, &offset);
    m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    for (int i = 0; i < MAX_BULLETS; ++i)
    {
        if (m_bullets[i].IsActive())
        {
            D3D11_MAPPED_SUBRESOURCE mappedResource;
            m_pImmediateContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
            ConstantBuffer* cb = (ConstantBuffer*)mappedResource.pData;
            cb->x_offset = m_bullets[i].GetX();
            cb->y_offset = m_bullets[i].GetY();
            m_pImmediateContext->Unmap(m_pConstantBuffer, 0);

            m_pImmediateContext->Draw(4, 0);
        }
    }
}

// 障害物を描画する
void Graphics::RenderObstacles()
{
    UINT stride = sizeof(SimpleVertex);
    UINT offset = 0;
    m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pObstacleVertexBuffer, &stride, &offset);
    m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    for (int i = 0; i < MAX_OBSTACLES; ++i)
    {
        if (m_obstacles[i].IsActive())
        {
            D3D11_MAPPED_SUBRESOURCE mappedResource;
            m_pImmediateContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
            ConstantBuffer* cb = (ConstantBuffer*)mappedResource.pData;
            cb->x_offset = m_obstacles[i].GetX();
            cb->y_offset = m_obstacles[i].GetY();
            m_pImmediateContext->Unmap(m_pConstantBuffer, 0);

            m_pImmediateContext->Draw(4, 0);
        }
    }
}