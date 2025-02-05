#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Hoshi_Yokeyouya3D.h"
/// <summary>
/// タイトルシーン
/// </summary>
class GameOverScene {
public:
	GameOverScene();

	~GameOverScene();
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
	Audio* audio_ = nullptr;

	bool finished_ = false;

	Sprite* gameOverSprite_ = nullptr;
	uint32_t gameOverTextureHandle_ = 0u;

	//サウンド
	uint32_t soundDataHandle=0;
	uint32_t voiceHandle=0;    
};