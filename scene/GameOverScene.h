#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "GameScene.h"
/// <summary>
/// タイトルシーン
/// </summary>
class GameOverScene {
public:
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

private:

	DirectXCommon* dxCommon_ = nullptr;
	WorldTransform titleWorldTransform_;
	ViewProjection viewProjection_;

	bool finished_ = false;


	// 2Dスプライト
	Sprite* sprite_ = nullptr;
	// テクスチャ
	uint32_t textureHandleRed_ = 0u;

	// 透明度制御用の変数
	Vector4 color = {1.0f, 1.0f, 1.0f, alpha};
	float alpha = 0.027f;

	// ボスフラグ
	bool bossFrag = false;
	// ボスフラグを制御する補助フラグ
	bool bossActivated = false;
	// カウントダウンタイマー
	int timer = 0;
};