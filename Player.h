#pragma once
#include <Model.h>
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <Input.h>
#include "MathUtilityForText.h"
#include <PlayerBullet.h>
#include <list>

class Player {
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

	void Attack();

	~Player();

	//ワールド座標を取得
	Vector3 GetWorldPosition();

	// 弾
	std::list<PlayerBullet*> bullets_;

	// 衝突を検知したら呼び出されるコールバック関数
	void OnCollision();

	const std::list<PlayerBullet*>& GetBullet() const { return bullets_; }


private:

	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	// 3Dモデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//キーボード入力
	Input* input_ = nullptr;
	
	


};