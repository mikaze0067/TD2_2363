#include "Hoshi_Yokeyouya3D.h"
#include "TextureManager.h"
#include <cassert>
#include <PrimitiveDrawer.h>
#include <AxisIndicator.h>


Hoshi_Yokeyouya3D::Hoshi_Yokeyouya3D() {}

Hoshi_Yokeyouya3D::~Hoshi_Yokeyouya3D() {
	delete player3D_;
	delete enemy3D_;
	delete modelPlayer3D_;
	delete modelEnemy3D_;
	audio_->StopWave(voiceHandle);
}

void Hoshi_Yokeyouya3D::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	// ファイル名を指定してテクスチャを読み込む
	// 3Dモデルの生成
	modelPlayer3D_ = Model::CreateFromOBJ("ziki", true);
	modelEnemy3D_ = Model::CreateFromOBJ("hosi", true);
	modelHaikei_ = Model::CreateFromOBJ("Haikei", true);

	

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	// 自キャラの生成
	player3D_ = new Player3D();
	// 自キャラの初期化
	player3D_->Initialize(modelPlayer3D_, &viewProjection_);

	// 敵キャラの生成
	enemy3D_ = new Enemy3D();
	// 敵キャラの初期化
	enemy3D_->Initialize(modelEnemy3D_, &viewProjection_);

	// 天球の生成
	haikei_ = new Haikei();
	// 天球の初期化
	haikei_->Initialize(modelHaikei_, &viewProjection_);

	// サウンド
	soundDataHandle = audio_->LoadWave("playbgm.mp3");
	// 音声再生
	voiceHandle = audio_->PlayWave(soundDataHandle, true);
}

void Hoshi_Yokeyouya3D::Update() {
	// 天球の更新
	haikei_->Update();
	//自キャラの更新
	player3D_->Update();

	// 敵キャラの更新
	enemy3D_->Update();

	if (enemy3D_->IsFinished() == true || player3D_->IsFinished() == true) {
		finished_ = true;
	}

	//敵キャラに自キャラのアドレスを渡す
	enemy3D_->SetPlayer(player3D_);

	CheckAllCollision();

	HealthFlag_ = player3D_->IsHealth();
}

void Hoshi_Yokeyouya3D::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>l

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

	//自キャラの描画
	player3D_->Draw(viewProjection_);

	// 敵キャラの描画
	enemy3D_->Draw(viewProjection_);

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

	const std::list<PlayerBullet3D*>& playerBullets3D = player3D_->GetBullet();

	// 敵弾リストの取得
	const std::list<EnemyBullet3D*>& enemyBullets3D = enemy3D_->GetBullets();

	posA = enemy3D_->GetWorldPosition();

	#pragma region 自弾と敵キャラの当たり判定
	posA = enemy3D_->GetWorldPosition();
	for (PlayerBullet3D* bullet : playerBullets3D) {

		if (bullet->IsDead() == false) {
			posB = bullet->GetWorldPosition();

			if (((posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) + (posB.z - posA.z) * (posB.z - posA.z) <= (1.0f + 1.0f) * (1.0f + 1.0f))) {
				enemy3D_->OnCollision();
				bullet->OnCollision();
			}
		}
	}
	#pragma endregion

	#pragma region 自キャラと敵弾の当たり判定
	// 自キャラの座標
	posB = player3D_->GetWorldPosition();
	for (EnemyBullet3D* bullet : enemyBullets3D) {
		if (bullet->IsDead() == false) {
			// 敵弾の座標
			posA = bullet->GetWorldPosition();
			// 球と球の交差判定
			if (((posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) + (posB.z - posA.z) * (posB.z - posA.z) <= (1.0f + 1.0f) * (1.0f + 1.0f))) {
				// 自キャラの衝突時コールバックを呼び出す
				player3D_->OnCollision();
				// 敵弾の衝突時コールバックを呼び出す
				bullet->OnCollision();
			}
		}
	}
#pragma endregion
}

void Hoshi_Yokeyouya3D::GameOver() { finished_ = true; }

void Hoshi_Yokeyouya3D::GameClear() { finished_ = true; }