#include "Obstacle.h"

/**
 * @brief Obstacle�N���X�̃R���X�g���N�^
 * �����o�ϐ������������܂��B��Q���͍ŏ��͔�A�N�e�B�u��Ԃł��B
 */
Obstacle::Obstacle() :
    m_isActive(false),
    m_x(0.0f),
    m_y(0.0f),
    m_hp(0)
{
}

/**
 * @brief ��Q����L�������܂��B
 * @param x �o���ʒu��X���W
 * @param y �o���ʒu��Y���W
 * @param hp ����HP
 */
void Obstacle::Activate(float x, float y, int hp)
{
    m_isActive = true; // ��Q�����A�N�e�B�u��Ԃɂ���
    m_x = x;           // X���W��ݒ�
    m_y = y;           // Y���W��ݒ�
    m_hp = hp;         // HP��ݒ�
}

/**
 * @brief ��Q���𖳌������܂��B
 */
void Obstacle::Deactivate()
{
    m_isActive = false; // ��Q�����A�N�e�B�u��Ԃɂ���
}

/**
 * @brief �e�������������̏����ł��B
 */
void Obstacle::Hit()
{
    // ��Q�����A�N�e�B�u�ȏꍇ�̂ݏ������s��
    if (m_isActive)
    {
        m_hp--; // HP��1���炷
        if (m_hp <= 0)
        {
            Deactivate(); // HP��0�ȉ��ɂȂ����疳����
        }
    }
}