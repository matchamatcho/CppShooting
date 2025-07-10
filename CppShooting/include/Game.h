#pragma once
#include "Bullet.h"
#include "Obstacle.h"
#include <windows.h> // GetAsyncKeyState �̂��߂ɕK�v

// �O���錾
class Bullet;
class Obstacle;

class Game {
public:
    Game();

    void Update(); // �Q�[���̏�Ԃ��X�V

    // �`��p�̃I�u�W�F�N�g���ւ̃A�N�Z�X���
    float getPlayerX() const { return m_playerX; }
    float getPlayerY() const { return m_playerY; }
    const Bullet* getBullets() const { return m_bullets; }
    const Obstacle* getObstacles() const { return m_obstacles; }
    static const int getMaxBullets() { return MAX_BULLETS; }
    static const int getMaxObstacles() { return MAX_OBSTACLES; }


private:
    // �Q�[���I�u�W�F�N�g
    float m_playerX;
    float m_playerY;

    // �e�̊Ǘ�
    static const int MAX_BULLETS = 50;
    Bullet m_bullets[MAX_BULLETS];
    float m_fireCooldown;

    // ��Q���̊Ǘ�
    static const int MAX_OBSTACLES = 10;
    Obstacle m_obstacles[MAX_OBSTACLES];
    float m_obstacleSpawnTimer;

    // �X�V�����̃w���p�[�֐�
    void UpdatePlayer();
    void UpdateBullets();
    void UpdateObstacles();
    void CheckCollisions();
};