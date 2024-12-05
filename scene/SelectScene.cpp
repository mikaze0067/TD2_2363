#include "SelectScene.h"
#include <numbers>
#include <TextureManager.h>



SelectScene::~SelectScene() {
	delete selectSprite_;
}

void SelectScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	titleWorldTransform_.Initialize();
	viewProjection_.Initialize();

	selectHandle_ = TextureManager::Load("Blue.png");
	selectSprite_ = Sprite::Create(selectHandle_, { 0,0 });
}

void SelectScene::Update()
{
	if (Input::GetInstance()->TriggerKey(DIK_RETURN)) {
		finished_ = true;
	}
}

void SelectScene::Draw()
{
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	Model::PreDraw(commandList);


	Model::PostDraw();

	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	selectSprite_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();

}