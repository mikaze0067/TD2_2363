#pragma once
#include <Model.h>
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <Input.h>
#include "MathUtilityForText.h"
#include <PlayerBullet.h>
#include <list>

class Player {
public:
    /// <summary>
    /// 初期化
    /// </summary>
    void Initialize(Model* model, ViewProjection*viewProjection);

    /// <summary>
    /// 毎フレーム処理
    /// </summary>
    void Update();

    /// <summary>
    /// 描画
    /// </summary>
    void Draw(ViewProjection& viewProjection);

    /// <summary>
    /// 攻撃処理（弾を発射）
    /// </summary>
    void Attack();

    /// <summary>
    /// デストラクタ（弾のメモリを解放）
    /// </summary>
    ~Player();

    /// <summary>
    /// ワールド座標を取得
    /// </summary>
    Vector3 GetWorldPosition();

    /// <summary>
    /// 衝突時の処理
    /// </summary>
    void OnCollision();

    /// <summary>
    /// 弾リストを取得
    /// </summary>
    const std::list<PlayerBullet*>& GetBullet() const { return bullets_; }

    /// <summary>
    /// プレイヤーが生存しているかを取得
    /// </summary>
    bool IsHealth() const { return HealthFlag_; }

    /// <summary>
    /// プレイヤーの処理が終了したかを取得
    /// </summary>
    bool IsFinished() const { return finished_; }

private:
    // ワールドトランスフォーム
    WorldTransform worldTransform_;
    ViewProjection* viewProjection_ = nullptr;
    // 3Dモデル
    Model* model_ = nullptr;
    // キーボード入力
    Input* input_ = nullptr;

    // プレイヤーの処理が終了したかのフラグ
    bool finished_ = false;

    // 衝突判定時のフラグ
    bool HealthFlag_ = false;

    // HP
    int hp_ = 1;

    // デスフラグ
    bool isDead_ = false;

    // 発射した弾のリスト
    std::list<PlayerBullet*> bullets_;

    // 弾の発射間隔を制御するクールダウン
    int fireCooldown_ = 0;


};
