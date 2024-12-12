#include "Hoshi_Yokeyouya.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include <ImGuiManager.h>


Hoshi_Yokeyouya::Hoshi_Yokeyouya() {}

Hoshi_Yokeyouya::~Hoshi_Yokeyouya() {
	delete model_;
	delete player_;
	delete debugCamera_;
	delete enemy_;
}

void Hoshi_Yokeyouya::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg ");
	textureHandleEnemy_ = TextureManager::Load("Blue.png ");

	// 3Dモデルの生成
	model_ = Model::Create();
	modelEnemy_ = Model::Create();
	modelHaikei_ = Model::CreateFromOBJ("Haikei", true);

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	// 自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	player_->Initialize(model_, textureHandle_);

	// 敵キャラの生成
	enemy_ = new Enemy();
	// 敵キャラの初期化
	enemy_->Initialize(modelEnemy_, textureHandleEnemy_);

	// 天球の生成
	haikei_ = new Haikei();
	// 天球の初期化
	haikei_->Initialize(modelHaikei_, &viewProjection_);

	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
}

void Hoshi_Yokeyouya::Update() {
	// 天球の更新
	haikei_->Update();
	//自キャラの更新
	player_->Update();

	// 敵キャラの更新
	enemy_->Update();

	// 敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);

	CheckAllCollision();
	// キャラクターの座標を画面表示する処理
	ImGui::Begin("Scene");

	ImGui::Text("kHoshi_Yokeyouya"); // フェーズ名を表示

	ImGui::End();
}

void Hoshi_Yokeyouya::Draw() {// コマンドリストの取得
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

	// 天球の描画
	haikei_->Draw();

	// 自キャラの描画
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

void Hoshi_Yokeyouya::CheckAllCollision() {
	Vector3 posA, posB;

	int count = 0;

	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullet();
	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

#pragma region 自弾と敵キャラの当たり判定
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
#pragma endregion

#pragma region 自キャラと敵弾の当たり判定
	// 自キャラの座標
	posB = player_->GetWorldPosition();
	for (EnemyBullet* bullet : enemyBullets) {
		// 敵弾の座標
		posA = bullet->GetWorldPosition();
		// 球と球の交差判定
		if (((posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) + (posB.z - posA.z) * (posB.z - posA.z) <= (1.0f + 1.0f) * (1.0f + 1.0f))) {
			// 自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			// 敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}
#pragma endregion
}

void Hoshi_Yokeyouya::GameOver() { finished_ = true; }

void Hoshi_Yokeyouya::GameClear() { finished_ = true; }
