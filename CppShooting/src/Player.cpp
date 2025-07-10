#include "Player.h"
#include "GameConfig.h" // PLAYER_MOVE_SPEED�Ȃǂ̐ݒ�l�𗘗p���邽��

/**
 * @brief Player�N���X�̃R���X�g���N�^
 */
Player::Player() :
    m_x(0.0f),
    m_y(-0.8f), // ��ʉ����������ʒu�ɐݒ�
    m_fireCooldown(0.0f)
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

    // �ˌ�����
    HandleShooting(bullets, maxBullets);
}

/**
 * @brief �L�[���͂Ɋ�Â��ăv���C���[���ړ������܂��B
 */
void Player::HandleMovement()
{
    // 'W', 'S', 'A', 'D' �L�[�Ńv���C���[���ړ�
    if (GetAsyncKeyState('W') & 0x8000) m_y += PLAYER_MOVE_SPEED;
    if (GetAsyncKeyState('S') & 0x8000) m_y -= PLAYER_MOVE_SPEED;
    if (GetAsyncKeyState('A') & 0x8000) m_x -= PLAYER_MOVE_SPEED;
    if (GetAsyncKeyState('D') & 0x8000) m_x += PLAYER_MOVE_SPEED;

    // �v���C���[����ʊO�ɏo�Ȃ��悤�ɍ��W�𐧌� (��Ԃ̕����l��)
    if (m_x > 0.85f) m_x = 0.85f;
    if (m_x < -0.85f) m_x = -0.85f;
    if (m_y > 0.9f) m_y = 0.9f;
    if (m_y < -0.9f) m_y = -0.9f;
}

/**
 * @brief �L�[���͂Ɋ�Â��Ēe�𔭎˂��܂��B
 */
void Player::HandleShooting(Bullet* bullets, int maxBullets)
{
    // �X�y�[�X�L�[��������A���N�[���_�E�����I�����Ă�����V�����e�𔭎�
    if ((GetAsyncKeyState(VK_SPACE) & 0x8000) && m_fireCooldown <= 0.0f)
    {
        m_fireCooldown = PLAYER_FIRE_COOLDOWN; // �N�[���_�E�������Z�b�g

        // ��������������ύX������
        // ���ˈʒu��V������Ԃ̌`��ɍ��킹��
        // ��(�����`)�A����(���O�p�`)�A�E(���܊p�`)�̐�[���甭��
        const float firePosX[] = { -0.1f, 0.0f,  0.1f };
        const float firePosY[] = { 0.05f, 0.09f, 0.09f };
        // �����������܂ŕύX������

        int bulletsFired = 0; // ���̃t���[���Ŕ��˂����e�̐�

        // ��A�N�e�B�u�Ȓe��T����3�������ɔ��˂���
        for (int i = 0; i < maxBullets; ++i)
        {
            if (!bullets[i].IsActive())
            {
                // �v���C���[�̈ʒu�ɃI�t�Z�b�g���������ꏊ����e�𔭎�
                bullets[i].Activate(m_x + firePosX[bulletsFired], m_y + firePosY[bulletsFired]);

                bulletsFired++;
                if (bulletsFired >= 3) {
                    break; // 3���������烋�[�v�𔲂���
                }
            }
        }
    }
}