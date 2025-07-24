#include "Obstacle.h"
#include "GameConfig.h" // OBSTACLE_FIRE_COOLDOWN���g�p���邽��

/**
 * @brief Obstacle�N���X�̃R���X�g���N�^
 * �����o�ϐ������������܂��B��Q���͍ŏ��͔�A�N�e�B�u��Ԃł��B
 */
Obstacle::Obstacle() :
    m_isActive(false),
    m_x(0.0f),
    m_y(0.0f),
    m_hp(0),
    m_shape(BulletShape::Square),// �f�t�H���g�`��
    m_obstacleFireCooldown(0.0f)
{
}

/**
 * @brief ��Q����L�������܂��B
 * @param x �o���ʒu��X���W
 * @param y �o���ʒu��Y���W
 * @param hp ����HP
 * @param shape ��Q���̌`��
 */
void Obstacle::Activate(float x, float y, int hp, BulletShape shape)
{
    m_isActive = true; // ��Q�����A�N�e�B�u��Ԃɂ���
    m_x = x;           // X���W��ݒ�
    m_y = y;           // Y���W��ݒ�
    m_hp = hp;         // HP��ݒ�
    m_shape = shape;   // �`���ݒ�
}

/**
 * @brief ��Q���𖳌������܂��B
 */
void Obstacle::Deactivate()
{
    m_isActive = false; // ��Q�����A�N�e�B�u��Ԃɂ���
}

/**
 * @brief �e�������������̏����ł��B
 */
void Obstacle::Hit()
{
    // ��Q�����A�N�e�B�u�ȏꍇ�̂ݏ������s��
    if (m_isActive)
    {
        m_hp--; // HP��1���炷
        if (m_hp <= 0)
        {
            Deactivate(); // HP��0�ȉ��ɂȂ����疳����
        }
    }
}

void Obstacle::Update(ObstacleBullet* obstacleBullets)
{
    // ��Q�����A�N�e�B�u�ȏꍇ�̂ݏ������s��
    if (m_isActive)
    {
        // �����ł͓��ɉ������Ȃ����A�K�v�ɉ����Ĉʒu�̍X�V�Ȃǂ��s�����Ƃ��ł���
        // �Ⴆ�΁A��Q�������Ɉړ�����Ȃǂ̏�����ǉ��\
        // �N�[���_�E���^�C�}�[���X�V
        if (m_obstacleFireCooldown > 0.0f) {
            m_obstacleFireCooldown -= 0.1f;
        }
    }
    Obstacle::HandleShooting(obstacleBullets, MAX_OBSTACLE_BULLETS);
}
void Obstacle::HandleShooting(ObstacleBullet* obstacleBullets, int maxObstacleBullets)
{
    // �I�[�g�t�@�C�A���L���A���N�[���_�E�����I�����Ă�����e�𔭎�
    if (m_obstacleFireCooldown <= 0.0f)
    {
        m_obstacleFireCooldown = OBSTACLE_FIRE_COOLDOWN; // �N�[���_�E�������Z�b�g
        

       
        float firePosX[] = { -0.1f, 0.0f, 0.1f }; // ���A�����A�E�̒e��X�ʒu
        float firePosY[] = { 0.06f, 0.1f, 0.06f }; // ���A�����A�E�̒e��Y�ʒu
  
        // ��A�N�e�B�u�Ȓe��T���Ĕ��˂���
        for (int i = 0; i < maxObstacleBullets ; ++i)
        {
            if (!obstacleBullets[i].IsActive())
            {
                // �`��ƈʒu���w�肵�Ēe��L����
                obstacleBullets[i].Activate(
                    m_x ,
                    m_y ,
                    m_shape
                );
				break; // 1���˂����烋�[�v�𔲂���
                
            }
        }
    }
}