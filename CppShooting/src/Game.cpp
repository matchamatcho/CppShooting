#include "Game.h"
#include "GameConfig.h" // ゲーム設定ファイルをインクルード
#include <time.h>       // 乱数の初期化 (srand) のために必要
#include <stdlib.h>     // 乱数 (rand) のために必要

/**
 * @brief Gameクラスのコンストラクタ
 * メンバ変数の初期化と乱数シードの設定を行います。
 */
Game::Game() :
    m_playerX(0.0f),
    m_playerY(0.0f),
    m_fireCooldown(0.0f),
    m_obstacleSpawnTimer(0.0f)
{
    // 乱数のシードを現在時刻で初期化
    srand((unsigned int)time(NULL));
}

/**
 * @brief ゲーム全体のロジックを毎フレーム更新します。
 */
void Game::Update()
{
    UpdatePlayer();    // プレイヤーの移動などを処理
    UpdateBullets();   // 弾の移動と発射を処理
    UpdateObstacles(); // 障害物の出現を処理
    CheckCollisions(); // 衝突判定を処理
}

/**
 * @brief プレイヤーの状態を更新します。
 * キー入力に応じた移動と、画面外への移動制限を行います。
 */
void Game::UpdatePlayer()
{
    // 'W', 'S', 'A', 'D' キーでプレイヤーを移動 (GameConfig.h の定数を使用)
    if (GetAsyncKeyState('W') & 0x8000) m_playerY += PLAYER_MOVE_SPEED;
    if (GetAsyncKeyState('S') & 0x8000) m_playerY -= PLAYER_MOVE_SPEED;
    if (GetAsyncKeyState('A') & 0x8000) m_playerX -= PLAYER_MOVE_SPEED;
    if (GetAsyncKeyState('D') & 0x8000) m_playerX += PLAYER_MOVE_SPEED;

    // プレイヤーが画面外に出ないように座標を制限
    if (m_playerX > 0.95f) m_playerX = 0.95f;
    if (m_playerX < -0.95f) m_playerX = -0.95f;
    if (m_playerY > 0.95f) m_playerY = 0.95f;
    if (m_playerY < -0.95f) m_playerY = -0.95f;
}

/**
 * @brief 弾の状態を更新します。
 * クールダウンに基づいて新しい弾を発射し、既存の弾の位置を更新します。
 */
void Game::UpdateBullets()
{
    // 発射クールダウンタイマーを減らす
    m_fireCooldown -= 0.1f;

    // クールダウンが終了したら新しい弾を発射
    if (m_fireCooldown <= 0.0f)
    {
        m_fireCooldown = PLAYER_FIRE_COOLDOWN; // クールダウンをリセット (GameConfig.h の定数を使用)

        // 非アクティブな弾を探して再利用する
        for (int i = 0; i < MAX_BULLETS; ++i)
        {
            if (!m_bullets[i].IsActive())
            {
                // プレイヤーの少し前から弾を発射
                m_bullets[i].Activate(m_playerX, m_playerY + 0.05f);
                break; // 1発撃ったらループを抜ける
            }
        }
    }

    // すべての弾の位置を更新
    for (int i = 0; i < MAX_BULLETS; ++i)
    {
        if (m_bullets[i].IsActive())
        {
            m_bullets[i].Update();
        }
    }
}

/**
 * @brief 障害物の状態を更新します。
 * 一定時間ごとに新しい障害物を生成します。
 */
void Game::UpdateObstacles()
{
    // 障害物出現タイマーを減らす
    m_obstacleSpawnTimer -= 0.05f;

    // タイマーが0になったら新しい障害物を出現させる
    if (m_obstacleSpawnTimer <= 0.0f)
    {
        m_obstacleSpawnTimer = OBSTACLE_SPAWN_INTERVAL; // タイマーをリセット (GameConfig.h の定数を使用)

        // 非アクティブな障害物を探して再利用する
        for (int i = 0; i < MAX_OBSTACLES; ++i)
        {
            if (!m_obstacles[i].IsActive())
            {
                // X座標をランダムに決定し、画面上部から出現させる
                float x = (rand() / (float)RAND_MAX) * 1.8f - 0.9f;
                float y = 0.9f;
                // HPの初期値を定数で設定
                m_obstacles[i].Activate(x, y, OBSTACLE_DEFAULT_HP);
                break; // 1つ出現させたらループを抜ける
            }
        }
    }
}

/**
 * @brief 弾と障害物の当たり判定をチェックします。
 */
void Game::CheckCollisions()
{
    // すべての有効な弾に対してループ
    for (int i = 0; i < MAX_BULLETS; ++i)
    {
        if (m_bullets[i].IsActive())
        {
            // すべての有効な障害物に対してループ
            for (int j = 0; j < MAX_OBSTACLES; ++j)
            {
                if (m_obstacles[j].IsActive())
                {
                    // 弾と障害物の距離を計算 (三平方の定理)
                    float dx = m_bullets[i].GetX() - m_obstacles[j].GetX();
                    float dy = m_bullets[i].GetY() - m_obstacles[j].GetY();
                    float dist_squared = dx * dx + dy * dy; // 平方根の計算を省略するため、距離の2乗で比較

                    // 当たり判定の半径 (の2乗) より距離が小さければ衝突
                    if (dist_squared < OBSTACLE_COLLISION_RADIUS * OBSTACLE_COLLISION_RADIUS)
                    {
                        m_bullets[i].Deactivate();  // 弾を消す
                        m_obstacles[j].Hit();       // 障害物のHPを減らす
                        break; // この弾は消えたので、他の障害物との判定は不要
                    }
                }
            }
        }
    }
}