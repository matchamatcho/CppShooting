#include "Obstacle.h"

// �R���X�g���N�^
Obstacle::Obstacle() :
    m_isActive(false),
    m_x(0.0f),
    m_y(0.0f),
    m_hp(0)
{
}

// ��Q����L��������
void Obstacle::Activate(float x, float y, int hp)
{
    m_isActive = true;
    m_x = x;
    m_y = y;
    m_hp = hp;
}

// ��Q���𖳌�������
void Obstacle::Deactivate()
{
    m_isActive = false;
}

// �e�������������̏���
void Obstacle::Hit()
{
    if (m_isActive)
    {
        m_hp--; // HP��1���炷
        if (m_hp <= 0)
        {
            Deactivate(); // HP��0�ȉ��ɂȂ����疳����
        }
    }
}