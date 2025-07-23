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

    /**
     * @brief 左の戦車の現在の形状を返します。
     * @return 弾の形状
     */
    BulletShape GetLeftShape() const { return m_leftShape; }

    /**
     * @brief 真ん中の戦車の現在の形状を返します。
     * @return 弾の形状
     */
    BulletShape GetCenterShape() const { return m_centerShape; }

    /**
     * @brief 右の戦車の現在の形状を返します。
     * @return 弾の形状
     */
    BulletShape GetRightShape() const { return m_rightShape; }

    void Hit();


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
     * @brief 'Z', 'X', 'C' キー入力に基づいて戦車の形状を変更します。
     */
    void HandleShapeChange();

    /**
     * @brief オートファイアが有効な場合に弾を発射します。
     * @param bullets ゲーム内に存在する弾の配列へのポインタ
     * @param maxBullets 弾の最大数
     */
    void HandleShooting(Bullet* bullets, int maxBullets);

    /**
     * @brief 指定されたキーが押された瞬間かどうかを判定します。
     * @param vKey 判定する仮想キーコード
     * @param wasKeyPressedLastFrame 前のフレームでそのキーが押されていたかどうかの状態を保持する変数への参照
     * @return bool 押された瞬間であればtrue
    */
    bool IsKeyPressedOnce(int vKey, bool& wasKeyPressedLastFrame);

    /**
     * @brief 次の弾の形状を返します。(正方形 -> 三角形 -> 五角形 -> 正方形)
     * @param currentShape 現在の形状
     * @return BulletShape 次の形状
    */
    BulletShape GetNextShape(BulletShape currentShape);


private:
    float m_x;                  // プレイヤーのX座標
    float m_y;                  // プレイヤーのY座標
    float m_fireCooldown;       // 弾の発射クールダウンタイマー
    bool  m_isAutoFireEnabled;  // オートファイアが有効かどうかのフラグ
    bool  m_wasSpaceKeyPressed; // 前のフレームでスペースキーが押されていたか

    // 戦車の形状
    BulletShape m_leftShape;
    BulletShape m_centerShape;
    BulletShape m_rightShape;

    // 形状変更キーの入力状態
    bool m_wasZKeyPressed;
    bool m_wasXKeyPressed;
    bool m_wasCKeyPressed;
};