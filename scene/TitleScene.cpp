#include "TitleScene.h"
#include <numbers>
#include <TextureManager.h>

void TitleScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	titleWorldTransform_.Initialize();
	viewProjection_.Initialize();

	TitleHandle_ = TextureManager::Load("Green.png");
	titleSprite_ = Sprite::Create(TitleHandle_, { 0,0 });
}

void TitleScene::Update()
{
	if (Input::GetInstance()->TriggerKey(DIK_RETURN)) {
		finished_ = true;
	}
}

void TitleScene::Draw()
{
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	Model::PreDraw(commandList);

	Model::PostDraw();

	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	titleSprite_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();
}