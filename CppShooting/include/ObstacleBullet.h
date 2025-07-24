#pragma once
#include "Bullet.h" // BulletShapeを共有するためインクルード



class ObstacleBullet
{
public:
	ObstacleBullet();

	void Activate(float x, float y, BulletShape shape);
	void Deactivate();
	void Update();
	bool IsActive() const { return m_isActive; }
	float GetX() const { return m_x; }
	float GetY() const { return m_y; }
	BulletShape GetShape() const { return m_shape; }
	void Hit();
private:
	bool m_isActive;          // 弾がアクティブかどうか
	float m_x;                // 弾のX座標
	float m_y;                // 弾のY座標
	float m_velocityY;        // 弾のY方向の速度
	BulletShape m_shape;    // 弾の形状
};

