#pragma once
#include "WorldTransform.h"
#include <Model.h>
#include "ViewProjection.h"
#include <EnemyBullet.h>
enum class Phase {
	Approach, //接近する
	Leave,    //離脱する
};

class Player;

class Enemy {
public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model,uint32_t textureHandle);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);

	void Fire();
	
	~Enemy();

	//発射間隔
	static const int kFireInterval = 60;
	//接近フェーズ初期化
	void Approach();

	Vector3 GetWorldPosition();

	void SetPlayer(Player* player) { player_ = player;}

private:
	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	// 3Dモデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//フェーズ
	Phase phase_ = Phase::Approach;
	// 弾
	std::list<EnemyBullet*> bullets_;
	//発射タイマー
	int32_t fireTimer = 0;
	//自キャラ
	Player* player_ = nullptr;
};