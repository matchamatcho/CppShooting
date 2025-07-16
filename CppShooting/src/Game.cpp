#include "Game.h"
#include "GameConfig.h" 
#include <time.h>       
#include <stdlib.h>     

/**
 * @brief Gameクラスのコンストラクタ
 * メンバ変数の初期化と乱数シードの設定を行います。
 */
Game::Game() :
    m_player(), // Playerオブジェクトを初期化
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
    // PlayerクラスのUpdateを呼び出し、プレイヤーの操作と弾の発射を行う
    m_player.Update(m_bullets, MAX_BULLETS);

    UpdateBullets();   // 弾の移動を処理
    UpdateObstacles(); // 障害物の出現を処理
    CheckCollisions(); // 衝突判定を処理
}

/**
 * @brief 弾の状態を更新します。
 * 発射ロジックはPlayerクラスに移譲し、ここでは弾の移動のみを担当します。
 */
void Game::UpdateBullets()
{
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
                float x = (rand() / (float)RAND_MAX) * 1.54f - 0.77f;
                // Y座標をオブジェクトの大きさを考慮した値に修正 (修正箇所)
                float y = 0.75f;

                // 形状をランダムに決定
                int shapeType = rand() % 3; // 0, 1, 2のいずれかの値
                BulletShape shape;
                switch (shapeType) {
                case 0:
                    shape = BulletShape::Square;
                    break;
                case 1:
                    shape = BulletShape::Triangle;
                    break;
                case 2:
                    shape = BulletShape::Pentagon;
                    break;
                }

                // HPの初期値を定数で設定し、形状も渡す
                m_obstacles[i].Activate(x, y, OBSTACLE_DEFAULT_HP, shape);
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