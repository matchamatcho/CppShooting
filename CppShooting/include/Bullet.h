#pragma once

/**
 * @class Bullet
 * @brief �v���C���[�����˂���e���Ǘ�����N���X
 */
class Bullet
{
public:
    /**
     * @brief �R���X�g���N�^
     */
    Bullet();

    /**
     * @brief �e��L�������A�w�肵�����W���甭�˂��܂��B
     * @param x ���˂���X���W
     * @param y ���˂���Y���W
     */
    void Activate(float x, float y);

    /**
     * @brief �e�𖳌������܂��B�i��ʊO�ɏo���A�G�ɓ��������Ȃǁj
     */
    void Deactivate();

    /**
     * @brief �e�̈ʒu���X�V���܂��B
     */
    void Update();

    /**
     * @brief �e�����ݗL���i��ʓ��ɑ��݂��Ă���j���ǂ�����Ԃ��܂��B
     * @return true �L���ȏꍇ
     * @return false �����ȏꍇ
     */
    bool IsActive() const { return m_isActive; }

    /**
     * @brief �e�̌��݂�X���W��Ԃ��܂��B
     * @return X���W
     */
    float GetX() const { return m_x; }

    /**
     * @brief �e�̌��݂�Y���W��Ԃ��܂��B
     * @return Y���W
     */
    float GetY() const { return m_y; }

private:
    bool  m_isActive;    // �e���L�����ǂ����̃t���O
    float m_x;           // X���W
    float m_y;           // Y���W
    float m_velocityY;   // Y�����̈ړ����x
};