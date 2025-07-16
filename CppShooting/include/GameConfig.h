#pragma once
#include <windows.h> // 仮想キーコード(VK_*)を使用するためにインクルード

// ============== ゲーム設定値 ================

// --- プレイヤー関連 ---
const float PLAYER_MOVE_SPEED = 0.02f;      // プレイヤーの移動速度
const float PLAYER_FIRE_COOLDOWN = 1.0f;    // 弾の発射クールダウン時間 (値が小さいほど連射速度が上がる)

// --- 弾関連 ---
const float BULLET_HALF_WIDTH = 0.005f;     // 弾の幅の半分
const float BULLET_HALF_HEIGHT = 0.02f;    // 弾の高さの半分
const float BULLET_SPEED = 0.05f;           // 弾の飛ぶ速度

// --- 障害物関連 ---
const float OBSTACLE_SPAWN_INTERVAL = 3.0f; // 障害物が次に出現するまでの時間
const int   OBSTACLE_DEFAULT_HP = 3;         // 障害物の初期HP
const float OBSTACLE_COLLISION_RADIUS = 0.05f; // 障害物との当たり判定の半径
const float OBSTACLE_SCALE = 1.0f;          // 障害物の大きさの倍率 (1.0fが基準)


// ============== キーバインド設定 ================
// ゲームの操作キーを設定します。
// 文字キーは 'W' のようにシングルクォートで囲み、
// 特殊キーは VK_SPACE (スペースキー) のように指定します。

// --- ゲーム操作 ---
const int KEY_QUIT = 'P'; // ゲームを終了するキー

// --- プレイヤー操作 ---
const int KEY_MOVE_UP = 'W';        // 上移動
const int KEY_MOVE_DOWN = 'S';        // 下移動
const int KEY_MOVE_LEFT = 'A';        // 左移動
const int KEY_MOVE_RIGHT = 'D';        // 右移動
const int KEY_TOGGLE_AUTOFIRE = VK_SPACE;   // オートファイア切り替え
const int KEY_CHANGE_SHAPE_LEFT = 'Z';        // 左戦車の形状変更
const int KEY_CHANGE_SHAPE_CENTER = 'X';        // 中央戦車の形状変更
const int KEY_CHANGE_SHAPE_RIGHT = 'C';        // 右戦車の形状変更

// ==========================================