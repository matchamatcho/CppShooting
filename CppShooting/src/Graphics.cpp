#include "Graphics.h"
#include <d3dcompiler.h> // シェーダーコンパイル用
#include "GameConfig.h"  // ゲーム設定ファイルをインクルード

// 必要なライブラリをリンク
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

/**
 * @struct ConstantBuffer
 * @brief シェーダーに渡す定数バッファのデータ構造体
 * オブジェクトのオフセット（移動量）を格納します。
 */
struct ConstantBuffer
{
    float x_offset; // X方向のオフセット
    float y_offset; // Y方向のオフセット
    float padding1; // 16バイトアライメントのためのパディング
    float padding2; // 16バイトアライメントのためのパディング
};

/**
 * @struct SimpleVertex
 * @brief 頂点データを格納する構造体
 * 3D空間上の位置(Pos)と色(Color)を持ちます。
 */
struct SimpleVertex {
    float Pos[3];   // 座標 (x, y, z)
    float Color[4]; // 色 (r, g, b, a)
};

/**
 * @brief Graphicsクラスのコンストラクタ
 * すべてのメンバ変数を nullptr で初期化します。
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
 * @brief Graphicsクラスのデストラクタ
 * Shutdown() を呼び出してリソースを解放します。
 */
Graphics::~Graphics() {
    Shutdown();
}

/**
 * @brief DirectXの初期化を行います。
 * デバイス、スワップチェーン、レンダーターゲット、シェーダー、頂点バッファなどを生成します。
 * @param hWnd 描画対象のウィンドウハンドル
 * @return HRESULT 処理の成否
 */
