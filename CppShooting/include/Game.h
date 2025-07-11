#pragma once
#include "Bullet.h"
#include "Obstacle.h"
#include "Player.h"   // Playerクラスをインクルード

// クラスの前方宣言
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
    const Player& getPlayer() const { return m_player; }
    const Bullet* getBullets() const { return m_bullets; }
    const Obstacle* getObstacles() const { return m_obstacles; }
    static const int getMaxBullets() { return MAX_BULLETS; }
    static const int getMaxObstacles() { return MAX_OBSTACLES; }


private:
    // --- ゲームオブジェクト ---
    Player m_player; // プレイヤーオブジェクト

    // --- 弾の管理 ---
    static const int MAX_BULLETS = 50;
    Bullet m_bullets[MAX_BULLETS];

    // --- 障害物の管理 ---
    static const int MAX_OBSTACLES = 10;
    Obstacle m_obstacles[MAX_OBSTACLES];
    float m_obstacleSpawnTimer;

    // --- 更新処理のヘルパー関数 ---
    void UpdateBullets();   // 弾の状態を更新
    void UpdateObstacles(); // 障害物の状態を更新
    void CheckCollisions(); // 弾と障害物の当たり判定をチェック
};