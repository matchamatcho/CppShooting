#include "Player.h"
#include "GameConfig.h" // PLAYER_MOVE_SPEEDなどの設定値を利用するため

/**
 * @brief Playerクラスのコンストラクタ
 */
Player::Player() :
    m_x(0.0f),
    m_y(-0.8f), // 画面下部を初期位置に設定
    m_fireCooldown(0.0f)
{
}

/**
 * @brief プレイヤーの状態を毎フレーム更新します。
 */
void Player::Update(Bullet* bullets, int maxBullets)
{
    // クールダウンタイマーを更新
    if (m_fireCooldown > 0.0f) {
        m_fireCooldown -= 0.1f;
    }

    // 移動処理
    HandleMovement();

    // 射撃処理
    HandleShooting(bullets, maxBullets);
}

/**
 * @brief キー入力に基づいてプレイヤーを移動させます。
 */
void Player::HandleMovement()
{
    // 'W', 'S', 'A', 'D' キーでプレイヤーを移動
    if (GetAsyncKeyState('W') & 0x8000) m_y += PLAYER_MOVE_SPEED;
    if (GetAsyncKeyState('S') & 0x8000) m_y -= PLAYER_MOVE_SPEED;
    if (GetAsyncKeyState('A') & 0x8000) m_x -= PLAYER_MOVE_SPEED;
    if (GetAsyncKeyState('D') & 0x8000) m_x += PLAYER_MOVE_SPEED;

    // プレイヤーが画面外に出ないように座標を制限 (戦車の幅を考慮)
    if (m_x > 0.85f) m_x = 0.85f;
    if (m_x < -0.85f) m_x = -0.85f;
    if (m_y > 0.9f) m_y = 0.9f;
    if (m_y < -0.9f) m_y = -0.9f;
}

/**
 * @brief キー入力に基づいて弾を発射します。
 */
void Player::HandleShooting(Bullet* bullets, int maxBullets)
{
    // スペースキーが押され、かつクールダウンが終了していたら新しい弾を発射
    if ((GetAsyncKeyState(VK_SPACE) & 0x8000) && m_fireCooldown <= 0.0f)
    {
        m_fireCooldown = PLAYER_FIRE_COOLDOWN; // クールダウンをリセット

        // ★★★ここから変更★★★
        // 発射位置を新しい戦車の形状に合わせる
        // 左(正方形)、中央(正三角形)、右(正五角形)の先端から発射
        const float firePosX[] = { -0.1f, 0.0f,  0.1f };
        const float firePosY[] = { 0.05f, 0.09f, 0.09f };
        // ★★★ここまで変更★★★

        int bulletsFired = 0; // このフレームで発射した弾の数

        // 非アクティブな弾を探して3発同時に発射する
        for (int i = 0; i < maxBullets; ++i)
        {
            if (!bullets[i].IsActive())
            {
                // プレイヤーの位置にオフセットを加えた場所から弾を発射
                bullets[i].Activate(m_x + firePosX[bulletsFired], m_y + firePosY[bulletsFired]);

                bulletsFired++;
                if (bulletsFired >= 3) {
                    break; // 3発撃ったらループを抜ける
                }
            }
        }
    }
}