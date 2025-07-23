#pragma once

enum class ObstacleShape
{
	Square,   // �l�p�`
	Triangle, // �O�p�`
	Pentagon  // �܊p�`
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
	bool m_isActive;          // �e���A�N�e�B�u���ǂ���
	float m_x;                // �e��X���W
	float m_y;                // �e��Y���W
	float m_velocityY;        // �e��Y�����̑��x
	ObstacleShape m_shape;    // �e�̌`��
};

