#pragma once

// ��Q�����Ǘ�����N���X
class Obstacle
{
public:
    // �R���X�g���N�^
    Obstacle();

    // ��Q����L��������
    void Activate(float x, float y, int hp);

    // ��Q���𖳌�������
    void Deactivate();

    // ��Q�����L�����ǂ�����Ԃ�
    bool IsActive() const { return m_isActive; }

    // ��Q����HP�����鏈��
    void Hit();

    // X���W��Ԃ�
    float GetX() const { return m_x; }
    // Y���W��Ԃ�
    float GetY() const { return m_y; }

private:
    bool  m_isActive;
    float m_x;
    float m_y;
    int   m_hp;
};