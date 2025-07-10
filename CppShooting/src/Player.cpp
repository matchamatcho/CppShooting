#include "Player.h"
#include <Windows.h>
#include <vector>
#include <cmath> // sin, cos を使うために必要

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Player::Player() :
    m_x(0.0f),
    m_y(0.0f),
    m_radius(0.05f),
    m_vertexCount(4), // 最初は四角形からスタート
    m_shapeChanged(true) // 最初にバッファを作成するためにtrueに設定
{
}

void Player::Update()
{
    // プレイヤーの移動処理
    float moveSpeed = 0.01f;
    if (GetAsyncKeyState('W') & 0x8000) m_y += moveSpeed;
    if (GetAsyncKeyState('S') & 0x8000) m_y -= moveSpeed;
    if (GetAsyncKeyState('A') & 0x8000) m_x -= moveSpeed;
    if (GetAsyncKeyState('D') & 0x8000) m_x += moveSpeed;

    // プレイヤーが画面外に出ないように制限
    if (m_x > 0.95f) m_x = 0.95f;
    if (m_x < -0.95f) m_x = -0.95f;
    if (m_y > 0.95f) m_y = 0.95f;
    if (m_y < -0.95f) m_y = -0.95f;

    // Uキーで頂点を増やす
    if (GetAsyncKeyState('U') & 0x8001) {
        IncreaseVertices();
    }
    // Jキーで頂点を減らす
    if (GetAsyncKeyState('J') & 0x8001) {
        DecreaseVertices();
    }
}

void Player::IncreaseVertices()
{
    // 頂点の上限を設定（お好みで調整してください）
    if (m_vertexCount < 30) {
        m_vertexCount++;
        m_shapeChanged = true;
    }
}

void Player::DecreaseVertices()
{
    // 頂点は最低3つ（三角形）まで
    if (m_vertexCount > 3) {
        m_vertexCount--;
        m_shapeChanged = true;
    }
}

// 正多角形を三角形のリストとして生成する
void Player::GenerateVertices(std::vector<SimpleVertex>& vertices)
{
    vertices.clear();
    // n角形は n-2 個の三角形から作られる
    // しかし、中心点から各辺への三角形で構成すると簡単
    // この方法では n個 の三角形ができる
    vertices.reserve(m_vertexCount * 3);

    float angleStep = 2.0f * (float)M_PI / m_vertexCount;

    for (int i = 0; i < m_vertexCount; ++i)
    {
        float angle1 = i * angleStep;
        float angle2 = (i + 1) * angleStep;

        // 三角形を構成する3つの頂点
        SimpleVertex center = { { 0.0f, 0.0f, 0.5f }, { 1.0f, 1.0f, 1.0f, 1.0f } }; // 中心 (白)
        SimpleVertex v1 = {
            { m_radius * cos(angle1), m_radius * sin(angle1), 0.5f },
            { 0.0f, 1.0f, 0.0f, 1.0f } // 頂点 (緑)
        };
        SimpleVertex v2 = {
            { m_radius * cos(angle2), m_radius * sin(angle2), 0.5f },
            { 0.0f, 0.0f, 1.0f, 1.0f } // 頂点 (青)
        };

        vertices.push_back(center);
        vertices.push_back(v1);
        vertices.push_back(v2);
    }
}