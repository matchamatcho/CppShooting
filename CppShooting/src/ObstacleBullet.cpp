#include "ObstacleBullet.h"
#include "GameConfig.h" // OBSTACLE_BULLET_SPEED���g�p���邽��
/**
 * @brief ObstacleBullet�N���X�̃R���X�g���N�^
 * �����o�ϐ������������܂��B�e�͍ŏ��͔�A�N�e�B�u��Ԃł��B
 */
ObstacleBullet::ObstacleBullet() :
	m_isActive(false),
	m_x(0.0f),
	m_y(0.0f),
	m_velocityY(OBSTACLE_BULLET_SPEED), // GameConfig.h �̒萔�ŏ�����
	m_shape(ObstacleShape::Square) // �f�t�H���g�`����l�p��
{
}



