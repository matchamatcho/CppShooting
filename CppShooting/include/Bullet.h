#pragma once

/**
 * @class Bullet
 * @brief プレイヤーが発射する弾を管理するクラス
 */
class Bullet
{
public:
    /**
     * @brief コンストラクタ
     */
    Bullet();

    /**
     * @brief 弾を有効化し、指定した座標から発射します。
     * @param x 発射するX座標
     * @param y 発射するY座標
     */
    void Activate(float x, float y);

    /**
     * @brief 弾を無効化します。（画面外に出た、敵に当たったなど）
     */
    void Deactivate();

    /**
     * @brief 弾の位置を更新します。
     */
    void Update();

    /**
     * @brief 弾が現在有効（画面内に存在している）かどうかを返します。
     * @return true 有効な場合
     * @return false 無効な場合
     */
    bool IsActive() const { return m_isActive; }

    /**
     * @brief 弾の現在のX座標を返します。
     * @return X座標
     */
    float GetX() const { return m_x; }

    /**
     * @brief 弾の現在のY座標を返します。
     * @return Y座標
     */
    float GetY() const { return m_y; }

private:
    bool  m_isActive;    // 弾が有効かどうかのフラグ
    float m_x;           // X座標
    float m_y;           // Y座標
    float m_velocityY;   // Y方向の移動速度
};