HRESULT Graphics::Initialize(HWND hWnd) {
    HRESULT hr = S_OK;

    // --- スワップチェーンのデスクリプタ（設定情報） ---
    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferCount = 1;
    sd.BufferDesc.Width = 800; // ウィンドウ幅
    sd.BufferDesc.Height = 600; // ウィンドウ高さ
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 色形式
    sd.BufferDesc.RefreshRate.Numerator = 60; // リフレッシュレート
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE; // ウィンドウモード

    // --- デバイスとスワップチェーンの作成 ---
    D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };
    hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
        featureLevels, 1, D3D11_SDK_VERSION, &sd,
        &m_pSwapChain, &m_pd3dDevice, nullptr, &m_pImmediateContext);
    if (FAILED(hr)) return hr;

    // --- レンダーターゲットビューの作成 ---
    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
    if (FAILED(hr)) return hr;
    hr = m_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRenderTargetView);
    pBackBuffer->Release();
    if (FAILED(hr)) return hr;

    // --- レンダーターゲットの設定 ---
    m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, nullptr);

    // --- ビューポートの設定 ---
    D3D11_VIEWPORT vp;
    vp.Width = 800.0f;
    vp.Height = 600.0f;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    m_pImmediateContext->RSSetViewports(1, &vp);

    // --- 頂点シェーダーの作成 ---
    ID3DBlob* pVSBlob = nullptr;
    const char* vsShaderCode = // HLSLシェーダーコード
        "cbuffer ConstantBuffer : register(b0) {\n"
        "    float2 offset;\n"
        "};\n"
        "struct VS_INPUT { float4 Pos : POSITION; float4 Color : COLOR; };\n"
        "struct PS_INPUT { float4 Pos : SV_POSITION; float4 Color : COLOR; };\n"
        "PS_INPUT VS(VS_INPUT input) {\n"
        "    PS_INPUT output = (PS_INPUT)0;\n"
        "    output.Pos = input.Pos;\n"
        "    output.Pos.xy += offset; \n" // 頂点位置をオフセットでずらす
        "    output.Color = input.Color;\n"
        "    return output;\n"
        "}\n";
    D3DCompile(vsShaderCode, strlen(vsShaderCode), nullptr, nullptr, nullptr, "VS", "vs_4_0", 0, 0, &pVSBlob, nullptr);
    m_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &m_pVertexShader);

    // --- ピクセルシェーダーの作成 ---
    ID3DBlob* pPSBlob = nullptr;
    const char* psShaderCode = // HLSLシェーダーコード
        "struct PS_INPUT { float4 Pos : SV_POSITION; float4 Color : COLOR; };\n"
        "float4 PS(PS_INPUT input) : SV_Target {\n"
        "    return input.Color;\n" // 頂点から渡された色をそのまま出力
        "}\n";
    D3DCompile(psShaderCode, strlen(psShaderCode), nullptr, nullptr, nullptr, "PS", "ps_4_0", 0, 0, &pPSBlob, nullptr);
    m_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &m_pPixelShader);

    // --- 入力レイアウトの作成 (頂点データの構造をD3Dに教える) ---
    D3D11_INPUT_ELEMENT_DESC layout[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    m_pd3dDevice->CreateInputLayout(layout, 2, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &m_pVertexLayout);
    pVSBlob->Release(); // シェーダーBLOBは不要になったので解放
    pPSBlob->Release();

    // --- プレイヤーの頂点バッファ作成 ---
    {
        // プレイヤーの形状（三角形）を定義
        SimpleVertex vertices[] = {
            { { 0.0f, 0.05f, 0.5f }, { 1.0f, 0.0f, 0.0f, 1.0f } },   // 上の頂点 (赤)
            { { 0.05f, -0.05f, 0.5f }, { 0.0f, 1.0f, 0.0f, 1.0f } },  // 右下の頂点 (緑)
            { { -0.05f, -0.05f, 0.5f }, { 0.0f, 0.0f, 1.0f, 1.0f } } // 左下の頂点 (青)
        };
        D3D11_BUFFER_DESC bd = {};
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(SimpleVertex) * 3;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        D3D11_SUBRESOURCE_DATA InitData = { vertices };
        hr = m_pd3dDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer);
        if (FAILED(hr)) return hr;
    }

    // --- 弾の頂点バッファ作成 ---
    {
        // 弾の形状（四角形）を定義 (GameConfig.hの定数を使用)
        SimpleVertex vertices[] = {
            { { -BULLET_HALF_WIDTH, -BULLET_HALF_HEIGHT, 0.5f }, { 1.0f, 1.0f, 0.0f, 1.0f } }, // 左下 (黄)
            { { -BULLET_HALF_WIDTH,  BULLET_HALF_HEIGHT, 0.5f }, { 1.0f, 1.0f, 0.0f, 1.0f } }, // 左上 (黄)
            { {  BULLET_HALF_WIDTH, -BULLET_HALF_HEIGHT, 0.5f }, { 1.0f, 1.0f, 0.0f, 1.0f } }, // 右下 (黄)
            { {  BULLET_HALF_WIDTH,  BULLET_HALF_HEIGHT, 0.5f }, { 1.0f, 1.0f, 0.0f, 1.0f } }, // 右上 (黄)
        };
        D3D11_BUFFER_DESC bd = {};
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(SimpleVertex) * 4;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        D3D11_SUBRESOURCE_DATA InitData = { vertices };
        hr = m_pd3dDevice->CreateBuffer(&bd, &InitData, &m_pBulletVertexBuffer);
        if (FAILED(hr)) return hr;
    }

    // --- 障害物の頂点バッファ作成 ---
    {
        // 障害物の形状（四角形）を定義
        SimpleVertex vertices[] = {
            { { -0.05f, -0.05f, 0.5f }, { 0.5f, 0.5f, 0.5f, 1.0f } }, // 左下 (灰色)
            { { -0.05f,  0.05f, 0.5f }, { 0.5f, 0.5f, 0.5f, 1.0f } }, // 左上 (灰色)
            { {  0.05f, -0.05f, 0.5f }, { 0.5f, 0.5f, 0.5f, 1.0f } }, // 右下 (灰色)
            { {  0.05f,  0.05f, 0.5f }, { 0.5f, 0.5f, 0.5f, 1.0f } }  // 右上 (灰色)
        };
        D3D11_BUFFER_DESC bd = {};
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(SimpleVertex) * 4;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        D3D11_SUBRESOURCE_DATA InitData = { vertices };
        hr = m_pd3dDevice->CreateBuffer(&bd, &InitData, &m_pObstacleVertexBuffer);
        if (FAILED(hr)) return hr;
    }

    // --- 定数バッファ作成 ---
    {
        D3D11_BUFFER_DESC cbd = {};
        cbd.Usage = D3D11_USAGE_DYNAMIC; // CPUから毎フレーム更新するため DYNAMIC に設定
        cbd.ByteWidth = sizeof(ConstantBuffer);
        cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // CPUからの書き込みアクセスを許可
        hr = m_pd3dDevice->CreateBuffer(&cbd, nullptr, &m_pConstantBuffer);
        if (FAILED(hr)) return hr;
    }

    return S_OK; // 初期化成功
}

