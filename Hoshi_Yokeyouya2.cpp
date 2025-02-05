#include "Hoshi_Yokeyouya2.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include <ImGuiManager.h>


Hoshi_Yokeyouya2::Hoshi_Yokeyouya2() {}

Hoshi_Yokeyouya2::~Hoshi_Yokeyouya2() {
	delete modelPlayer_;
	delete player_;
	delete debugCamera_;
	delete enemy_;
	audio_->StopWave(voiceHandle);
}

void Hoshi_Yokeyouya2::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg ");
	textureHandleEnemy_ = TextureManager::Load("Blue.png ");
	modelHaikei_ = Model::CreateFromOBJ("Haikei", true);

	// 3Dモデルの生成
	
	modelPlayer_ = Model::CreateFromOBJ("ziki",true);
	modelEnemy_ = Model::CreateFromOBJ("hosi",true);

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	// 自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	player_->Initialize(modelPlayer_, &viewProjection_);

	// 敵キャラの生成
	enemy_ = new Enemy();
	// 敵キャラの初期化
	enemy_->Initialize(modelEnemy_,&viewProjection_);

	// 天球の生成
	haikei_ = new Haikei();
	// 天球の初期化
	haikei_->Initialize(modelHaikei_, &viewProjection_);

	//サウンド
	soundDataHandle=audio_->LoadWave("GameBGM.mp3");
	//音声再生
	voiceHandle=audio_->PlayWave(soundDataHandle,true);

	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
}

void Hoshi_Yokeyouya2::Update() {

	// 天球の更新
	haikei_->Update();
	// プレイヤーの更新
	player_->Update();

	// 敵キャラの更新
	enemy_->Update();

	//audio_->StopWave(voiceHandle);

	if (enemy_->IsFinished() == true || player_->IsFinished() == true) {
		finished_ = true;
	}

	HealthFlag_ = player_->IsHealth();

	// 敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);

	CheckAllCollision();
	
}

void Hoshi_Yokeyouya2::Draw() {
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

void Hoshi_Yokeyouya2::CheckAllCollision() {
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

void Hoshi_Yokeyouya2::GameOver() { finished_ = true; }

void Hoshi_Yokeyouya2::GameClear() { finished_ = true; }
