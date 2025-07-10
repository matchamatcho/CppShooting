#include "Bullet.h"
#include "GameConfig.h" // 設定ファイルを追加

// コンストラクタ
Bullet::Bullet() :
    m_isActive(false),
    m_x(0.0f),
    m_y(0.0f),
    m_velocityY(BULLET_SPEED) // 定数を使用
{
}

// 弾を有効化（発射）する
void Bullet::Activate(float x, float y)
{
    m_isActive = true;
    m_x = x;
    m_y = y;
}

// 弾を無効化する
void Bullet::Deactivate()
{
    m_isActive = false;
}

// 弾の位置を更新する
void Bullet::Update()
{
    if (m_isActive)
    {
        // 弾をY方向に移動させる
        m_y += m_velocityY;

        // 弾が画面の上端を超えたら非アクティブにする
        if (m_y > 1.0f)
        {
            Deactivate();
        }
    }
}