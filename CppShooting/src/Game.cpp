#include "Game.h"
#include "GameConfig.h" // �Q�[���ݒ�t�@�C�����C���N���[�h
#include <time.h>       // �����̏����� (srand) �̂��߂ɕK�v
#include <stdlib.h>     // ���� (rand) �̂��߂ɕK�v

/**
 * @brief Game�N���X�̃R���X�g���N�^
 * �����o�ϐ��̏������Ɨ����V�[�h�̐ݒ���s���܂��B
 */
Game::Game() :
    m_playerX(0.0f),
    m_playerY(0.0f),
    m_fireCooldown(0.0f),
    m_obstacleSpawnTimer(0.0f)
{
    // �����̃V�[�h�����ݎ����ŏ�����
    srand((unsigned int)time(NULL));
}

/**
 * @brief �Q�[���S�̂̃��W�b�N�𖈃t���[���X�V���܂��B
 */
void Game::Update()
{
    UpdatePlayer();    // �v���C���[�̈ړ��Ȃǂ�����
    UpdateBullets();   // �e�̈ړ��Ɣ��˂�����
    UpdateObstacles(); // ��Q���̏o��������
    CheckCollisions(); // �Փ˔��������
}

/**
 * @brief �v���C���[�̏�Ԃ��X�V���܂��B
 * �L�[���͂ɉ������ړ��ƁA��ʊO�ւ̈ړ��������s���܂��B
 */
void Game::UpdatePlayer()
{
    // 'W', 'S', 'A', 'D' �L�[�Ńv���C���[���ړ� (GameConfig.h �̒萔���g�p)
    if (GetAsyncKeyState('W') & 0x8000) m_playerY += PLAYER_MOVE_SPEED;
    if (GetAsyncKeyState('S') & 0x8000) m_playerY -= PLAYER_MOVE_SPEED;
    if (GetAsyncKeyState('A') & 0x8000) m_playerX -= PLAYER_MOVE_SPEED;
    if (GetAsyncKeyState('D') & 0x8000) m_playerX += PLAYER_MOVE_SPEED;

    // �v���C���[����ʊO�ɏo�Ȃ��悤�ɍ��W�𐧌�
    if (m_playerX > 0.95f) m_playerX = 0.95f;
    if (m_playerX < -0.95f) m_playerX = -0.95f;
    if (m_playerY > 0.95f) m_playerY = 0.95f;
    if (m_playerY < -0.95f) m_playerY = -0.95f;
}

/**
 * @brief �e�̏�Ԃ��X�V���܂��B
 * �N�[���_�E���Ɋ�Â��ĐV�����e�𔭎˂��A�����̒e�̈ʒu���X�V���܂��B
 */
void Game::UpdateBullets()
{
    // ���˃N�[���_�E���^�C�}�[�����炷
    m_fireCooldown -= 0.1f;

    // �N�[���_�E�����I��������V�����e�𔭎�
    if (m_fireCooldown <= 0.0f)
    {
        m_fireCooldown = PLAYER_FIRE_COOLDOWN; // �N�[���_�E�������Z�b�g (GameConfig.h �̒萔���g�p)

        // ��A�N�e�B�u�Ȓe��T���čė��p����
        for (int i = 0; i < MAX_BULLETS; ++i)
        {
            if (!m_bullets[i].IsActive())
            {
                // �v���C���[�̏����O����e�𔭎�
                m_bullets[i].Activate(m_playerX, m_playerY + 0.05f);
                break; // 1���������烋�[�v�𔲂���
            }
        }
    }

    // ���ׂĂ̒e�̈ʒu���X�V
    for (int i = 0; i < MAX_BULLETS; ++i)
    {
        if (m_bullets[i].IsActive())
        {
            m_bullets[i].Update();
        }
    }
}

/**
 * @brief ��Q���̏�Ԃ��X�V���܂��B
 * ��莞�Ԃ��ƂɐV������Q���𐶐����܂��B
 */
void Game::UpdateObstacles()
{
    // ��Q���o���^�C�}�[�����炷
    m_obstacleSpawnTimer -= 0.05f;

    // �^�C�}�[��0�ɂȂ�����V������Q�����o��������
    if (m_obstacleSpawnTimer <= 0.0f)
    {
        m_obstacleSpawnTimer = OBSTACLE_SPAWN_INTERVAL; // �^�C�}�[�����Z�b�g (GameConfig.h �̒萔���g�p)

        // ��A�N�e�B�u�ȏ�Q����T���čė��p����
        for (int i = 0; i < MAX_OBSTACLES; ++i)
        {
            if (!m_obstacles[i].IsActive())
            {
                // X���W�������_���Ɍ��肵�A��ʏ㕔����o��������
                float x = (rand() / (float)RAND_MAX) * 1.8f - 0.9f;
                float y = 0.9f;
                // HP�̏����l��萔�Őݒ�
                m_obstacles[i].Activate(x, y, OBSTACLE_DEFAULT_HP);
                break; // 1�o���������烋�[�v�𔲂���
            }
        }
    }
}

/**
 * @brief �e�Ə�Q���̓����蔻����`�F�b�N���܂��B
 */
void Game::CheckCollisions()
{
    // ���ׂĂ̗L���Ȓe�ɑ΂��ă��[�v
    for (int i = 0; i < MAX_BULLETS; ++i)
    {
        if (m_bullets[i].IsActive())
        {
            // ���ׂĂ̗L���ȏ�Q���ɑ΂��ă��[�v
            for (int j = 0; j < MAX_OBSTACLES; ++j)
            {
                if (m_obstacles[j].IsActive())
                {
                    // �e�Ə�Q���̋������v�Z (�O�����̒藝)
                    float dx = m_bullets[i].GetX() - m_obstacles[j].GetX();
                    float dy = m_bullets[i].GetY() - m_obstacles[j].GetY();
                    float dist_squared = dx * dx + dy * dy; // �������̌v�Z���ȗ����邽�߁A������2��Ŕ�r

                    // �����蔻��̔��a (��2��) ��苗������������ΏՓ�
                    if (dist_squared < OBSTACLE_COLLISION_RADIUS * OBSTACLE_COLLISION_RADIUS)
                    {
                        m_bullets[i].Deactivate();  // �e������
                        m_obstacles[j].Hit();       // ��Q����HP�����炷
                        break; // ���̒e�͏������̂ŁA���̏�Q���Ƃ̔���͕s�v
                    }
                }
            }
        }
    }
}