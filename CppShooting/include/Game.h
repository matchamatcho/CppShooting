#pragma once
#include "Bullet.h"
#include "Obstacle.h"
#include <windows.h> // GetAsyncKeyState �̂��߂ɕK�v

// �N���X�̑O���錾 (�z�Q�Ƃ�����邽��)
class Bullet;
class Obstacle;

/**
 * @class Game
 * @brief �Q�[���S�̂̃��W�b�N���Ǘ�����N���X
 */
class Game {
public:
    /**
     * @brief �R���X�g���N�^
     */
    Game();

    /**
     * @brief �Q�[���̏�Ԃ𖈃t���[���X�V���܂��B
     */
    void Update();

    // --- �`��p�̃I�u�W�F�N�g���ւ̃A�N�Z�X��񋟂���Q�b�^�[�֐� ---
    float getPlayerX() const { return m_playerX; }
    float getPlayerY() const { return m_playerY; }
    const Bullet* getBullets() const { return m_bullets; }
    const Obstacle* getObstacles() const { return m_obstacles; }
    static const int getMaxBullets() { return MAX_BULLETS; }
    static const int getMaxObstacles() { return MAX_OBSTACLES; }


private:
    // --- �Q�[���I�u�W�F�N�g ---
    float m_playerX; // �v���C���[��X���W
    float m_playerY; // �v���C���[��Y���W

    // --- �e�̊Ǘ� ---
    static const int MAX_BULLETS = 50;  // ��ʏ�ɑ��݂ł���e�̍ő吔
    Bullet m_bullets[MAX_BULLETS];      // �e�̔z��
    float m_fireCooldown;               // �e�̔��ˊԊu�𐧌䂷��^�C�}�[

    // --- ��Q���̊Ǘ� ---
    static const int MAX_OBSTACLES = 10; // ��ʏ�ɑ��݂ł����Q���̍ő吔
    Obstacle m_obstacles[MAX_OBSTACLES]; // ��Q���̔z��
    float m_obstacleSpawnTimer;          // ��Q���̏o���Ԋu�𐧌䂷��^�C�}�[

    // --- �X�V�����̃w���p�[�֐� ---
    void UpdatePlayer();    // �v���C���[�̏�Ԃ��X�V
    void UpdateBullets();   // �e�̏�Ԃ��X�V
    void UpdateObstacles(); // ��Q���̏�Ԃ��X�V
    void CheckCollisions(); // �e�Ə�Q���̓����蔻����`�F�b�N
};