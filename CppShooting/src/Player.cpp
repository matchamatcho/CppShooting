#include "Player.h"
#include "GameConfig.h" // PLAYER_MOVE_SPEEDなどの設定値を利用するため

/**
 * @brief Playerクラスのコンストラクタ
 */
Player::Player() :
    m_x(0.0f),
    m_y(-0.8f),
    m_fireCooldown(0.0f),
    m_isAutoFireEnabled(false), // ★追加: 初期状態はオートファイアOFF
    m_wasSpaceKeyPressed(false) // ★追加: 初期状態はキーが押されていない
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

    // ★変更: 射撃のトグル処理と、実際の射撃処理を分離
    HandleFireToggle();
    HandleShooting(bullets, maxBullets);
}

/**
 * @brief キー入力に基づいてプレイヤーを移動させます。(変更なし)
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
 * @brief ★★★ここから追加★★★
 * キー入力に基づいてオートファイアの状態を切り替えます。
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
 * @brief ★★★ここから変更★★★
 * オートファイアが有効な場合に弾を発射します。
 */
void Player::HandleShooting(Bullet* bullets, int maxBullets)
{
    // オートファイアが有効、かつクールダウンが終了していたら弾を発射
    if (m_isAutoFireEnabled && m_fireCooldown <= 0.0f)
    {
        m_fireCooldown = PLAYER_FIRE_COOLDOWN; // クールダウンをリセット

        // 発射位置を定義 (プレイヤーの中心からの相対座標)
        const float firePosX[] = { -0.1f, 0.0f,  0.1f };
        const float firePosY[] = { 0.05f, 0.09f, 0.09f };

        int bulletsFired = 0;

        // 非アクティブな弾を探して3発同時に発射する
        for (int i = 0; i < maxBullets; ++i)
        {
            if (!bullets[i].IsActive())
            {
                bullets[i].Activate(m_x + firePosX[bulletsFired], m_y + firePosY[bulletsFired]);

                bulletsFired++;
                if (bulletsFired >= 3) {
                    break;
                }
            }
        }
    }
}