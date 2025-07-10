#pragma once
#include <vector>
#include "SimpleVertex.h" // 頂点構造体を共有するためにインクルード

class Player
{
public:
    Player();

    // 更新
    void Update();

    // 頂点の数を変更
    void IncreaseVertices();
    void DecreaseVertices();

    // ゲッター
    float GetX() const { return m_x; }
    float GetY() const { return m_y; }
    int GetVertexCount() const { return m_vertexCount; }
    bool HasShapeChanged() const { return m_shapeChanged; }
    void OnShapeUpdated() { m_shapeChanged = false; }

    // 頂点データを生成する
    void GenerateVertices(std::vector<SimpleVertex>& vertices);

private:
    float m_x;          // X座標
    float m_y;          // Y座標
    float m_radius;     // ポリゴンの半径（サイズ）
    int m_vertexCount;  // 頂点の数（強さ）
    bool m_shapeChanged; // 形状が変更されたかどうかのフラグ
};