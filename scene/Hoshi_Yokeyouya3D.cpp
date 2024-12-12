#include "Hoshi_Yokeyouya3D.h"
#include "TextureManager.h"
#include <cassert>
#include <ImGuiManager.h>
#include <PrimitiveDrawer.h>
#include <AxisIndicator.h>


Hoshi_Yokeyouya3D::Hoshi_Yokeyouya3D() {}

Hoshi_Yokeyouya3D::~Hoshi_Yokeyouya3D() {
	delete model_;
	delete player_;
	delete debugCamera_;
	delete enemy_;
}

void Hoshi_Yokeyouya3D::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg ");
	textureHandleEnemy_ = TextureManager::Load("Blue.png ");

	// 3Dモデルの生成
	model_ = Model::Create();
	modelEnemy_ = Model::Create();

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();


	// 自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	player_->Initialize(model_,textureHandle_);

	// 敵キャラの生成
	enemy_ = new Enemy();
	// 敵キャラの初期化
	enemy_->Initialize(modelEnemy_, textureHandleEnemy_);

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
}

void Hoshi_Yokeyouya3D::Update() {
	//自キャラの更新
	player_->Update();

	// 敵キャラの更新
	enemy_->Update();

	//敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);

	CheckAllCollision();
	#ifdef _DEBUG
	if (input_->TriggerKey(DIK_0)) {
		isDebugCameraActive_ = true;
	}
	#endif
	//カメラの処理
	if (isDebugCameraActive_) {
		// デバッグカメラの更新
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		//ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {
		// ビュープロジェクション行列の更新と転送
		viewProjection_.UpdateMatrix();
	}
	// キャラクターの座標を画面表示する処理
	ImGui::Begin("Scene");

	ImGui::Text("kHoshi_Yokeyouya3D"); // フェーズ名を表示

	ImGui::End();
}

void Hoshi_Yokeyouya3D::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

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

	//自キャラの描画
	player_->Draw(viewProjection_);

	// 敵キャラの描画
	enemy_->Draw(viewProjection_);

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

void Hoshi_Yokeyouya3D::CheckAllCollision() {
	Vector3 posA, posB;

	int count = 0;

	const std::list<PlayerBullet*>& playerBullets = player_->GetBullet();

	posA = enemy_->GetWorldPosition();

	for (PlayerBullet* bullet : playerBullets) {

		if (bullet->IsDead() == false) {
			posB = bullet->GetWorldPosition();

			if (((posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) + (posB.z - posA.z) * (posB.z - posA.z) <= (1.0f + 1.0f) * (1.0f + 1.0f))) {
				count++;
				if (count <= 50) {
					enemy_->OnCollision();
					bullet->OnCollision();
				}
			}
		}
	}
}

void Hoshi_Yokeyouya3D::GameOver() { finished_ = true; }

void Hoshi_Yokeyouya3D::GameClear() { finished_ = true; }
