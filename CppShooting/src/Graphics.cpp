#include "Graphics.h"
#include <d3dcompiler.h> // シェーダーコンパイル用
#include "GameConfig.h"  // ゲーム設定ファイルをインクルード

// 必要なライブラリをリンク
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

/**
 * @struct ConstantBuffer
 * @brief シェーダーに渡す定数バッファのデータ構造体
 */
struct ConstantBuffer
{
    float x_offset; // X方向のオフセット
    float y_offset; // Y方向のオフセット
    float scale;    // オブジェクトの拡大率
    float padding1; // 16バイトアライメントのためのパディング
};

/**
 * @struct SimpleVertex
 * @brief 頂点データを格納する構造体
 */
struct SimpleVertex {
    float Pos[3];   // 座標 (x, y, z)
    float Color[4]; // 色 (r, g, b, a)
};

/**
 * @brief Graphicsクラスのコンストラクタ
 * @details すべてのメンバポインタをnullptrで初期化
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
 * @brief Graphicsクラスのデストラクタ
 * @details Shutdown()を呼び出してリソースを解放
 */
Graphics::~Graphics() {
    Shutdown();
}

/**
 * @brief DirectXの初期化を行います。
 * @param hWnd ウィンドウハンドル
 * @return HRESULT 成功時はS_OK、失敗時はエラーコード
 * @details スワップチェーン、デバイス、シェーダー、頂点バッファなどを作成
 */
