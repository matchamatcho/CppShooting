#include "Player.h"
#include "GameConfig.h" // PLAYER_MOVE_SPEED�Ȃǂ̐ݒ�l�𗘗p���邽��

/**
 * @brief Player�N���X�̃R���X�g���N�^
 */
Player::Player() :
    m_x(0.0f),
    m_y(-0.8f),
    m_fireCooldown(0.0f),
    m_isAutoFireEnabled(false), // ���ǉ�: ������Ԃ̓I�[�g�t�@�C�AOFF
    m_wasSpaceKeyPressed(false) // ���ǉ�: ������Ԃ̓L�[��������Ă��Ȃ�
{
}

/**
 * @brief �v���C���[�̏�Ԃ𖈃t���[���X�V���܂��B
 */
void Player::Update(Bullet* bullets, int maxBullets)
{
    // �N�[���_�E���^�C�}�[���X�V
    if (m_fireCooldown > 0.0f) {
        m_fireCooldown -= 0.1f;
    }

    // �ړ�����
    HandleMovement();

    // ���ύX: �ˌ��̃g�O�������ƁA���ۂ̎ˌ������𕪗�
    HandleFireToggle();
    HandleShooting(bullets, maxBullets);
}

/**
 * @brief �L�[���͂Ɋ�Â��ăv���C���[���ړ������܂��B(�ύX�Ȃ�)
 */
void Player::HandleMovement()
{
    // 'W', 'S', 'A', 'D' �L�[�Ńv���C���[���ړ�
    if (GetAsyncKeyState('W') & 0x8000) m_y += PLAYER_MOVE_SPEED;
    if (GetAsyncKeyState('S') & 0x8000) m_y -= PLAYER_MOVE_SPEED;
    if (GetAsyncKeyState('A') & 0x8000) m_x -= PLAYER_MOVE_SPEED;
    if (GetAsyncKeyState('D') & 0x8000) m_x += PLAYER_MOVE_SPEED;

    // �v���C���[����ʊO�ɏo�Ȃ��悤�ɍ��W�𐧌�
    if (m_x > 0.85f) m_x = 0.85f;
    if (m_x < -0.85f) m_x = -0.85f;
    if (m_y > 0.9f) m_y = 0.9f;
    if (m_y < -0.9f) m_y = -0.9f;
}

/**
 * @brief ��������������ǉ�������
 * �L�[���͂Ɋ�Â��ăI�[�g�t�@�C�A�̏�Ԃ�؂�ւ��܂��B
 */
void Player::HandleFireToggle()
{
    // ���݂̃X�y�[�X�L�[�̏�Ԃ��擾
    bool isSpaceKeyDown = (GetAsyncKeyState(VK_SPACE) & 0x8000);

    // �u���O�̃t���[���ł͉�����Ă��炸�A���݂̃t���[���ŉ�����Ă���v�ꍇ�A�g�O����؂�ւ���
    if (isSpaceKeyDown && !m_wasSpaceKeyPressed) {
        m_isAutoFireEnabled = !m_isAutoFireEnabled; // bool�l�𔽓]������
    }

    // ���݂̃L�[�̏�Ԃ��u���O�̏�ԁv�Ƃ��ĕۑ�����
    m_wasSpaceKeyPressed = isSpaceKeyDown;
}


/**
 * @brief ��������������ύX������
 * �I�[�g�t�@�C�A���L���ȏꍇ�ɒe�𔭎˂��܂��B
 */
void Player::HandleShooting(Bullet* bullets, int maxBullets)
{
    // �I�[�g�t�@�C�A���L���A���N�[���_�E�����I�����Ă�����e�𔭎�
    if (m_isAutoFireEnabled && m_fireCooldown <= 0.0f)
    {
        m_fireCooldown = PLAYER_FIRE_COOLDOWN; // �N�[���_�E�������Z�b�g

        // ���ˈʒu���` (�v���C���[�̒��S����̑��΍��W)
        const float firePosX[] = { -0.1f, 0.0f,  0.1f };
        const float firePosY[] = { 0.05f, 0.09f, 0.09f };

        int bulletsFired = 0;

        // ��A�N�e�B�u�Ȓe��T����3�������ɔ��˂���
        for (int i = 0; i < maxBullets; ++i)
        {
            if (!bullets[i].IsActive())
            {
                bullets[i].Activate(m_x + firePosX[bulletsFired], m_y + firePosY[bulletsFired]);

                bulletsFired++;
                if (bulletsFired >= 3) {
                    break;
                }
            }
        }
    }
}