#include "Enemy.h"
#include <cassert>
#include "MathUtilityForText.h"
#include <imgui.h>
#include <Player.h>


void Enemy::Initialize(Model* model, uint32_t textureHandle) {
	// NULLポインタチェック
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;
	// ワールド変換の初期化
	worldTransform_.Initialize();
	
}

void Enemy::Update() {

	// デスフラグの立った弾を削除
	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
	
	worldTransform_.translation_.z = 10.0f;

	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};

	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;

	move.x -= kCharacterSpeed;
	//move.x += kCharacterSpeed;
	//move.y -= kCharacterSpeed;
	//move.y += kCharacterSpeed;
	//move.z -= kCharacterSpeed;
	//move.z += kCharacterSpeed;
	
	// 座標移動（ベクトルの加算）
	//worldTransform_.translation_ += move;

	// アフィン変換行列の作成
	//(MakeAffineMatrix：自分で作った数学系関数)
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	const float kMoveLimitX = 34;
	const float kMoveLimitY = 18;

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();

	// 弾を発射
	// Fire();
	Approach();

	// 弾更新
	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}

	const char* phaseName = "Approach";
	Vector3 Amove = {0.2f, 0.0f, 0.0f};
	Vector3 Lmove = {0.2f, 0.0f, 0.0f};
	switch (phase_) {
	case Phase::Approach:
	default:
		phaseName = "Approach";

		// 移動（ベクトルを加算）
		worldTransform_.translation_ += Amove;
		// 既定の位置に到達したら離脱
		if (worldTransform_.translation_.x > 20.0f) {
			phase_ = Phase::Leave;
		}
		break;
	case Phase::Leave:
		phaseName = "Leave";

		// 移動（ベクトルを加算）
		worldTransform_.translation_ -= Lmove;
		// 既定の位置に到達したら離脱
		if (worldTransform_.translation_.x < -20.0f) {
			phase_ = Phase::Approach;
		}
	}
	// キャラクターの座標を画面表示する処理
	ImGui::Begin("Player");

	ImGui::Text("Current Phase: %s", phaseName); // フェーズ名を表示

	ImGui::End();
}

void Enemy::Draw(ViewProjection& viewProjection) {
	// 3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	// 弾描画
	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Enemy::Fire() {
	assert(player_);

	// 弾の速度(調整項目)
	const float kBulletSpeed = -1.0f;

	Vector3 velocity(0, 0, kBulletSpeed);
	// 速度ベクトルを自機の向きに合わせて回転させる
	velocity = TransformNormal(velocity, worldTransform_.matWorld_);
	
	//自キャラの座標を取得する
	Vector3 playerPos = player_->GetWorldPosition();
	//敵キャラの座標を取得する
	Vector3 enemyPos = Enemy::GetWorldPosition();
	//敵キャラから自キャラへと差別ベクトルを求める
	Vector3 direction = enemyPos - playerPos;
	//ベクトル正規化
	Vector3 normalizedDirection = Normalize(direction);
	//ベクトルの長さを、速さに合わせる
	velocity = normalizedDirection * kBulletSpeed;

	// 弾を生成し、初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);
	// 弾を登録する
	bullets_.push_back(newBullet);
}

Enemy::~Enemy() {
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Enemy::Approach() {
	//発射タイマーカウントダウン
	fireTimer++;
	//指定時間に達した
	if (fireTimer == kFireInterval) {
		// 弾を発射
		Fire();
		// 発射タイマー
		fireTimer = 0;
	}
}

Vector3 Enemy::GetWorldPosition() {
	/// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;
	return worldPos;
}
