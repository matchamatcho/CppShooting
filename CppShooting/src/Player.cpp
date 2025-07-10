#include "Player.h"
#include <Windows.h>
#include <vector>
#include <cmath> // sin, cos ���g�����߂ɕK�v

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Player::Player() :
    m_x(0.0f),
    m_y(0.0f),
    m_radius(0.05f),
    m_vertexCount(4), // �ŏ��͎l�p�`����X�^�[�g
    m_shapeChanged(true) // �ŏ��Ƀo�b�t�@���쐬���邽�߂�true�ɐݒ�
{
}

void Player::Update()
{
    // �v���C���[�̈ړ�����
    float moveSpeed = 0.01f;
    if (GetAsyncKeyState('W') & 0x8000) m_y += moveSpeed;
    if (GetAsyncKeyState('S') & 0x8000) m_y -= moveSpeed;
    if (GetAsyncKeyState('A') & 0x8000) m_x -= moveSpeed;
    if (GetAsyncKeyState('D') & 0x8000) m_x += moveSpeed;

    // �v���C���[����ʊO�ɏo�Ȃ��悤�ɐ���
    if (m_x > 0.95f) m_x = 0.95f;
    if (m_x < -0.95f) m_x = -0.95f;
    if (m_y > 0.95f) m_y = 0.95f;
    if (m_y < -0.95f) m_y = -0.95f;

    // U�L�[�Œ��_�𑝂₷
    if (GetAsyncKeyState('U') & 0x8001) {
        IncreaseVertices();
    }
    // J�L�[�Œ��_�����炷
    if (GetAsyncKeyState('J') & 0x8001) {
        DecreaseVertices();
    }
}

void Player::IncreaseVertices()
{
    // ���_�̏����ݒ�i���D�݂Œ������Ă��������j
    if (m_vertexCount < 30) {
        m_vertexCount++;
        m_shapeChanged = true;
    }
}

void Player::DecreaseVertices()
{
    // ���_�͍Œ�3�i�O�p�`�j�܂�
    if (m_vertexCount > 3) {
        m_vertexCount--;
        m_shapeChanged = true;
    }
}

// �����p�`���O�p�`�̃��X�g�Ƃ��Đ�������
void Player::GenerateVertices(std::vector<SimpleVertex>& vertices)
{
    vertices.clear();
    // n�p�`�� n-2 �̎O�p�`��������
    // �������A���S�_����e�ӂւ̎O�p�`�ō\������ƊȒP
    // ���̕��@�ł� n�� �̎O�p�`���ł���
    vertices.reserve(m_vertexCount * 3);

    float angleStep = 2.0f * (float)M_PI / m_vertexCount;

    for (int i = 0; i < m_vertexCount; ++i)
    {
        float angle1 = i * angleStep;
        float angle2 = (i + 1) * angleStep;

        // �O�p�`���\������3�̒��_
        SimpleVertex center = { { 0.0f, 0.0f, 0.5f }, { 1.0f, 1.0f, 1.0f, 1.0f } }; // ���S (��)
        SimpleVertex v1 = {
            { m_radius * cos(angle1), m_radius * sin(angle1), 0.5f },
            { 0.0f, 1.0f, 0.0f, 1.0f } // ���_ (��)
        };
        SimpleVertex v2 = {
            { m_radius * cos(angle2), m_radius * sin(angle2), 0.5f },
            { 0.0f, 0.0f, 1.0f, 1.0f } // ���_ (��)
        };

        vertices.push_back(center);
        vertices.push_back(v1);
        vertices.push_back(v2);
    }
}