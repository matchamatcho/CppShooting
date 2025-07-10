#include "Game.h"
#include <time.h>
#include <stdlib.h> // rand, srand

Game::Game() :
    m_playerX(0.0f),
    m_playerY(0.0f),
    m_fireCooldown(0.0f),
    m_obstacleSpawnTimer(0.0f)
{
    srand((unsigned int)time(NULL)); // �����̏�����
}

void Game::Update()
{
    UpdatePlayer();
    UpdateBullets();
    UpdateObstacles();
    CheckCollisions();
}

void Game::UpdatePlayer()
{
    // �v���C���[�̈ړ�����
    float moveSpeed = 0.02f;
    if (GetAsyncKeyState('W') & 0x8000) m_playerY += moveSpeed;
    if (GetAsyncKeyState('S') & 0x8000) m_playerY -= moveSpeed;
    if (GetAsyncKeyState('A') & 0x8000) m_playerX -= moveSpeed;
    if (GetAsyncKeyState('D') & 0x8000) m_playerX += moveSpeed;

    // �v���C���[����ʊO�ɏo�Ȃ��悤�ɐ���
    if (m_playerX > 0.95f) m_playerX = 0.95f;
    if (m_playerX < -0.95f) m_playerX = -0.95f;
    if (m_playerY > 0.95f) m_playerY = 0.95f;
    if (m_playerY < -0.95f) m_playerY = -0.95f;
}

void Game::UpdateBullets()
{
    // �e�̔��˃N�[���_�E���^�C�}�[�����炷
    m_fireCooldown -= 0.1f;

    // �N�[���_�E�����I��������V�����e�𔭎˂���
    if (m_fireCooldown <= 0.0f)
    {
        // ���̔��˂܂ł̎��Ԃ��Đݒ�
        m_fireCooldown = 1.0f;

        // ��A�N�e�B�u�Ȓe��T���Ĕ��˂���
        for (int i = 0; i < MAX_BULLETS; ++i)
        {
            if (!m_bullets[i].IsActive())
            {
                // �O�p�`�̐�[���甭�˂���
                m_bullets[i].Activate(m_playerX, m_playerY + 0.05f);
                break; // 1�t���[����1����������
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

void Game::UpdateObstacles()
{
    // ��Q�������^�C�}�[�����炷
    m_obstacleSpawnTimer -= 0.05f;

    // �^�C�}�[��0�ɂȂ�����V������Q���𐶐�
    if (m_obstacleSpawnTimer <= 0.0f)
    {
        m_obstacleSpawnTimer = 10.0f; // ���̐����܂ł̎���

        for (int i = 0; i < MAX_OBSTACLES; ++i)
        {
            if (!m_obstacles[i].IsActive())
            {
                // X���W�������_���Ɍ��� (-0.9����0.9�͈̔�)
                float x = (rand() / (float)RAND_MAX) * 1.8f - 0.9f;
                // Y���W�͉�ʂ̏���ɌŒ�
                float y = 0.9f;
                // HP��3�ɐݒ�
                int hp = 3;
                m_obstacles[i].Activate(x, y, hp);
                break; // 1���������甲����
            }
        }
    }
    // �����ł͏�Q���̈ړ��͂����Ȃ����A�����ړ�������Ȃ炱���ɒǉ�����
}


void Game::CheckCollisions()
{
    // �e�Ə�Q���̓����蔻��
    for (int i = 0; i < MAX_BULLETS; ++i)
    {
        if (m_bullets[i].IsActive())
        {
            for (int j = 0; j < MAX_OBSTACLES; ++j)
            {
                if (m_obstacles[j].IsActive())
                {
                    float dx = m_bullets[i].GetX() - m_obstacles[j].GetX();
                    float dy = m_bullets[i].GetY() - m_obstacles[j].GetY();
                    float dist_squared = dx * dx + dy * dy;

                    // �ȒP�ȉ~�`�̓����蔻�� (0.05�͏�Q���̔��a�̖ڈ�)
                    if (dist_squared < 0.05f * 0.05f)
                    {
                        m_bullets[i].Deactivate(); // �e������
                        m_obstacles[j].Hit();      // ��Q����HP�����炷
                        break; // ���̒e�͂������̏�Q���ɂ͓�����Ȃ�
                    }
                }
            }
        }
    }
}