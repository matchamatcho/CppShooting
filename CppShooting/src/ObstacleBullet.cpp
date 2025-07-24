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
	m_shape(BulletShape::Square) // �f�t�H���g�`����l�p��
{
}
/**
 * @brief �e��L�����i���ˁj���܂��B
 * @param x ���ˈʒu��X���W
 * @param y ���ˈʒu��Y���W
 * @param shape �e�̌`��
 */
void ObstacleBullet::Activate(float x, float y, BulletShape shape)
{
	m_isActive = true; // �e���A�N�e�B�u��Ԃɂ���
	m_x = x;           // X���W��ݒ�
	m_y = y;           // Y���W��ݒ�
	m_shape = shape;   // �`���ݒ�
}
/**
 * @brief �e�𖳌������܂��B
 */
void ObstacleBullet::Deactivate()
{
	m_isActive = false; // �e���A�N�e�B�u��Ԃɂ���
}
/**
 * @brief �e�̏�Ԃ𖈃t���[���X�V���܂��B
 */
void ObstacleBullet::Update()
{
	// �e���A�N�e�B�u�ȏꍇ�̂ݏ������s��
	if (m_isActive)
	{
		// Y�����Ɉړ�������
		m_y += m_velocityY;
		// �e����ʂ̉��[�iY���W��-1.0��菬�����j�𒴂������A�N�e�B�u�ɂ���
		if (m_y < -1.0f)
		{
			Deactivate();
		}
	}
}




