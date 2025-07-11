#pragma once
#include "Bullet.h"
#include "Obstacle.h"
#include "Player.h"   // Player�N���X���C���N���[�h

// �N���X�̑O���錾
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
    const Player& getPlayer() const { return m_player; }
    const Bullet* getBullets() const { return m_bullets; }
    const Obstacle* getObstacles() const { return m_obstacles; }
    static const int getMaxBullets() { return MAX_BULLETS; }
    static const int getMaxObstacles() { return MAX_OBSTACLES; }


private:
    // --- �Q�[���I�u�W�F�N�g ---
    Player m_player; // �v���C���[�I�u�W�F�N�g

    // --- �e�̊Ǘ� ---
    static const int MAX_BULLETS = 50;
    Bullet m_bullets[MAX_BULLETS];

    // --- ��Q���̊Ǘ� ---
    static const int MAX_OBSTACLES = 10;
    Obstacle m_obstacles[MAX_OBSTACLES];
    float m_obstacleSpawnTimer;

    // --- �X�V�����̃w���p�[�֐� ---
    void UpdateBullets();   // �e�̏�Ԃ��X�V
    void UpdateObstacles(); // ��Q���̏�Ԃ��X�V
    void CheckCollisions(); // �e�Ə�Q���̓����蔻����`�F�b�N
};