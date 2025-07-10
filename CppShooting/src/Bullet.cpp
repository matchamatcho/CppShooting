#include "Bullet.h"

// コンストラクタ
Bullet::Bullet() :
    m_isActive(false),   // 最初は非アクティブ
    m_x(0.0f),
    m_y(0.0f),
    m_velocityY(0.05f)   // 弾の移動速度
{
}

// 弾を有効化（発射）する
// 引数:
//   x: 発射するX座標
//   y: 発射するY座標
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
    // 弾が有効な場合のみ処理を行う
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