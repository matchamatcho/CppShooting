#pragma once
#include "Bullet.h" // BulletShapeを共有するためインクルード
#include "ObstacleBullet.h"


/**
 * @class Obstacle
 * @brief ゲーム内に登場する障害物を管理するクラス
 */
class Obstacle
{
public:
    /**
     * @brief コンストラクタ
     */
    Obstacle();

    /**
     * @brief 障害物を有効化し、指定した座標とHPで初期化します。
     * @param x 出現させるX座標
     * @param y 出現させるY座標
     * @param hp 初期HP
     * @param shape 障害物の形状
     */
    void Activate(float x, float y, int hp, BulletShape shape);

    /**
     * @brief 障害物を無効化します。
     */
    void Deactivate();

    /**
     * @brief 障害物が有効かどうかを返します。
     * @return true 有効な場合
     * @return false 無効な場合
     */
    bool IsActive() const { return m_isActive; }

    /**
     * @brief 弾が当たった際の処理を行います。HPを減少させ、0以下になったら無効化します。
     */
    void Hit(int damage);

    /**
     * @brief 障害物の現在のX座標を返します。
     * @return X座標
     */
    float GetX() const { return m_x; }

    /**
     * @brief 障害物の現在のY座標を返します。
     * @return Y座標
     */
    float GetY() const { return m_y; }

    /**
     * @brief 障害物の形状を返します。
     * @return 障害物の形状
     */
    BulletShape GetShape() const { return m_shape; }

	void Update(ObstacleBullet* obstacleBullets);
    void HandleShooting(ObstacleBullet* obstacleBullets, int maxObstacleBullets);

private:
    bool        m_isActive; // 障害物が有効かどうかのフラグ
    float       m_x;        // X座標
    float       m_y;        // Y座標
    int         m_hp;       // ヒットポイント（耐久値）
    BulletShape m_shape;    // 障害物の形状
    float m_obstacleFireCooldown;       // 弾の発射クールダウンタイマー
};