
#pragma once
#include "TitleScene.h"
#include <numbers>
#include <TextureManager.h>

TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
	audio_->StopWave(voiceHandle);
}

void TitleScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	audio_ = Audio::GetInstance();
	worldTransform_.Initialize();
	viewProjection_.Initialize();

	model_ = Model::CreateFromOBJ("title", true);

	BlackBGHandle_ = TextureManager::Load("BlackBG.png");
	BlackBGSprite_ = Sprite::Create(BlackBGHandle_, { 0,0 });

	titleModel = new TitleModel();
	titleModel->Initialize(model_,&viewProjection_);

	//サウンド
	soundDataHandle=audio_->LoadWave("startbgm.mp3");
	//音声再生
	voiceHandle=audio_->PlayWave(soundDataHandle,true);
}

void TitleScene::Update()
{
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		finished_ = true;
	}
}

void TitleScene::Draw()
{
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>
	BlackBGSprite_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	titleModel->Draw();

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>


	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion 
}