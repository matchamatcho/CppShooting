#pragma once
#include <windows.h>
#include <d3d11.h>
#include "Bullet.h" // Bulletクラスのヘッダーをインクルード

// 前方宣言
class Bullet;

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
    ID3D11Buffer* m_pVertexBuffer;     // 三角形用の頂点バッファ
    ID3D11Buffer* m_pConstantBuffer;   // シェーダーにデータを渡すためのバッファ

    // ▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼ 追加 ▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼
    ID3D11Buffer* m_pBulletVertexBuffer; // 弾用の頂点バッファ

    // ゲームオブジェクト
    float m_triangleX; // 三角形のX座標
    float m_triangleY; // 三角形のY座標

    // 弾の管理
    static const int MAX_BULLETS = 50; // 画面に表示できる弾の最大数
    Bullet m_bullets[MAX_BULLETS];     // 弾の配列
    float m_fireCooldown;              // 弾の発射間隔を制御するタイマー
    // ▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲ 追加 ▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲

private:
    // ▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼ 追加 ▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼
    // 弾の更新処理
    void UpdateBullets();
    // 弾の描画処理
    void RenderBullets();
    // ▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲ 追加 ▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲
};