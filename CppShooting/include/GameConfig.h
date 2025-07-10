#pragma once

// ============== ゲーム設定値 ================

// --- プレイヤー関連 ---
const float PLAYER_MOVE_SPEED = 0.02f;      // プレイヤーの移動速度
const float PLAYER_FIRE_COOLDOWN = 1.0f;    // 弾の発射クールダウン時間 (値が小さいほど連射速度が上がる) ★変更：3.0f -> 5.0f

// --- 弾関連 ---
const float BULLET_HALF_WIDTH = 0.005f;     // 弾の幅の半分
const float BULLET_HALF_HEIGHT = 0.02f;    // 弾の高さの半分
const float BULLET_SPEED = 0.05f;           // 弾の飛ぶ速度

// --- 障害物関連 ---
const float OBSTACLE_SPAWN_INTERVAL = 10.0f; // 障害物が次に出現するまでの時間
const int   OBSTACLE_DEFAULT_HP = 3;         // 障害物の初期HP
const float OBSTACLE_COLLISION_RADIUS = 0.05f; // 障害物との当たり判定の半径

// ==========================================