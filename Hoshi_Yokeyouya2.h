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

/// <summary>
/// ゲームシーン
/// </summary>
class Hoshi_Yokeyouya2 {
public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	Hoshi_Yokeyouya2();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Hoshi_Yokeyouya2();

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

	bool IsTwo() const { return two_; }
	bool IsFinished() const { return finished_; }
	bool IsHealth()const { return HealthFlag_; }

	void GameOver();

	void GameClear();

	// 3Dモデル
	Model* modelPlayer_ = nullptr;
	Model* modelEnemy_ = nullptr;
	Model* modelHaikei_ = nullptr;
	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	// ビュープロジェクション
	ViewProjection viewProjection_;

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	// テクスチャハンドル
	uint32_t textureHandleEnemy_ = 0;
	// 自キャラ
	Player* player_ = nullptr;
	// 敵キャラ
	Enemy* enemy_ = nullptr;
	// デバッグカメラ有効
	bool isDebugCameraActive_ = false;

	DebugCamera* debugCamera_ = nullptr;

	bool two_ = false;
	bool finished_ = false;
	bool HealthFlag_ = false;
	// 天球
	Haikei* haikei_ = nullptr;

	//サウンド
	uint32_t soundDataHandle=0;
	uint32_t voiceHandle=0;    

	int currentStage_ = 0;
};