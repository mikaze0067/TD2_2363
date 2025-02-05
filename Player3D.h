#pragma once
#include "MathUtilityForText.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <Input.h>
#include <Model.h>
#include <PlayerBullet3D.h>
#include <list>

class Player3D {
public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, ViewProjection* viewProjection);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);

	void Attack();

	~Player3D();

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	// 弾
	std::list<PlayerBullet3D*> bullets3D_;

	// 衝突を検知したら呼び出されるコールバック関数
	void OnCollision();

	const std::list<PlayerBullet3D*>& GetBullet() const { return bullets3D_; }

	bool IsHealth() const { return HealthFlag_; }

	bool IsFinished() const { return finished_; }

private:
	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	// 3Dモデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	ViewProjection* viewProjection_ = nullptr;

	// キーボード入力
	Input* input_ = nullptr;

	bool finished_ = false;

	bool HealthFlag_ = false;

	int hp_;
	// デスフラグ
	bool isDead_ = false;

	// 弾の発射間隔を制御するクールダウン
	int fireCooldown_ = 0;
};
