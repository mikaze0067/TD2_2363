#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Hoshi_Yokeyouya3D.h"
#include "TitleModel.h"
/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene {
public:
	TitleScene();

	~TitleScene();
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
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
	Model* model_ = nullptr;
	Audio* audio_ = nullptr;
	
	bool finished_ = false;

	TitleModel* titleModel = nullptr;

	uint32_t BlackBGHandle_ = 0;
	Sprite* BlackBGSprite_ = nullptr;

	//サウンド
	uint32_t soundDataHandle=0;
	uint32_t voiceHandle=0;  
};