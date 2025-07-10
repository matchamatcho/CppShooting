#pragma once
#include <windows.h>
#include <d3d11.h>
#include "Game.h" // Gameクラスのヘッダーをインクルード

// 前方宣言
class Game;

class Graphics {
public:
    Graphics();
    ~Graphics();

    // 初期化とシャットダウン
    HRESULT Initialize(HWND hWnd);
    void Shutdown();

    // フレームのレンダリング
    void RenderFrame(const Game& game);

private:
    // DirectXインターフェース
    IDXGISwapChain* m_pSwapChain;
    ID3D11Device* m_pd3dDevice;
    ID3D11DeviceContext* m_pImmediateContext;
    ID3D11RenderTargetView* m_pRenderTargetView;
    ID3D11VertexShader* m_pVertexShader;
    ID3D11PixelShader* m_pPixelShader;
    ID3D11InputLayout* m_pVertexLayout;
    ID3D11Buffer* m_pVertexBuffer; // Player
    ID3D11Buffer* m_pConstantBuffer;
    ID3D11Buffer* m_pBulletVertexBuffer;
    ID3D11Buffer* m_pObstacleVertexBuffer; // 障害物用の頂点バッファ

private:
    // 描画処理のヘルパー関数
    void RenderPlayer(float x, float y);
    void RenderBullets(const Game& game);
    void RenderObstacles(const Game& game);
};