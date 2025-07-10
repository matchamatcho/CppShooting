#include "Bullet.h"
#include "GameConfig.h" // �Q�[���ݒ�t�@�C�����C���N���[�h

/**
 * @brief Bullet�N���X�̃R���X�g���N�^
 * �����o�ϐ������������܂��B�e�͍ŏ��͔�A�N�e�B�u��Ԃł��B
 */
Bullet::Bullet() :
    m_isActive(false),
    m_x(0.0f),
    m_y(0.0f),
    m_velocityY(BULLET_SPEED) // GameConfig.h �̒萔�ŏ�����
{
}

/**
 * @brief �e��L�����i���ˁj���܂��B
 * @param x ���ˈʒu��X���W
 * @param y ���ˈʒu��Y���W
 */
void Bullet::Activate(float x, float y)
{
    m_isActive = true; // �e���A�N�e�B�u��Ԃɂ���
    m_x = x;           // X���W��ݒ�
    m_y = y;           // Y���W��ݒ�
}

/**
 * @brief �e�𖳌������܂��B
 */
void Bullet::Deactivate()
{
    m_isActive = false; // �e���A�N�e�B�u��Ԃɂ���
}

/**
 * @brief �e�̏�Ԃ𖈃t���[���X�V���܂��B
 */
void Bullet::Update()
{
    // �e���A�N�e�B�u�ȏꍇ�̂ݏ������s��
    if (m_isActive)
    {
        // Y�����Ɉړ�������
        m_y += m_velocityY;

        // �e����ʂ̏�[�iY���W��1.0���傫���j�𒴂������A�N�e�B�u�ɂ���
        if (m_y > 1.0f)
        {
            Deactivate();
        }
    }
}