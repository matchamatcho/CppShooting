#include "Bullet.h"
#include "GameConfig.h" // �ݒ�t�@�C����ǉ�

// �R���X�g���N�^
Bullet::Bullet() :
    m_isActive(false),
    m_x(0.0f),
    m_y(0.0f),
    m_velocityY(BULLET_SPEED) // �萔���g�p
{
}

// �e��L�����i���ˁj����
void Bullet::Activate(float x, float y)
{
    m_isActive = true;
    m_x = x;
    m_y = y;
}

// �e�𖳌�������
void Bullet::Deactivate()
{
    m_isActive = false;
}

// �e�̈ʒu���X�V����
void Bullet::Update()
{
    if (m_isActive)
    {
        // �e��Y�����Ɉړ�������
        m_y += m_velocityY;

        // �e����ʂ̏�[�𒴂������A�N�e�B�u�ɂ���
        if (m_y > 1.0f)
        {
            Deactivate();
        }
    }
}