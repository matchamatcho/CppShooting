#pragma once

// �e���Ǘ�����N���X
class Bullet
{
public:
    // �R���X�g���N�^
    Bullet();

    // �e��L�����i���ˁj����
    void Activate(float x, float y);

    // �e�𖳌�������
    void Deactivate();

    // �e�̈ʒu���X�V����
    void Update();

    // �e���L�����ǂ�����Ԃ�
    bool IsActive() const { return m_isActive; }
    // X���W��Ԃ�
    float GetX() const { return m_x; }
    // Y���W��Ԃ�
    float GetY() const { return m_y; }

private:
    bool  m_isActive;  // �e����ʓ��ɑ��݂��A�L�����ǂ����̃t���O
    float m_x;         // X���W
    float m_y;         // Y���W
    float m_velocityY; // Y�����̑��x
};