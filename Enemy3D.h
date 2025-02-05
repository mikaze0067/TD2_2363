#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <EnemyBullet3D.h>
#include <Model.h>
#include <Sprite.h>
#include <DirectXCommon.h>

enum class Phase3D {
	Approach, // 接近する
	Leave,    // 離脱する
};

class Player3D;

class Enemy3D {
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

	void Fire();

	~Enemy3D();

	// 発射間隔
	static const int kFireInterval = 10;
	// 接近フェーズ初期化
	void Approach();

	Vector3 GetWorldPosition();

	void SetPlayer(Player3D* player3D) { player3D_ = player3D; }

	// ワールド座標を取得するメソッド
	Vector3 GetWorldPosition() const { return worldTransform_.translation_; }

	void OnCollision();

	const std::list<EnemyBullet3D*>& GetBullets() const { return bullets3D_; }

	bool IsFinished() const { return finished_; }

private:

	DirectXCommon* dxCommon_ = nullptr;
	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	// 3Dモデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	ViewProjection* viewProjection_ = nullptr;
	// フェーズ
	Phase3D phase3D_ = Phase3D::Approach;
	// 弾
	std::list<EnemyBullet3D*> bullets3D_;
	// 発射タイマー
	int32_t fireTimer = 0;
	// 自キャラ
	Player3D* player3D_ = nullptr;
	// デスフラグ
	bool isDead_ = false;

	bool finished_ = false;

	int hp_;

	int fireCount_ = 0; // 弾の発射回数を追跡

	std::vector<Sprite*> hpSprites3D_;
};