#pragma once
#include <windows.h>
#include <d3d11.h>
#include "Game.h" // Gameクラスの情報を描画に利用するためインクルード

// クラスの前方宣言 (循環参照を避けるため)
class Game;

/**
 * @class Graphics
 * @brief DirectX 11 を使用した描画処理を統括するクラス
 */
class Graphics {
public:
    /**
     * @brief コンストラクタ
     */
    Graphics();

    /**
     * @brief デストラクタ
     */
    ~Graphics();

    /**
     * @brief DirectX 11 の初期化処理を行います。
     * @param hWnd 描画対象のウィンドウハンドル
     * @return HRESULT 処理の成否
     */
    HRESULT Initialize(HWND hWnd);

    /**
     * @brief DirectX 11 のリソースを解放します。
     */
    void Shutdown();

    /**
     * @brief 1フレーム分の描画処理を行います。
     * @param game ゲームの状態が格納されたGameオブジェクト
     */
    void RenderFrame(const Game& game);

private:
    // --- DirectX 11 のコアインターフェース ---
    IDXGISwapChain* m_pSwapChain;           // スワップチェーン
    ID3D11Device* m_pd3dDevice;           // Direct3Dデバイス
    ID3D11DeviceContext* m_pImmediateContext;    // デバイスコンテキスト
    ID3D11RenderTargetView* m_pRenderTargetView;    // レンダーターゲットビュー

    // --- シェーダーオブジェクト ---
    ID3D11VertexShader* m_pVertexShader;        // 頂点シェーダー
    ID3D11PixelShader* m_pPixelShader;         // ピクセルシェーダー
    ID3D11InputLayout* m_pVertexLayout;        // 入力レイアウト

    // --- バッファオブジェクト ---
    ID3D11Buffer* m_pVertexBuffer;        // プレイヤー用の頂点バッファ
    ID3D11Buffer* m_pConstantBuffer;      // 定数バッファ (オブジェクトの位置などをシェーダーに渡す)
    ID3D11Buffer* m_pBulletVertexBuffer;  // 弾用の頂点バッファ
    ID3D11Buffer* m_pObstacleVertexBuffer;// 障害物用の頂点バッファ

private:
    // --- 描画処理のヘルパー関数 ---
    void RenderPlayer(float x, float y);
    void RenderBullets(const Game& game);
    void RenderObstacles(const Game& game);
};