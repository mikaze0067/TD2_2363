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
class SelectScene {
public:
	~SelectScene();
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

	bool IsOne() const { return One_; }

	bool IsTwo() const { return Two_; }

	bool IsThree() const { return Three_; }

private:

	DirectXCommon* dxCommon_ = nullptr;
	WorldTransform titleWorldTransform_;
	ViewProjection viewProjection_;

	bool One_ = false;
	bool Two_ = false;
	bool Three_ = false;

	uint32_t selectHandle_ = 0;
	Sprite* selectSprite_ = nullptr;

};