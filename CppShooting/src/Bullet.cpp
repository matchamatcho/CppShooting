#include "Bullet.h"
#include "GameConfig.h" // ゲーム設定ファイルをインクルード

/**
 * @brief Bulletクラスのコンストラクタ
 * メンバ変数を初期化します。弾は最初は非アクティブ状態です。
 */
Bullet::Bullet() :
    m_isActive(false),
    m_x(0.0f),
    m_y(0.0f),
    m_velocityY(BULLET_SPEED), // GameConfig.h の定数で初期化
    m_shape(BulletShape::Square) // デフォルト形状を四角に
{
}

/**
 * @brief 弾を有効化（発射）します。
 * @param x 発射位置のX座標
 * @param y 発射位置のY座標
 * @param shape 弾の形状
 */
void Bullet::Activate(float x, float y, BulletShape shape)
{
    m_isActive = true; // 弾をアクティブ状態にする
    m_x = x;           // X座標を設定
    m_y = y;           // Y座標を設定
    m_shape = shape;   // 形状を設定
}

/**
 * @brief 弾を無効化します。
 */
void Bullet::Deactivate()
{
    m_isActive = false; // 弾を非アクティブ状態にする
}

/**
 * @brief 弾の状態を毎フレーム更新します。
 */
void Bullet::Update()
{
    // 弾がアクティブな場合のみ処理を行う
    if (m_isActive)
    {
        // Y方向に移動させる
        m_y += m_velocityY;

        // 弾が画面の上端（Y座標が1.0より大きい）を超えたら非アクティブにする
        if (m_y > 1.0f)
        {
            Deactivate();
        }
    }
}