#pragma once
#include "Bullet.h"
#include "Obstacle.h"
#include <windows.h> // GetAsyncKeyState のために必要

// 前方宣言
class Bullet;
class Obstacle;

class Game {
public:
    Game();

    void Update(); // ゲームの状態を更新

    // 描画用のオブジェクト情報へのアクセスを提供
    float getPlayerX() const { return m_playerX; }
    float getPlayerY() const { return m_playerY; }
    const Bullet* getBullets() const { return m_bullets; }
    const Obstacle* getObstacles() const { return m_obstacles; }
    static const int getMaxBullets() { return MAX_BULLETS; }
    static const int getMaxObstacles() { return MAX_OBSTACLES; }


private:
    // ゲームオブジェクト
    float m_playerX;
    float m_playerY;

    // 弾の管理
    static const int MAX_BULLETS = 50;
    Bullet m_bullets[MAX_BULLETS];
    float m_fireCooldown;

    // 障害物の管理
    static const int MAX_OBSTACLES = 10;
    Obstacle m_obstacles[MAX_OBSTACLES];
    float m_obstacleSpawnTimer;

    // 更新処理のヘルパー関数
    void UpdatePlayer();
    void UpdateBullets();
    void UpdateObstacles();
    void CheckCollisions();
};