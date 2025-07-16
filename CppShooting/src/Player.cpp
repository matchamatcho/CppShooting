#include "Player.h"
#include "GameConfig.h" // PLAYER_MOVE_SPEEDやキー設定を利用するため

/**
 * @brief Playerクラスのコンストラクタ
 */
Player::Player() :
    m_x(0.0f),
    m_y(-0.8f),
    m_fireCooldown(0.0f),
    m_isAutoFireEnabled(false),
    m_wasSpaceKeyPressed(false),
    m_leftShape(BulletShape::Square),
    m_centerShape(BulletShape::Triangle),
    m_rightShape(BulletShape::Pentagon),
    m_wasZKeyPressed(false),
    m_wasXKeyPressed(false),
    m_wasCKeyPressed(false)
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

    // 射撃のトグル処理
    HandleFireToggle();

    // 形状変更の処理
    HandleShapeChange();

    // 射撃処理
    HandleShooting(bullets, maxBullets);
}

/**
 * @brief キー入力に基づいてプレイヤーを移動させます。
 */
void Player::HandleMovement()
{
    // GameConfig.hで設定されたキーでプレイヤーを移動
    if (GetAsyncKeyState(KEY_MOVE_UP) & 0x8000) m_y += PLAYER_MOVE_SPEED;
    if (GetAsyncKeyState(KEY_MOVE_DOWN) & 0x8000) m_y -= PLAYER_MOVE_SPEED;
    if (GetAsyncKeyState(KEY_MOVE_LEFT) & 0x8000) m_x -= PLAYER_MOVE_SPEED;
    if (GetAsyncKeyState(KEY_MOVE_RIGHT) & 0x8000) m_x += PLAYER_MOVE_SPEED;

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
    if (IsKeyPressedOnce(KEY_TOGGLE_AUTOFIRE, m_wasSpaceKeyPressed))
    {
        m_isAutoFireEnabled = !m_isAutoFireEnabled; // bool値を反転させる
    }
}

/**
 * @brief キー入力に基づいて戦車の形状を変更します。
 */
void Player::HandleShapeChange()
{
    if (IsKeyPressedOnce(KEY_CHANGE_SHAPE_LEFT, m_wasZKeyPressed))
    {
        m_leftShape = GetNextShape(m_leftShape);
    }
    if (IsKeyPressedOnce(KEY_CHANGE_SHAPE_CENTER, m_wasXKeyPressed))
    {
        m_centerShape = GetNextShape(m_centerShape);
    }
    if (IsKeyPressedOnce(KEY_CHANGE_SHAPE_RIGHT, m_wasCKeyPressed))
    {
        m_rightShape = GetNextShape(m_rightShape);
    }
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
        BulletShape shapesToFire[] = { m_leftShape, m_centerShape, m_rightShape };
        float firePosX[] = { -0.1f, 0.0f, 0.1f }; // 左、中央、右の弾のX位置
        float firePosY[] = { 0.06f, 0.1f, 0.06f }; // 左、中央、右の弾のY位置
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

/**
 * @brief 指定されたキーが押された瞬間かどうかを判定します。
 * @param vKey 判定する仮想キーコード
 * @param wasKeyPressedLastFrame 前のフレームでそのキーが押されていたかどうかの状態を保持する変数への参照
 * @return bool 押された瞬間であればtrue
 */
bool Player::IsKeyPressedOnce(int vKey, bool& wasKeyPressedLastFrame)
{
    bool isKeyDown = (GetAsyncKeyState(vKey) & 0x8000);
    bool keyPressedOnce = isKeyDown && !wasKeyPressedLastFrame;
    wasKeyPressedLastFrame = isKeyDown;
    return keyPressedOnce;
}

/**
 * @brief 次の弾の形状を返します。(正方形 -> 三角形 -> 五角形 -> 正方形)
 * @param currentShape 現在の形状
 * @return BulletShape 次の形状
*/
BulletShape Player::GetNextShape(BulletShape currentShape)
{
    switch (currentShape)
    {
    case BulletShape::Square:
        return BulletShape::Triangle;
    case BulletShape::Triangle:
        return BulletShape::Pentagon;
    case BulletShape::Pentagon:
        return BulletShape::Square;
    default:
        return BulletShape::Square; // 不明な形状の場合はデフォルトに戻す
    }
}