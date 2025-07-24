#pragma once
#include "Bullet.h" // BulletShape�����L���邽�߃C���N���[�h



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
	bool m_isActive;          // �e���A�N�e�B�u���ǂ���
	float m_x;                // �e��X���W
	float m_y;                // �e��Y���W
	float m_velocityY;        // �e��Y�����̑��x
	BulletShape m_shape;    // �e�̌`��
};

