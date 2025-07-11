#pragma once
#include "Bullet.h" // �e�𔭎˂��邽�߂�Bullet�N���X�̒�`���K�v
#include <windows.h> // GetAsyncKeyState �̂��߂ɕK�v

/**
 * @class Player
 * @brief �v���C���[�L�����N�^�[�̊Ǘ��N���X
 * �v���C���[�̈ړ��A�e�̔��ˁA��ԊǗ���S�����܂��B
 */
class Player
{
public:
    /**
     * @brief �R���X�g���N�^
     */
    Player();

    /**
     * @brief �v���C���[�̏�Ԃ𖈃t���[���X�V���܂��B
     * �ړ����͂Ɣ��˓��͂��������܂��B
     * @param bullets �Q�[�����ɑ��݂���e�̔z��ւ̃|�C���^
     * @param maxBullets �e�̍ő吔
     */
    void Update(Bullet* bullets, int maxBullets);

    /**
     * @brief �v���C���[�̌��݂�X���W��Ԃ��܂��B
     * @return X���W
     */
    float GetX() const { return m_x; }

    /**
     * @brief �v���C���[�̌��݂�Y���W��Ԃ��܂��B
     * @return Y���W
     */
    float GetY() const { return m_y; }

private:
    /**
     * @brief �L�[���͂Ɋ�Â��ăv���C���[���ړ������܂��B
     */
    void HandleMovement();

    /**
     * @brief �L�[���͂Ɋ�Â��ăI�[�g�t�@�C�A�̏�Ԃ�؂�ւ��܂��B
     */
    void HandleFireToggle();

    /**
     * @brief �I�[�g�t�@�C�A���L���ȏꍇ�ɒe�𔭎˂��܂��B
     * @param bullets �Q�[�����ɑ��݂���e�̔z��ւ̃|�C���^
     * @param maxBullets �e�̍ő吔
     */
    void HandleShooting(Bullet* bullets, int maxBullets);

private:
    float m_x;                  // �v���C���[��X���W
    float m_y;                  // �v���C���[��Y���W
    float m_fireCooldown;       // �e�̔��˃N�[���_�E���^�C�}�[
    bool  m_isAutoFireEnabled;  // �I�[�g�t�@�C�A���L�����ǂ����̃t���O
    bool  m_wasSpaceKeyPressed; // �O�̃t���[���ŃX�y�[�X�L�[��������Ă�����
};