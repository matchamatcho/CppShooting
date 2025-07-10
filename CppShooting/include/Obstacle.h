#pragma once

// 障害物を管理するクラス
class Obstacle
{
public:
    // コンストラクタ
    Obstacle();

    // 障害物を有効化する
    void Activate(float x, float y, int hp);

    // 障害物を無効化する
    void Deactivate();

    // 障害物が有効かどうかを返す
    bool IsActive() const { return m_isActive; }

    // 障害物のHPが減る処理
    void Hit();

    // X座標を返す
    float GetX() const { return m_x; }
    // Y座標を返す
    float GetY() const { return m_y; }

private:
    bool  m_isActive;
    float m_x;
    float m_y;
    int   m_hp;
};