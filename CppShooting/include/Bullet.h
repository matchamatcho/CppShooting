#pragma once

// 弾を管理するクラス
class Bullet
{
public:
    // コンストラクタ
    Bullet();

    // 弾を有効化（発射）する
    void Activate(float x, float y);

    // 弾を無効化する
    void Deactivate();

    // 弾の位置を更新する
    void Update();

    // 弾が有効かどうかを返す
    bool IsActive() const { return m_isActive; }
    // X座標を返す
    float GetX() const { return m_x; }
    // Y座標を返す
    float GetY() const { return m_y; }

private:
    bool  m_isActive;  // 弾が画面内に存在し、有効かどうかのフラグ
    float m_x;         // X座標
    float m_y;         // Y座標
    float m_velocityY; // Y方向の速度
};