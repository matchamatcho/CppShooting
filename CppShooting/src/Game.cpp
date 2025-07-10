#include "Game.h"
#include "GameConfig.h" // �ݒ�t�@�C����ǉ�
#include <time.h>
#include <stdlib.h> 

Game::Game() :
    m_playerX(0.0f),
    m_playerY(0.0f),
    m_fireCooldown(0.0f),
    m_obstacleSpawnTimer(0.0f)
{
    srand((unsigned int)time(NULL));
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
    // �萔���g�p
    if (GetAsyncKeyState('W') & 0x8000) m_playerY += PLAYER_MOVE_SPEED;
    if (GetAsyncKeyState('S') & 0x8000) m_playerY -= PLAYER_MOVE_SPEED;
    if (GetAsyncKeyState('A') & 0x8000) m_playerX -= PLAYER_MOVE_SPEED;
    if (GetAsyncKeyState('D') & 0x8000) m_playerX += PLAYER_MOVE_SPEED;

    // �v���C���[����ʊO�ɏo�Ȃ��悤�ɐ���
    if (m_playerX > 0.95f) m_playerX = 0.95f;
    if (m_playerX < -0.95f) m_playerX = -0.95f;
    if (m_playerY > 0.95f) m_playerY = 0.95f;
    if (m_playerY < -0.95f) m_playerY = -0.95f;
}

void Game::UpdateBullets()
{
    m_fireCooldown -= 0.1f;

    if (m_fireCooldown <= 0.0f)
    {
        m_fireCooldown = PLAYER_FIRE_COOLDOWN; // �萔���g�p

        for (int i = 0; i < MAX_BULLETS; ++i)
        {
            if (!m_bullets[i].IsActive())
            {
                m_bullets[i].Activate(m_playerX, m_playerY + 0.05f);
                break;
            }
        }
    }

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
    m_obstacleSpawnTimer -= 0.05f;

    if (m_obstacleSpawnTimer <= 0.0f)
    {
        m_obstacleSpawnTimer = OBSTACLE_SPAWN_INTERVAL; // �萔���g�p

        for (int i = 0; i < MAX_OBSTACLES; ++i)
        {
            if (!m_obstacles[i].IsActive())
            {
                float x = (rand() / (float)RAND_MAX) * 1.8f - 0.9f;
                float y = 0.9f;
                // HP�̏����l��萔�Őݒ�
                m_obstacles[i].Activate(x, y, OBSTACLE_DEFAULT_HP);
                break;
            }
        }
    }
}


void Game::CheckCollisions()
{
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

                    // �����蔻��̔��a��萔�Őݒ�
                    if (dist_squared < OBSTACLE_COLLISION_RADIUS * OBSTACLE_COLLISION_RADIUS)
                    {
                        m_bullets[i].Deactivate();
                        m_obstacles[j].Hit();
                        break;
                    }
                }
            }
        }
    }
}