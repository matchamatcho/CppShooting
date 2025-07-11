#include "Player.h"
#include "GameConfig.h" // PLAYER_MOVE_SPEED�Ȃǂ̐ݒ�l�𗘗p���邽��

/**
 * @brief Player�N���X�̃R���X�g���N�^
 */
Player::Player() :
    m_x(0.0f),
    m_y(-0.8f),
    m_fireCooldown(0.0f),
    m_isAutoFireEnabled(false),
    m_wasSpaceKeyPressed(false)
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

    // �ˌ��̃g�O�������ƁA���ۂ̎ˌ������𕪗�
    HandleFireToggle();
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

    // �v���C���[����ʊO�ɏo�Ȃ��悤�ɍ��W�𐧌�
    if (m_x > 0.85f) m_x = 0.85f;
    if (m_x < -0.85f) m_x = -0.85f;
    if (m_y > 0.9f) m_y = 0.9f;
    if (m_y < -0.9f) m_y = -0.9f;
}

/**
 * @brief �L�[���͂Ɋ�Â��ăI�[�g�t�@�C�A�̏�Ԃ�؂�ւ��܂��B
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
 * @brief �I�[�g�t�@�C�A���L���ȏꍇ�ɒe�𔭎˂��܂��B
 */
void Player::HandleShooting(Bullet* bullets, int maxBullets)
{
    // �I�[�g�t�@�C�A���L���A���N�[���_�E�����I�����Ă�����e�𔭎�
    if (m_isAutoFireEnabled && m_fireCooldown <= 0.0f)
    {
        m_fireCooldown = PLAYER_FIRE_COOLDOWN; // �N�[���_�E�������Z�b�g

        // ���˂���e�̌`��ƁA���ꂼ��̔��ˈʒu�I�t�Z�b�g���`
        BulletShape shapesToFire[] = { BulletShape::Square, BulletShape::Triangle, BulletShape::Pentagon };
        float firePosX[] = { -0.1f, 0.0f, 0.1f }; // �l�p�A�O�p�A�܊p�̒e��X�ʒu
        float firePosY[] = { 0.06f, 0.1f, 0.06f }; // �l�p�A�O�p�A�܊p�̒e��Y�ʒu
        int numBulletsToFire = 3;

        int bulletsFired = 0;
        // ��A�N�e�B�u�Ȓe��T���Ĕ��˂���
        for (int i = 0; i < maxBullets && bulletsFired < numBulletsToFire; ++i)
        {
            if (!bullets[i].IsActive())
            {
                // �`��ƈʒu���w�肵�Ēe��L����
                bullets[i].Activate(
                    m_x + firePosX[bulletsFired],
                    m_y + firePosY[bulletsFired],
                    shapesToFire[bulletsFired]
                );
                bulletsFired++;
            }
        }
    }
}