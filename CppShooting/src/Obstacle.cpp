#include "Obstacle.h"
#include "GameConfig.h" // OBSTACLE_FIRE_COOLDOWNを使用するため

/**
 * @brief Obstacleクラスのコンストラクタ
 * メンバ変数を初期化します。障害物は最初は非アクティブ状態です。
 */
Obstacle::Obstacle() :
    m_isActive(false),
    m_x(0.0f),
    m_y(0.0f),
    m_hp(0),
    m_shape(BulletShape::Square),// デフォルト形状
    m_obstacleFireCooldown(0.0f)
{
}

/**
 * @brief 障害物を有効化します。
 * @param x 出現位置のX座標
 * @param y 出現位置のY座標
 * @param hp 初期HP
 * @param shape 障害物の形状
 */
void Obstacle::Activate(float x, float y, int hp, BulletShape shape)
{
    m_isActive = true; // 障害物をアクティブ状態にする
    m_x = x;           // X座標を設定
    m_y = y;           // Y座標を設定
    m_hp = hp;         // HPを設定
    m_shape = shape;   // 形状を設定
}

/**
 * @brief 障害物を無効化します。
 */
void Obstacle::Deactivate()
{
    m_isActive = false; // 障害物を非アクティブ状態にする
}

/**
 * @brief 弾が当たった時の処理です。
 */
void Obstacle::Hit()
{
    // 障害物がアクティブな場合のみ処理を行う
    if (m_isActive)
    {
        m_hp--; // HPを1減らす
        if (m_hp <= 0)
        {
            Deactivate(); // HPが0以下になったら無効化
        }
    }
}

void Obstacle::Update(ObstacleBullet* obstacleBullets)
{
    // 障害物がアクティブな場合のみ処理を行う
    if (m_isActive)
    {
        // ここでは特に何もしないが、必要に応じて位置の更新などを行うことができる
        // 例えば、障害物が下に移動するなどの処理を追加可能
        // クールダウンタイマーを更新
        if (m_obstacleFireCooldown > 0.0f) {
            m_obstacleFireCooldown -= 0.1f;
        }
    }
    Obstacle::HandleShooting(obstacleBullets, MAX_OBSTACLE_BULLETS);
}
void Obstacle::HandleShooting(ObstacleBullet* obstacleBullets, int maxObstacleBullets)
{
    // オートファイアが有効、かつクールダウンが終了していたら弾を発射
    if (m_obstacleFireCooldown <= 0.0f)
    {
        m_obstacleFireCooldown = OBSTACLE_FIRE_COOLDOWN; // クールダウンをリセット
        

       
        float firePosX[] = { -0.1f, 0.0f, 0.1f }; // 左、中央、右の弾のX位置
        float firePosY[] = { 0.06f, 0.1f, 0.06f }; // 左、中央、右の弾のY位置
  
        // 非アクティブな弾を探して発射する
        for (int i = 0; i < maxObstacleBullets ; ++i)
        {
            if (!obstacleBullets[i].IsActive())
            {
                // 形状と位置を指定して弾を有効化
                obstacleBullets[i].Activate(
                    m_x ,
                    m_y ,
                    m_shape
                );
				break; // 1つ発射したらループを抜ける
                
            }
        }
    }
}