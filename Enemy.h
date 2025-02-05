#pragma once
#include "WorldTransform.h"
#include <Model.h>
#include "ViewProjection.h"
#include <EnemyBullet.h>
#include <Sprite.h>
#include <DirectXCommon.h>
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
	void Initialize(Model* model,ViewProjection*viewProjection);

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

	// ワールド座標を取得するメソッド
	Vector3 GetWorldPosition() const { return worldTransform_.translation_; }

	void OnCollision();

	const std::list<EnemyBullet*>& GetBullets() const { return bullets_; }

	bool IsFinished()const { return finished_; }


private:

	DirectXCommon* dxCommon_ = nullptr;
	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_ = nullptr;
	// 3Dモデル
	Model* model_ = nullptr;
	//フェーズ
	Phase phase_ = Phase::Approach;
	// 弾
	std::list<EnemyBullet*> bullets_;
	//発射タイマー
	int32_t fireTimer = 0;
	//自キャラ
	Player* player_ = nullptr;
	// デスフラグ
	bool isDead_ = false;

	bool finished_ = false;

	int hp_;

	int fireCount_ = 0; // 弾の発射回数を追跡

	std::vector<Sprite*> hpSprites_;

	Model* enemyModel_ = nullptr;
};