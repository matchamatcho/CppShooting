#include "Bullet.h"

// �R���X�g���N�^
Bullet::Bullet() :
    m_isActive(false),   // �ŏ��͔�A�N�e�B�u
    m_x(0.0f),
    m_y(0.0f),
    m_velocityY(0.05f)   // �e�̈ړ����x
{
}

// �e��L�����i���ˁj����
// ����:
//   x: ���˂���X���W
//   y: ���˂���Y���W
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
    // �e���L���ȏꍇ�̂ݏ������s��
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