/**
 * @brief DirectXリソースをクリーンアップ（解放）します。
 * Initializeで作成したのと逆の順序で解放していきます。
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
 * @brief 1フレーム分の描画処理を行います。
 * @param game 描画するオブジェクトの情報を持つGameオブジェクト
 */
void Graphics::RenderFrame(const Game& game) {
    // 背景色で画面をクリア
    float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // 暗い青色
    m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, ClearColor);

    // --- 共通の描画設定 ---
    m_pImmediateContext->IASetInputLayout(m_pVertexLayout);
    m_pImmediateContext->VSSetShader(m_pVertexShader, nullptr, 0);
    m_pImmediateContext->PSSetShader(m_pPixelShader, nullptr, 0);
    m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);

    // --- 各オブジェクトの描画 ---
    RenderPlayer(game.getPlayerX(), game.getPlayerY());
    RenderBullets(game);
    RenderObstacles(game);

    // --- バックバッファを画面に表示 ---
    m_pSwapChain->Present(1, 0);
}

/**
 * @brief プレイヤーを描画します。
 * @param x プレイヤーのX座標
 * @param y プレイヤーのY座標
 */
void Graphics::RenderPlayer(float x, float y)
{
    // 定数バッファを更新してプレイヤーの座標をシェーダーに渡す
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    m_pImmediateContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    ConstantBuffer* cb = (ConstantBuffer*)mappedResource.pData;
    cb->x_offset = x;
    cb->y_offset = y;
    m_pImmediateContext->Unmap(m_pConstantBuffer, 0);

    // プレイヤーの頂点バッファを設定
    UINT stride = sizeof(SimpleVertex);
    UINT offset = 0;
    m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
    // プリミティブトポロジーを三角形リストに設定
    m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    // 3頂点で描画
    m_pImmediateContext->Draw(3, 0);
}

/**
 * @brief すべての有効な弾を描画します。
 * @param game 弾の情報を持つGameオブジェクト
 */
void Graphics::RenderBullets(const Game& game)
{
    // 弾の頂点バッファとプリミティブトポロジーを設定
    UINT stride = sizeof(SimpleVertex);
    UINT offset = 0;
    m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pBulletVertexBuffer, &stride, &offset);
    m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); // 四角形なのでトライアングルストリップ

    const Bullet* bullets = game.getBullets();
    // すべての弾をチェック
    for (int i = 0; i < game.getMaxBullets(); ++i)
    {
        if (bullets[i].IsActive()) // 有効な弾のみ描画
        {
            // 定数バッファを更新して弾の座標をシェーダーに渡す
            D3D11_MAPPED_SUBRESOURCE mappedResource;
            m_pImmediateContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
            ConstantBuffer* cb = (ConstantBuffer*)mappedResource.pData;
            cb->x_offset = bullets[i].GetX();
            cb->y_offset = bullets[i].GetY();
            m_pImmediateContext->Unmap(m_pConstantBuffer, 0);

            // 4頂点で描画
            m_pImmediateContext->Draw(4, 0);
        }
    }
}

/**
 * @brief すべての有効な障害物を描画します。
 * @param game 障害物の情報を持つGameオブジェクト
 */
void Graphics::RenderObstacles(const Game& game)
{
    // 障害物の頂点バッファとプリミティブトポロジーを設定
    UINT stride = sizeof(SimpleVertex);
    UINT offset = 0;
    m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pObstacleVertexBuffer, &stride, &offset);
    m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); // 四角形なのでトライアングルストリップ

    const Obstacle* obstacles = game.getObstacles();
    // すべての障害物をチェック
    for (int i = 0; i < game.getMaxObstacles(); ++i)
    {
        if (obstacles[i].IsActive()) // 有効な障害物のみ描画
        {
            // 定数バッファを更新して障害物の座標をシェーダーに渡す
            D3D11_MAPPED_SUBRESOURCE mappedResource;
            m_pImmediateContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
            ConstantBuffer* cb = (ConstantBuffer*)mappedResource.pData;
            cb->x_offset = obstacles[i].GetX();
            cb->y_offset = obstacles[i].GetY();
            m_pImmediateContext->Unmap(m_pConstantBuffer, 0);

            // 4頂点で描画
            m_pImmediateContext->Draw(4, 0);
        }
    }
}