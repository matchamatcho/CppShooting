#include "Obstacle.h"

/**
 * @brief Obstacleクラスのコンストラクタ
 * メンバ変数を初期化します。障害物は最初は非アクティブ状態です。
 */
Obstacle::Obstacle() :
    m_isActive(false),
    m_x(0.0f),
    m_y(0.0f),
    m_hp(0)
{
}

/**
 * @brief 障害物を有効化します。
 * @param x 出現位置のX座標
 * @param y 出現位置のY座標
 * @param hp 初期HP
 */
void Obstacle::Activate(float x, float y, int hp)
{
    m_isActive = true; // 障害物をアクティブ状態にする
    m_x = x;           // X座標を設定
    m_y = y;           // Y座標を設定
    m_hp = hp;         // HPを設定
}

/**
 * @brief 障害物を無効化します。
 */
void Obstacle::Deactivate()
{
    m_isActive = false; // 障害物を非アクティブ状態にする
}

/**
 * @brief 弾が当たった時の処理です。
 */
void Obstacle::Hit()
{
    // 障害物がアクティブな場合のみ処理を行う
    if (m_isActive)
    {
        m_hp--; // HPを1減らす
        if (m_hp <= 0)
        {
            Deactivate(); // HPが0以下になったら無効化
        }
    }
}