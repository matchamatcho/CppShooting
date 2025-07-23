#pragma once

enum class ObstacleShape
{
	Square,   // 四角形
	Triangle, // 三角形
	Pentagon  // 五角形
};

class ObstacleBullet
{
public:
	ObstacleBullet();

	void Activate(float x, float y, ObstacleShape shape);
	void Deactivate();
	void Update();
	bool IsActive() const { return m_isActive; }
	float GetX() const { return m_x; }
	float GetY() const { return m_y; }
	ObstacleShape GetShape() const { return m_shape; }
private:
	bool m_isActive;          // 弾がアクティブかどうか
	float m_x;                // 弾のX座標
	float m_y;                // 弾のY座標
	float m_velocityY;        // 弾のY方向の速度
	ObstacleShape m_shape;    // 弾の形状
};

