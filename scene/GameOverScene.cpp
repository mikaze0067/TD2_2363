
#pragma once
#include "GameOverScene.h"
#include <numbers>
#include <TextureManager.h>

GameOverScene::GameOverScene()
{
}

GameOverScene::~GameOverScene()
{
	audio_->StopWave(voiceHandle);
}

void GameOverScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	audio_ = Audio::GetInstance();
	titleWorldTransform_.Initialize();
	viewProjection_.Initialize();

	gameOverTextureHandle_ = TextureManager::Load("gameOver.png");
	gameOverSprite_ = Sprite::Create(gameOverTextureHandle_, { 0,0 });

	//サウンド
	soundDataHandle=audio_->LoadWave("gameoverbgm.mp3");
	//音声再生
	voiceHandle=audio_->PlayWave(soundDataHandle,true);
}

void GameOverScene::Update()
{
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		finished_ = true;
	}

}

void GameOverScene::Draw()
{
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	Model::PreDraw(commandList);

	Model::PostDraw();

	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	gameOverSprite_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();
}