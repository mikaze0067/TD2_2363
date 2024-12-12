#pragma once
#include "DirectXCommon.h"
#include "Hoshi_Yokeyouya3D.h"
#include "Input.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
/// <summary>
/// タイトルシーン
/// </summary>
class GameClearScene {
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

	uint32_t CrearHandle_ = 0;
	Sprite* crearSprite_ = nullptr;
};