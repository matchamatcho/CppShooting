#include "ObstacleBullet.h"
#include "GameConfig.h" // OBSTACLE_BULLET_SPEEDを使用するため
/**
 * @brief ObstacleBulletクラスのコンストラクタ
 * メンバ変数を初期化します。弾は最初は非アクティブ状態です。
 */
ObstacleBullet::ObstacleBullet() :
	m_isActive(false),
	m_x(0.0f),
	m_y(0.0f),
	m_velocityY(OBSTACLE_BULLET_SPEED), // GameConfig.h の定数で初期化
	m_shape(ObstacleShape::Square) // デフォルト形状を四角に
{
}



