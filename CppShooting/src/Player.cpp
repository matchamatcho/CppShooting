#include "Player.h"
#include "GameConfig.h" // PLAYER_MOVE_SPEED��L�[�ݒ�𗘗p���邽��

/**
 * @brief Player�N���X�̃R���X�g���N�^
 */
Player::Player() :
    m_x(0.0f),
    m_y(-0.8f),
    m_fireCooldown(0.0f),
    m_isAutoFireEnabled(false),
    m_wasSpaceKeyPressed(false),
    m_leftShape(BulletShape::Square),
    m_centerShape(BulletShape::Triangle),
    m_rightShape(BulletShape::Pentagon),
    m_wasZKeyPressed(false),
    m_wasXKeyPressed(false),
    m_wasCKeyPressed(false)
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

    // �ˌ��̃g�O������
    HandleFireToggle();

    // �`��ύX�̏���
    HandleShapeChange();

    // �ˌ�����
    HandleShooting(bullets, maxBullets);
}

/**
 * @brief �L�[���͂Ɋ�Â��ăv���C���[���ړ������܂��B
 */
void Player::HandleMovement()
{
    // GameConfig.h�Őݒ肳�ꂽ�L�[�Ńv���C���[���ړ�
    if (GetAsyncKeyState(KEY_MOVE_UP) & 0x8000) m_y += PLAYER_MOVE_SPEED;
    if (GetAsyncKeyState(KEY_MOVE_DOWN) & 0x8000) m_y -= PLAYER_MOVE_SPEED;
    if (GetAsyncKeyState(KEY_MOVE_LEFT) & 0x8000) m_x -= PLAYER_MOVE_SPEED;
    if (GetAsyncKeyState(KEY_MOVE_RIGHT) & 0x8000) m_x += PLAYER_MOVE_SPEED;

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
    if (IsKeyPressedOnce(KEY_TOGGLE_AUTOFIRE, m_wasSpaceKeyPressed))
    {
        m_isAutoFireEnabled = !m_isAutoFireEnabled; // bool�l�𔽓]������
    }
}

/**
 * @brief �L�[���͂Ɋ�Â��Đ�Ԃ̌`���ύX���܂��B
 */
void Player::HandleShapeChange()
{
    if (IsKeyPressedOnce(KEY_CHANGE_SHAPE_LEFT, m_wasZKeyPressed))
    {
        m_leftShape = GetNextShape(m_leftShape);
    }
    if (IsKeyPressedOnce(KEY_CHANGE_SHAPE_CENTER, m_wasXKeyPressed))
    {
        m_centerShape = GetNextShape(m_centerShape);
    }
    if (IsKeyPressedOnce(KEY_CHANGE_SHAPE_RIGHT, m_wasCKeyPressed))
    {
        m_rightShape = GetNextShape(m_rightShape);
    }
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
        BulletShape shapesToFire[] = { m_leftShape, m_centerShape, m_rightShape };
        float firePosX[] = { -0.1f, 0.0f, 0.1f }; // ���A�����A�E�̒e��X�ʒu
        float firePosY[] = { 0.06f, 0.1f, 0.06f }; // ���A�����A�E�̒e��Y�ʒu
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

/**
 * @brief �w�肳�ꂽ�L�[�������ꂽ�u�Ԃ��ǂ����𔻒肵�܂��B
 * @param vKey ���肷�鉼�z�L�[�R�[�h
 * @param wasKeyPressedLastFrame �O�̃t���[���ł��̃L�[��������Ă������ǂ����̏�Ԃ�ێ�����ϐ��ւ̎Q��
 * @return bool �����ꂽ�u�Ԃł����true
 */
bool Player::IsKeyPressedOnce(int vKey, bool& wasKeyPressedLastFrame)
{
    bool isKeyDown = (GetAsyncKeyState(vKey) & 0x8000);
    bool keyPressedOnce = isKeyDown && !wasKeyPressedLastFrame;
    wasKeyPressedLastFrame = isKeyDown;
    return keyPressedOnce;
}

/**
 * @brief ���̒e�̌`���Ԃ��܂��B(�����` -> �O�p�` -> �܊p�` -> �����`)
 * @param currentShape ���݂̌`��
 * @return BulletShape ���̌`��
*/
BulletShape Player::GetNextShape(BulletShape currentShape)
{
    switch (currentShape)
    {
    case BulletShape::Square:
        return BulletShape::Triangle;
    case BulletShape::Triangle:
        return BulletShape::Pentagon;
    case BulletShape::Pentagon:
        return BulletShape::Square;
    default:
        return BulletShape::Square; // �s���Ȍ`��̏ꍇ�̓f�t�H���g�ɖ߂�
    }
}