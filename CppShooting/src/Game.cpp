#include "Game.h"
#include "GameConfig.h" 
#include <time.h>       
#include <stdlib.h>     

/**
 * @brief Game�N���X�̃R���X�g���N�^
 * �����o�ϐ��̏������Ɨ����V�[�h�̐ݒ���s���܂��B
 */
Game::Game() :
    m_player(), // Player�I�u�W�F�N�g��������
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
    // Player�N���X��Update���Ăяo���A�v���C���[�̑���ƒe�̔��˂��s��
    m_player.Update(m_bullets, MAX_BULLETS);

    UpdateBullets();   // �e�̈ړ�������
    UpdateObstacles(); // ��Q���̏o��������
    CheckCollisions(); // �Փ˔��������
}

/**
 * @brief �e�̏�Ԃ��X�V���܂��B
 * ���˃��W�b�N��Player�N���X�Ɉڏ����A�����ł͒e�̈ړ��݂̂�S�����܂��B
 */
void Game::UpdateBullets()
{
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
                float x = (rand() / (float)RAND_MAX) * 1.54f - 0.77f;
                // Y���W���I�u�W�F�N�g�̑傫�����l�������l�ɏC�� (�C���ӏ�)
                float y = 0.75f;

                // �`��������_���Ɍ���
                int shapeType = rand() % 3; // 0, 1, 2�̂����ꂩ�̒l
                BulletShape shape;
                switch (shapeType) {
                case 0:
                    shape = BulletShape::Square;
                    break;
                case 1:
                    shape = BulletShape::Triangle;
                    break;
                case 2:
                    shape = BulletShape::Pentagon;
                    break;
                }

                // HP�̏����l��萔�Őݒ肵�A�`����n��
                m_obstacles[i].Activate(x, y, OBSTACLE_DEFAULT_HP, shape);
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