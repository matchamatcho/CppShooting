#pragma once

/**
 * @class Obstacle
 * @brief �Q�[�����ɓo�ꂷ���Q�����Ǘ�����N���X
 */
class Obstacle
{
public:
    /**
     * @brief �R���X�g���N�^
     */
    Obstacle();

    /**
     * @brief ��Q����L�������A�w�肵�����W��HP�ŏ��������܂��B
     * @param x �o��������X���W
     * @param y �o��������Y���W
     * @param hp ����HP
     */
    void Activate(float x, float y, int hp);

    /**
     * @brief ��Q���𖳌������܂��B
     */
    void Deactivate();

    /**
     * @brief ��Q�����L�����ǂ�����Ԃ��܂��B
     * @return true �L���ȏꍇ
     * @return false �����ȏꍇ
     */
    bool IsActive() const { return m_isActive; }

    /**
     * @brief �e�����������ۂ̏������s���܂��BHP�����������A0�ȉ��ɂȂ����疳�������܂��B
     */
    void Hit();

    /**
     * @brief ��Q���̌��݂�X���W��Ԃ��܂��B
     * @return X���W
     */
    float GetX() const { return m_x; }

    /**
     * @brief ��Q���̌��݂�Y���W��Ԃ��܂��B
     * @return Y���W
     */
    float GetY() const { return m_y; }

private:
    bool  m_isActive; // ��Q�����L�����ǂ����̃t���O
    float m_x;        // X���W
    float m_y;        // Y���W
    int   m_hp;       // �q�b�g�|�C���g�i�ϋv�l�j
};