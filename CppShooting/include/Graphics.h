#pragma once
#include <windows.h>
#include <d3d11.h>
#include "Bullet.h"
#include "Obstacle.h" // Obstacleクラスのヘッダーをインクルード

// 前方宣言
class Bullet;
class Obstacle;

class Graphics {
public:
    Graphics();
    ~Graphics();

    // 初期化とシャットダウン
    HRESULT Initialize(HWND hWnd);
    void Shutdown();

    // フレームの更新とレンダリング
    void Update();
    void RenderFrame();

private:
    // DirectXインターフェース
    IDXGISwapChain* m_pSwapChain;
    ID3D11Device* m_pd3dDevice;
    ID3D11DeviceContext* m_pImmediateContext;
    ID3D11RenderTargetView* m_pRenderTargetView;
    ID3D11VertexShader* m_pVertexShader;
    ID3D11PixelShader* m_pPixelShader;
    ID3D11InputLayout* m_pVertexLayout;
    ID3D11Buffer* m_pVertexBuffer;
    ID3D11Buffer* m_pConstantBuffer;
    ID3D11Buffer* m_pBulletVertexBuffer;
    ID3D11Buffer* m_pObstacleVertexBuffer; // 障害物用の頂点バッファ

    // ゲームオブジェクト
    float m_triangleX;
    float m_triangleY;

    // 弾の管理
    static const int MAX_BULLETS = 50;
    Bullet m_bullets[MAX_BULLETS];
    float m_fireCooldown;

    // 障害物の管理
    static const int MAX_OBSTACLES = 10;
    Obstacle m_obstacles[MAX_OBSTACLES];
    float m_obstacleSpawnTimer;

private:
    // 弾の更新処理
    void UpdateBullets();
    // 弾の描画処理
    void RenderBullets();

    // 障害物の更新処理
    void UpdateObstacles();
    // 障害物の描画処理
    void RenderObstacles();
};