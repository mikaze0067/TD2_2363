#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "MathUtilityForText.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <DebugCamera.h>
#include <Enemy.h>
#include <Player.h>
#include "Haikei.h"
#include "PlayerBullet.h"

/// <summary>
/// ゲームシーン
/// </summary>
class Hoshi_Yokeyouya {
public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	Hoshi_Yokeyouya();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Hoshi_Yokeyouya();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void CheckAllCollision();

	bool IsOne() const { return one_; }
	bool IsFinished() const { return finished_; }

	bool IsHealth()const { return HealthFlag_; }

	void GameOver();

	void GameClear();

	// 3Dモデル
	Model* modelPlayer_ = nullptr;
	Model* modelEnemy_ = nullptr;
	Model* modelHaikei_ = nullptr;
	Model* modelPlayerBullet_ = nullptr;
	
	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	// ビュープロジェクション
	ViewProjection viewProjection_;

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	// 自キャラ
	Player* player_ = nullptr;
	// 敵キャラ
	Enemy* enemy_ = nullptr;
	// デバッグカメラ有効
	bool isDebugCameraActive_ = false;

	DebugCamera* debugCamera_ = nullptr;

	bool one_ = false;
	bool finished_ = false;

	bool HealthFlag_ = false;
	// 天球
	Haikei* haikei_ = nullptr;
	// 自機弾
	//PlayerBullet* playerBullet_ = nullptr;

	// 敵の弾
	//EnemyBullet* enemyBullet_ = nullptr;
};