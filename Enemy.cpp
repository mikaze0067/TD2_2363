#include "Enemy.h"
#include <cassert>
#include "MathUtilityForText.h"
#include <Player.h>
#include <TextureManager.h>




void Enemy::Initialize(Model* model, ViewProjection*viewProjection) {
	// NULLポインタチェック
	assert(model);
	dxCommon_ = DirectXCommon::GetInstance();
	model_ = model;
	viewProjection_ = viewProjection;
	// ワールド変換の初期化
	worldTransform_.Initialize();
	isDead_ = false;

	hp_ = 30;
	fireCount_ = 0; // 発射カウンタを初期化

	// HPのリソースを格納
	for (int i = 1; i <= 30; i++) {
		std::string filePath = "HP/enemyHp" + std::to_string(i) + ".png";
		uint32_t handle = TextureManager::Load(filePath);
		hpSprites_.push_back(Sprite::Create(handle, {950, 30}));
	}
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
	
	worldTransform_.translation_.x = 30.0f;

	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};

	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;

	move.y -= kCharacterSpeed;
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
	Vector3 Amove = {0.0f, 0.8f, 0.0f};
	Vector3 Lmove = {0.0f, 0.8f, 0.0f};
	switch (phase_) {
	case Phase::Approach:
	default:
		phaseName = "Approach";

		// 移動（ベクトルを加算）
		worldTransform_.translation_ += Amove;
		// 既定の位置に到達したら離脱
		if (worldTransform_.translation_.y > 10.0f) {
			phase_ = Phase::Leave;
		}
		break;
	case Phase::Leave:
		phaseName = "Leave";

		// 移動（ベクトルを加算）
		worldTransform_.translation_ -= Lmove;
		// 既定の位置に到達したら離脱
		if (worldTransform_.translation_.y < -8.0f) {
			phase_ = Phase::Approach;
		}
	}
}

void Enemy::Draw(ViewProjection& viewProjection) {
	// 3Dモデルを描画
	if (isDead_ == false) {
		model_->Draw(worldTransform_, viewProjection);
	}
	// 弾描画
	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
		
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	/// 
	
	if (hp_ > 0 && hp_ <= 30) {
		hpSprites_[hp_ - 1]->Draw();
	}

	// スプライト描画後処理
	Sprite::PostDraw();
}

void Enemy::Fire() {
	// 通常の弾の速度
	const float kNormalBulletSpeed = 0.5f;
	// 速い弾の速度
	const float kFastBulletSpeed = 1.0f;
	// 一度に発射する弾の数
	const int kBulletsPerFire = 4; // 一度に発射する弾の数
	// 速い弾が発射される確率（0.0f ～ 1.0f）
	const float kFastBulletProbability = 0.3f; // 30%の確率で速い弾

	// 弾の初期位置のランダム範囲
	const float kRandomRangeX = 5.0f;  // X軸のランダム範囲
	const float kRandomRangeY = 20.0f; // Y軸のランダム範囲

	for (int i = 0; i < kBulletsPerFire; ++i) {
		// ランダムな初期位置を計算
		float randomX = (static_cast<float>(rand()) / RAND_MAX) * 2.0f * kRandomRangeX - kRandomRangeX;
		float randomY = (static_cast<float>(rand()) / RAND_MAX) * 2.0f * kRandomRangeY - kRandomRangeY;
		Vector3 bulletPosition(worldTransform_.translation_.x + randomX, worldTransform_.translation_.y + randomY, worldTransform_.translation_.z);

		// ランダムに速い弾かどうかを決定
		float randomValue = static_cast<float>(rand()) / RAND_MAX; // 0.0f ～ 1.0f のランダム値
		float bulletSpeed = (randomValue < kFastBulletProbability) ? kFastBulletSpeed : kNormalBulletSpeed;

		// 発射方向は固定（Z軸方向にまっすぐ）
		Vector3 velocity(-bulletSpeed, 0.0f, 0.0f);

		// 弾を生成し、初期化
		EnemyBullet* newBullet = new EnemyBullet();
		newBullet->Initialize(model_, bulletPosition, velocity);

		// 弾を登録する
		bullets_.push_back(newBullet);
	}

	// 発射カウンタを更新
	fireCount_++;
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

void Enemy::OnCollision() {
	hp_--;

	if (hp_ <= 0) {
		hp_ = 0;
		isDead_ = true;
		finished_ = true;
	}
}

