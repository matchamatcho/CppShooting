#pragma once
#include <vector>
#include "SimpleVertex.h" // ���_�\���̂����L���邽�߂ɃC���N���[�h

class Player
{
public:
    Player();

    // �X�V
    void Update();

    // ���_�̐���ύX
    void IncreaseVertices();
    void DecreaseVertices();

    // �Q�b�^�[
    float GetX() const { return m_x; }
    float GetY() const { return m_y; }
    int GetVertexCount() const { return m_vertexCount; }
    bool HasShapeChanged() const { return m_shapeChanged; }
    void OnShapeUpdated() { m_shapeChanged = false; }

    // ���_�f�[�^�𐶐�����
    void GenerateVertices(std::vector<SimpleVertex>& vertices);

private:
    float m_x;          // X���W
    float m_y;          // Y���W
    float m_radius;     // �|���S���̔��a�i�T�C�Y�j
    int m_vertexCount;  // ���_�̐��i�����j
    bool m_shapeChanged; // �`�󂪕ύX���ꂽ���ǂ����̃t���O
};