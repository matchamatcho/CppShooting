#include "Game.h"
#include <time.h>
#include <stdlib.h> // rand, srand

Game::Game() :
    m_playerX(0.0f),
    m_playerY(0.0f),
    m_fireCooldown(0.0f),
    m_obstacleSpawnTimer(0.0f)
{
    srand((unsigned int)time(NULL)); // 乱数の初期化
}

void Game::Update()
{
    UpdatePlayer();
    UpdateBullets();
    UpdateObstacles();
    CheckCollisions();
}

void Game::UpdatePlayer()
{
    // プレイヤーの移動処理
    float moveSpeed = 0.02f;
    if (GetAsyncKeyState('W') & 0x8000) m_playerY += moveSpeed;
    if (GetAsyncKeyState('S') & 0x8000) m_playerY -= moveSpeed;
    if (GetAsyncKeyState('A') & 0x8000) m_playerX -= moveSpeed;
    if (GetAsyncKeyState('D') & 0x8000) m_playerX += moveSpeed;

    // プレイヤーが画面外に出ないように制限
    if (m_playerX > 0.95f) m_playerX = 0.95f;
    if (m_playerX < -0.95f) m_playerX = -0.95f;
    if (m_playerY > 0.95f) m_playerY = 0.95f;
    if (m_playerY < -0.95f) m_playerY = -0.95f;
}

void Game::UpdateBullets()
{
    // 弾の発射クールダウンタイマーを減らす
    m_fireCooldown -= 0.1f;

    // クールダウンが終了したら新しい弾を発射する
    if (m_fireCooldown <= 0.0f)
    {
        // 次の発射までの時間を再設定
        m_fireCooldown = 1.0f;

        // 非アクティブな弾を探して発射する
        for (int i = 0; i < MAX_BULLETS; ++i)
        {
            if (!m_bullets[i].IsActive())
            {
                // 三角形の先端から発射する
                m_bullets[i].Activate(m_playerX, m_playerY + 0.05f);
                break; // 1フレームに1発だけ発射
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

void Game::UpdateObstacles()
{
    // 障害物生成タイマーを減らす
    m_obstacleSpawnTimer -= 0.05f;

    // タイマーが0になったら新しい障害物を生成
    if (m_obstacleSpawnTimer <= 0.0f)
    {
        m_obstacleSpawnTimer = 10.0f; // 次の生成までの時間

        for (int i = 0; i < MAX_OBSTACLES; ++i)
        {
            if (!m_obstacles[i].IsActive())
            {
                // X座標をランダムに決定 (-0.9から0.9の範囲)
                float x = (rand() / (float)RAND_MAX) * 1.8f - 0.9f;
                // Y座標は画面の上方に固定
                float y = 0.9f;
                // HPを3に設定
                int hp = 3;
                m_obstacles[i].Activate(x, y, hp);
                break; // 1つ生成したら抜ける
            }
        }
    }
    // ここでは障害物の移動はさせないが、もし移動させるならここに追加する
}


void Game::CheckCollisions()
{
    // 弾と障害物の当たり判定
    for (int i = 0; i < MAX_BULLETS; ++i)
    {
        if (m_bullets[i].IsActive())
        {
            for (int j = 0; j < MAX_OBSTACLES; ++j)
            {
                if (m_obstacles[j].IsActive())
                {
                    float dx = m_bullets[i].GetX() - m_obstacles[j].GetX();
                    float dy = m_bullets[i].GetY() - m_obstacles[j].GetY();
                    float dist_squared = dx * dx + dy * dy;

                    // 簡単な円形の当たり判定 (0.05は障害物の半径の目安)
                    if (dist_squared < 0.05f * 0.05f)
                    {
                        m_bullets[i].Deactivate(); // 弾を消す
                        m_obstacles[j].Hit();      // 障害物のHPを減らす
                        break; // この弾はもう他の障害物には当たらない
                    }
                }
            }
        }
    }
}