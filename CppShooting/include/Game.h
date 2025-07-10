#pragma once
#include "Bullet.h"
#include "Obstacle.h"
#include <windows.h> // GetAsyncKeyState のために必要

// クラスの前方宣言 (循環参照を避けるため)
class Bullet;
class Obstacle;

/**
 * @class Game
 * @brief ゲーム全体のロジックを管理するクラス
 */
class Game {
public:
    /**
     * @brief コンストラクタ
     */
    Game();

    /**
     * @brief ゲームの状態を毎フレーム更新します。
     */
    void Update();

    // --- 描画用のオブジェクト情報へのアクセスを提供するゲッター関数 ---
    float getPlayerX() const { return m_playerX; }
    float getPlayerY() const { return m_playerY; }
    const Bullet* getBullets() const { return m_bullets; }
    const Obstacle* getObstacles() const { return m_obstacles; }
    static const int getMaxBullets() { return MAX_BULLETS; }
    static const int getMaxObstacles() { return MAX_OBSTACLES; }


private:
    // --- ゲームオブジェクト ---
    float m_playerX; // プレイヤーのX座標
    float m_playerY; // プレイヤーのY座標

    // --- 弾の管理 ---
    static const int MAX_BULLETS = 50;  // 画面上に存在できる弾の最大数
    Bullet m_bullets[MAX_BULLETS];      // 弾の配列
    float m_fireCooldown;               // 弾の発射間隔を制御するタイマー

    // --- 障害物の管理 ---
    static const int MAX_OBSTACLES = 10; // 画面上に存在できる障害物の最大数
    Obstacle m_obstacles[MAX_OBSTACLES]; // 障害物の配列
    float m_obstacleSpawnTimer;          // 障害物の出現間隔を制御するタイマー

    // --- 更新処理のヘルパー関数 ---
    void UpdatePlayer();    // プレイヤーの状態を更新
    void UpdateBullets();   // 弾の状態を更新
    void UpdateObstacles(); // 障害物の状態を更新
    void CheckCollisions(); // 弾と障害物の当たり判定をチェック
};