#include "GameOverScene.h"
#include <numbers>
#include <TextureManager.h>

void GameOverScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	titleWorldTransform_.Initialize();
	viewProjection_.Initialize();

	textureHandleRed_ = TextureManager::Load("RED.png ");

	sprite_ = Sprite::Create(textureHandleRed_, {0.0});

}

void GameOverScene::Update()
{
	sprite_->SetColor(color);

if (!bossActivated) { // 一度だけ有効化
	bossFrag = true;
	bossActivated = true; // 再度実行されないようにする
	timer = 110;          // 3秒間のカウントダウン
}
// ボスフラグが立っている間、透明度を上げ下げする
if (bossFrag) {
	
	
	timer--;

	// 透明度を増加
	color.w += alpha;

	// 透明度が0.8を超えたら減少方向に切り替え、0.0未満になったら増加方向に切り替え
	if (color.w >= 0.8f || color.w <= 0.0f) {
		alpha = -alpha; // 増加・減少の切り替え
	}

	// 透明度が0.0以下にならないように制限
	if (color.w < 0.0f) {
		color.w = 0.0f;
	}

	// 透明度が1.0以上にならないように制限
	if (color.w > 1.0f) {
		color.w = 1.0f;
	}
}

// タイマーが0になった場合の処理
if (timer <= 0) {
	bossFrag = false;
	color.w = 0.0f; // アルファ値をリセット
}

	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		finished_ = true;
	}
}

void GameOverScene::Draw()
{
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	Model::PreDraw(commandList);
	sprite_->Draw();

	Model::PostDraw();

}