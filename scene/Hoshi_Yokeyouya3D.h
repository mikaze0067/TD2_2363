#pragma once
#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <DebugCamera.h>
#include <Player3D.h>
#include "MathUtilityForText.h"
#include <Enemy3D.h>
#include <Haikei.h>

/// <summary>
/// ゲームシーン
/// </summary>
class Hoshi_Yokeyouya3D {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	Hoshi_Yokeyouya3D();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Hoshi_Yokeyouya3D();

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

	bool IsThree() const { return three_; }

	bool IsFinished() const { return finished_; }
	
	bool IsHealth() const { return HealthFlag_; }

	void GameOver();

	void GameClear();

	// 3Dモデル
	Model* modelPlayer3D_ = nullptr;
	Model* modelEnemy3D_ = nullptr;
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
	Player3D* player3D_ = nullptr;
	// 敵キャラ
	Enemy3D* enemy3D_ = nullptr;

	bool three_ = false;
	bool finished_ = false;
	bool HealthFlag_ = false;

	// 天球
	Haikei* haikei_ = nullptr;

	// サウンド
	uint32_t soundDataHandle = 0;
	uint32_t voiceHandle = 0;
};