#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <DebugCamera.h>
#include <Player.h>
#include "MathUtilityForText.h"
#include <Enemy.h>


/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

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

	bool IsFinished() const { return finished_; }

	void GameOver();

	void GameClear();

	// 3Dモデル
	Model* model_ = nullptr;
	Model* modelEnemy_ = nullptr;
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
	//自キャラ
	Player* player_ = nullptr;
	// 敵キャラ
	Enemy* enemy_ = nullptr;
	//デバッグカメラ有効
	bool isDebugCameraActive_ = false;

	DebugCamera* debugCamera_ = nullptr;

	bool finished_ = false;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
