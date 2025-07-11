#include "Player.h"
#include "GameConfig.h" // PLAYER_MOVE_SPEEDなどの設定値を利用するため

/**
 * @brief Playerクラスのコンストラクタ
 */
Player::Player() :
    m_x(0.0f),
    m_y(-0.8f),
    m_fireCooldown(0.0f),
    m_isAutoFireEnabled(false),
    m_wasSpaceKeyPressed(false)
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

    // 射撃のトグル処理と、実際の射撃処理を分離
    HandleFireToggle();
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

    // プレイヤーが画面外に出ないように座標を制限
    if (m_x > 0.85f) m_x = 0.85f;
    if (m_x < -0.85f) m_x = -0.85f;
    if (m_y > 0.9f) m_y = 0.9f;
    if (m_y < -0.9f) m_y = -0.9f;
}

/**
 * @brief キー入力に基づいてオートファイアの状態を切り替えます。
 */
void Player::HandleFireToggle()
{
    // 現在のスペースキーの状態を取得
    bool isSpaceKeyDown = (GetAsyncKeyState(VK_SPACE) & 0x8000);

    // 「直前のフレームでは押されておらず、現在のフレームで押されている」場合、トグルを切り替える
    if (isSpaceKeyDown && !m_wasSpaceKeyPressed) {
        m_isAutoFireEnabled = !m_isAutoFireEnabled; // bool値を反転させる
    }

    // 現在のキーの状態を「直前の状態」として保存する
    m_wasSpaceKeyPressed = isSpaceKeyDown;
}


/**
 * @brief オートファイアが有効な場合に弾を発射します。
 */
void Player::HandleShooting(Bullet* bullets, int maxBullets)
{
    // オートファイアが有効、かつクールダウンが終了していたら弾を発射
    if (m_isAutoFireEnabled && m_fireCooldown <= 0.0f)
    {
        m_fireCooldown = PLAYER_FIRE_COOLDOWN; // クールダウンをリセット

        // 発射する弾の形状と、それぞれの発射位置オフセットを定義
        BulletShape shapesToFire[] = { BulletShape::Square, BulletShape::Triangle, BulletShape::Pentagon };
        float firePosX[] = { -0.1f, 0.0f, 0.1f }; // 四角、三角、五角の弾のX位置
        float firePosY[] = { 0.06f, 0.1f, 0.06f }; // 四角、三角、五角の弾のY位置
        int numBulletsToFire = 3;

        int bulletsFired = 0;
        // 非アクティブな弾を探して発射する
        for (int i = 0; i < maxBullets && bulletsFired < numBulletsToFire; ++i)
        {
            if (!bullets[i].IsActive())
            {
                // 形状と位置を指定して弾を有効化
                bullets[i].Activate(
                    m_x + firePosX[bulletsFired],
                    m_y + firePosY[bulletsFired],
                    shapesToFire[bulletsFired]
                );
                bulletsFired++;
            }
        }
    }
}