HRESULT Graphics::Initialize(HWND hWnd) {
    HRESULT hr = S_OK;

    // --- スワップチェーン、デバイス、レンダーターゲット等の初期化 ---
    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferCount = 1;                                    // バックバッファ数
    sd.BufferDesc.Width = 800;                             // ウィンドウ幅
    sd.BufferDesc.Height = 600;                            // ウィンドウ高さ
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // ピクセル形式
    sd.BufferDesc.RefreshRate.Numerator = 60;              // リフレッシュレート（分子）
    sd.BufferDesc.RefreshRate.Denominator = 1;             // リフレッシュレート（分母）
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;       // レンダーターゲットとして使用
    sd.OutputWindow = hWnd;                                 // 出力ウィンドウ
    sd.SampleDesc.Count = 1;                               // マルチサンプリング数
    sd.SampleDesc.Quality = 0;                             // マルチサンプリング品質
    sd.Windowed = TRUE;                                    // ウィンドウモード

    // DirectX 11.0の機能レベルを指定
    D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };

    // デバイスとスワップチェーンを同時に作成
    hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
        featureLevels, 1, D3D11_SDK_VERSION, &sd,
        &m_pSwapChain, &m_pd3dDevice, nullptr, &m_pImmediateContext);
    if (FAILED(hr)) {
        // DirectXデバイス作成に失敗
        return hr;
    }

    // バックバッファを取得してレンダーターゲットビューを作成
    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
    if (FAILED(hr)) {
        // バックバッファ取得に失敗
        return hr;
    }

    hr = m_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRenderTargetView);
    pBackBuffer->Release(); // バックバッファの参照カウントを減らす
    if (FAILED(hr)) {
        // レンダーターゲットビュー作成に失敗
        return hr;
    }

    // レンダーターゲットを設定
    m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, nullptr);

    // ビューポートを設定（描画領域の定義）
    D3D11_VIEWPORT vp;
    vp.Width = 800.0f;      // ビューポート幅
    vp.Height = 600.0f;     // ビューポート高さ
    vp.MinDepth = 0.0f;     // 最小深度値
    vp.MaxDepth = 1.0f;     // 最大深度値
    vp.TopLeftX = 0;        // 左上X座標
    vp.TopLeftY = 0;        // 左上Y座標
    m_pImmediateContext->RSSetViewports(1, &vp);

    // --- シェーダーの作成 ---
    ID3DBlob* pVSBlob = nullptr;

    // 頂点シェーダー: スケールと平行移動を適用
    const char* vsShaderCode =
        "cbuffer ConstantBuffer : register(b0) { float2 offset; float scale; float padding; };\n"
        "struct VS_INPUT { float4 Pos : POSITION; float4 Color : COLOR; };\n"
        "struct PS_INPUT { float4 Pos : SV_POSITION; float4 Color : COLOR; };\n"
        "PS_INPUT VS(VS_INPUT input) {\n"
        "    PS_INPUT output = (PS_INPUT)0;\n"
        "    output.Pos = input.Pos;\n"
        "    output.Pos.xy *= scale;\n"    // 拡大・縮小を適用
        "    output.Pos.xy += offset;\n"   // 平行移動を適用
        "    output.Color = input.Color; return output;\n"
        "}\n";

    // 頂点シェーダーをコンパイル
    D3DCompile(vsShaderCode, strlen(vsShaderCode), nullptr, nullptr, nullptr, "VS", "vs_4_0", 0, 0, &pVSBlob, nullptr);
    m_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &m_pVertexShader);

    ID3DBlob* pPSBlob = nullptr;

    // ピクセルシェーダー: 頂点カラーをそのまま出力
    const char* psShaderCode =
        "struct PS_INPUT { float4 Pos : SV_POSITION; float4 Color : COLOR; };\n"
        "float4 PS(PS_INPUT input) : SV_Target { return input.Color; }\n";

    // ピクセルシェーダーをコンパイル
    D3DCompile(psShaderCode, strlen(psShaderCode), nullptr, nullptr, nullptr, "PS", "ps_4_0", 0, 0, &pPSBlob, nullptr);
    m_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &m_pPixelShader);

    // --- 入力レイアウトの作成 ---
    // 頂点データの構造を定義
    D3D11_INPUT_ELEMENT_DESC layout[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },     // 座標データ
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },   // カラーデータ
    };
    m_pd3dDevice->CreateInputLayout(layout, 2, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &m_pVertexLayout);

    // シェーダーブロブを解放
    pVSBlob->Release();
    pPSBlob->Release();

    // --- 弾と障害物、プレイヤーの各パーツの頂点バッファを作成 ---

    // 1. 四角形 (青色)
    {
        SimpleVertex vertices[] = {
            { { -0.04f, -0.04f, 0.5f }, { 0.2f, 0.2f, 0.8f, 1.0f } }, // 左下
            { { -0.04f,  0.04f, 0.5f }, { 0.2f, 0.2f, 0.8f, 1.0f } }, // 左上
            { {  0.04f, -0.04f, 0.5f }, { 0.2f, 0.2f, 0.8f, 1.0f } }, // 右下
            { {  0.04f,  0.04f, 0.5f }, { 0.2f, 0.2f, 0.8f, 1.0f } }  // 右上
        };
        D3D11_BUFFER_DESC bd = {};
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(vertices);
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        D3D11_SUBRESOURCE_DATA InitData = { vertices };
        hr = m_pd3dDevice->CreateBuffer(&bd, &InitData, &m_pSquareVertexBuffer);
        if (FAILED(hr)) return hr;
    }

    // 2. 三角形 (赤色)
    {
        SimpleVertex vertices[] = {
            { { -0.04f, -0.04f, 0.5f }, { 0.8f, 0.2f, 0.2f, 1.0f } }, // 左下
            { {  0.0f,   0.08f, 0.5f }, { 0.8f, 0.2f, 0.2f, 1.0f } }, // 上
            { {  0.04f, -0.04f, 0.5f }, { 0.8f, 0.2f, 0.2f, 1.0f } }  // 右下
        };
        D3D11_BUFFER_DESC bd = {};
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(vertices);
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        D3D11_SUBRESOURCE_DATA InitData = { vertices };
        hr = m_pd3dDevice->CreateBuffer(&bd, &InitData, &m_pTriangleVertexBuffer);
        if (FAILED(hr)) return hr;
    }

    // 3. 五角形 (緑色)
    {
        SimpleVertex pentagonVerts[] = {
            { { 0.0f,    -0.08f, 0.5f }, { 0.2f, 0.8f, 0.2f, 1.0f } }, // 下
            { {-0.076f, -0.025f, 0.5f }, { 0.2f, 0.8f, 0.2f, 1.0f } }, // 左下
            { {-0.047f,  0.065f, 0.5f }, { 0.2f, 0.8f, 0.2f, 1.0f } }, // 左上
            { { 0.047f,  0.065f, 0.5f }, { 0.2f, 0.8f, 0.2f, 1.0f } }, // 右上
            { { 0.076f, -0.025f, 0.5f }, { 0.2f, 0.8f, 0.2f, 1.0f } }, // 右下
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

    // --- 障害物用の頂点バッファは、弾のバッファを再利用するため、ここでは作成しない ---

    // --- 定数バッファの作成 ---
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
 * @brief DirectXリソースをクリーンアップします。
 * @details すべてのCOMインターフェースを安全に解放
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
 * @brief 1フレーム分の描画処理を行います。
 * @param game ゲーム状態オブジェクトの参照
 * @details 画面クリア、各オブジェクトの描画、画面表示を実行
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
 * @brief プレイヤーを描画します。
 * @param x プレイヤーのX座標
 * @param y プレイヤーのY座標
 * @param leftShape 左の戦車の形状
 * @param centerShape 真ん中の戦車の形状
 * @param rightShape 右の戦車の形状
 */
void Graphics::RenderPlayer(float x, float y, BulletShape leftShape, BulletShape centerShape, BulletShape rightShape)
{
    // 各パーツの位置オフセット
    float leftX = x - 0.1f;
    float leftY = y + 0.06f;

    float centerX = x;
    float centerY = y + 0.1f;

    float rightX = x + 0.1f;
    float rightY = y + 0.06f;

    // 各パーツを描画
    DrawShape(leftShape, leftX, leftY, 1.0f);
    DrawShape(centerShape, centerX, centerY, 1.0f);
    DrawShape(rightShape, rightX, rightY, 1.0f);
}

/**
 * @brief すべての有効な弾を描画します。
 * @param game ゲーム状態オブジェクトの参照
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
 * @brief すべての有効な障害物を描画します。
 * @param game ゲーム状態オブジェクトの参照
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
 * @brief 指定された形状を描画します。
 * @param shape 描画する形状
 * @param xOffset X座標のオフセット
 * @param yOffset Y座標のオフセット
 * @param scale 拡大率
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