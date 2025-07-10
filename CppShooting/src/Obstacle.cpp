#include "Obstacle.h"

// コンストラクタ
Obstacle::Obstacle() :
    m_isActive(false),
    m_x(0.0f),
    m_y(0.0f),
    m_hp(0)
{
}

// 障害物を有効化する
void Obstacle::Activate(float x, float y, int hp)
{
    m_isActive = true;
    m_x = x;
    m_y = y;
    m_hp = hp;
}

// 障害物を無効化する
void Obstacle::Deactivate()
{
    m_isActive = false;
}

// 弾が当たった時の処理
void Obstacle::Hit()
{
    if (m_isActive)
    {
        m_hp--; // HPを1減らす
        if (m_hp <= 0)
        {
            Deactivate(); // HPが0以下になったら無効化
        }
    }
}