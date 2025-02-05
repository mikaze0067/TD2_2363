
#pragma once
#include "Player3D.h"
#include <cassert>
//#include <imgui.h>

void Player3D::Initialize(Model* model, ViewProjection* viewProjection) {
	// NULLポインタチェック
	assert(model);

	model_ = model;
	viewProjection_ = viewProjection;
	// ワールド変換の初期化
	worldTransform_.Initialize();
	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();


	isDead_ = false;

	hp_ = 1;

	worldTransform_.rotation_.y = 3.1412f/2.0f;
}

void Player3D::Update() {

	// デスフラグの立った弾を削除
	bullets3D_.remove_if([](PlayerBullet3D* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};

	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;

	// 押した方向で移動ベクトルを変更(左右)
	if (input_->PushKey(DIK_A)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_D)) {
		move.x += kCharacterSpeed;
	}
	// 押した方向で移動ベクトルを変更(上下)
	if (input_->PushKey(DIK_S)) {
		move.y -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_W)) {
		move.y += kCharacterSpeed;
	}

	// 座標移動（ベクトルの加算）
	worldTransform_.translation_ += move;

	// アフィン変換行列の作成
	//(MakeAffineMatrix：自分で作った数学系関数)
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	// 定数バッファに転送
	worldTransform_.TransferMatrix();

	const float kMoveLimitX = 34;
	const float kMoveLimitY = 18;

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	// 連射クールダウンを減少させる
	if (fireCooldown_ > 0) {
		fireCooldown_--;
	}

	if (input_->PushKey(DIK_SPACE) && fireCooldown_ <= 0) {
		// キャラクター攻撃処理
		Attack();
	}

	// 弾更新
	for (PlayerBullet3D* bullet : bullets3D_) {
		bullet->Update();
	}
}

void Player3D::Draw(ViewProjection& viewProjection) {
	// 3Dモデルを描画
	if (!isDead_) {
		model_->Draw(worldTransform_, viewProjection);
	}

	// 弾描画
	for (PlayerBullet3D* bullet : bullets3D_) {
		bullet->Draw(viewProjection);
	}
}

void Player3D::Attack() {
	const int kFireCooldownMax = 10; // 10フレームに1回撃てる

	// 弾の速度
	const float kBulletSpeed = 1.0f;
	Vector3 velocity(0, 0, kBulletSpeed);

	// 速度ベクトルを自機の向きに合わせて回転させる
	velocity = TransformNormal(velocity, worldTransform_.matWorld_);

	// 弾を生成し、初期化
	PlayerBullet3D* newBullet = new PlayerBullet3D();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);
	// 弾を登録する
	bullets3D_.push_back(newBullet);

	// クールダウンをリセット
	fireCooldown_ = kFireCooldownMax;
}

Player3D::~Player3D() {
	for (PlayerBullet3D* bullet : bullets3D_) {
		delete bullet;
	}
}

Vector3 Player3D::GetWorldPosition() {
	/// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;
	return worldPos;
}

void Player3D::OnCollision() {
	hp_--;

	if (hp_ <= 0) {
		hp_ = 0;
		isDead_ = true;
		HealthFlag_ = true;
		finished_ = true;
	}
}
