#pragma once
#include "Bullet.h" // 弾を発射するためにBulletクラスの定義が必要
#include <windows.h> // GetAsyncKeyState のために必要

/**
 * @class Player
 * @brief プレイヤーキャラクターの管理クラス
 * プレイヤーの移動、弾の発射、状態管理を担当します。
 */
class Player
{
public:
    /**
     * @brief コンストラクタ
     */
    Player();

    /**
     * @brief プレイヤーの状態を毎フレーム更新します。
     * 移動入力と発射入力を処理します。
     * @param bullets ゲーム内に存在する弾の配列へのポインタ
     * @param maxBullets 弾の最大数
     */
    void Update(Bullet* bullets, int maxBullets);

    /**
     * @brief プレイヤーの現在のX座標を返します。
     * @return X座標
     */
    float GetX() const { return m_x; }

    /**
     * @brief プレイヤーの現在のY座標を返します。
     * @return Y座標
     */
    float GetY() const { return m_y; }

private:
    /**
     * @brief キー入力に基づいてプレイヤーを移動させます。
     */
    void HandleMovement();

    /**
     * @brief キー入力に基づいてオートファイアの状態を切り替えます。
     */
    void HandleFireToggle();

    /**
     * @brief オートファイアが有効な場合に弾を発射します。
     * @param bullets ゲーム内に存在する弾の配列へのポインタ
     * @param maxBullets 弾の最大数
     */
    void HandleShooting(Bullet* bullets, int maxBullets);

private:
    float m_x;                  // プレイヤーのX座標
    float m_y;                  // プレイヤーのY座標
    float m_fireCooldown;       // 弾の発射クールダウンタイマー
    bool  m_isAutoFireEnabled;  // オートファイアが有効かどうかのフラグ
    bool  m_wasSpaceKeyPressed; // 前のフレームでスペースキーが押されていたか
};