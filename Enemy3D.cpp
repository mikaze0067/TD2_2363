#include "Enemy3D.h"
#include "MathUtilityForText.h"
#include <Player3D.h>
#include <cassert>
#include <TextureManager.h>

void Enemy3D::Initialize(Model* model, ViewProjection* viewProjection) {
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


	// HPバーのスプライトをロードしてリストに格納
	for (int i = 1; i <= 30; i++) {
		std::string filePath = "HP/enemyHp" + std::to_string(i) + ".png";
		uint32_t handle = TextureManager::Load(filePath);
		hpSprites3D_.push_back(Sprite::Create(handle, {800, 30}));
	}
}

void Enemy3D::Update() {

	// デスフラグの立った弾を削除
	bullets3D_.remove_if([](EnemyBullet3D* bullet3D) {
		if (bullet3D->IsDead()) {
			delete bullet3D;
			return true;
		}
		return false;
	});

	worldTransform_.translation_.z = 20.0f;


	
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

	worldTransform_.rotation_.y += 3.1412f / 2.0f;
	worldTransform_.rotation_.z += 3.1412f / 2.0f;


	// 弾を発射
	// Fire();
	Approach();

	// 弾更新
	for (EnemyBullet3D* bullet : bullets3D_) {
		bullet->Update();
	}

	const char* phaseName = "Approach";
	// ランダムな移動ベクトルを作成
	float randomX = (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 2.0f; // -1.0 ~ +1.0
	float randomY = (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 2.0f; // -1.0 ~ +1.0

	Vector3 Amove = {randomX, randomY, 0.0f};   // XとY方向にランダム移動
	Vector3 Lmove = {-randomX, -randomY, 0.0f}; // 逆方向に戻る
	switch (phase3D_) {
	case Phase3D::Approach:
	default:
		phaseName = "Approach";

		// 移動（ベクトルを加算）
		worldTransform_.translation_ += Amove;
		// 既定の位置に到達したら離脱
		if (worldTransform_.translation_.x > 10.0f || worldTransform_.translation_.y > 10.0f) {
			phase3D_ = Phase3D::Leave;
		}
		break;
	case Phase3D::Leave:
		phaseName = "Leave";

		// 移動（ベクトルを加算）
		worldTransform_.translation_ -= Lmove;
		// 既定の位置に到達したら再び接近
		if (worldTransform_.translation_.x < -10.0f || worldTransform_.translation_.y < -10.0f) {
			phase3D_ = Phase3D::Approach;
		}
	}
}

void Enemy3D::Draw(ViewProjection& viewProjection) {
	// 3Dモデルを描画
	if (isDead_ == false) {
		model_->Draw(worldTransform_, viewProjection);
	}
	// 弾描画
	for (EnemyBullet3D* bullet : bullets3D_) {
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
		hpSprites3D_[hp_ - 1]->Draw();
	}

	// スプライト描画後処理
	Sprite::PostDraw();
}

void Enemy3D::Fire() {
	// 通常の弾の速度
	const float kNormalBulletSpeed = 1.0f;
	// 速い弾の速度
	const float kFastBulletSpeed = 3.0f;
	// 速い弾を発射する間隔
	const int kFastBulletInterval = 3;

	// 弾の初期位置を画面奥に設定
	const float kOutsideZ = 60.0f;     // Z軸の画面外位置
	const float kRandomRangeY = 10.0f; // Y軸のランダム範囲
	const float kRandomRangeX = 15.0f; // X軸のランダム範囲

	// ランダムな初期位置を計算
	float randomX = (static_cast<float>(rand()) / RAND_MAX) * 2.0f * kRandomRangeX - kRandomRangeX;
	float randomY = (static_cast<float>(rand()) / RAND_MAX) * 2.0f * kRandomRangeY - kRandomRangeY;
	Vector3 bulletPosition(randomX, randomY, kOutsideZ);

	// 発射カウンタに基づいて弾の速度を決定
	float bulletSpeed = (fireCount_ % kFastBulletInterval == 0) ? kFastBulletSpeed : kNormalBulletSpeed;

	// 弾の初期速度ベクトル（-Z方向に飛ぶ）
	Vector3 velocity(0.0f, 0.0f, -bulletSpeed);

	// 弾を生成し、初期化
	EnemyBullet3D* newBullet3D = new EnemyBullet3D();
	newBullet3D->Initialize(model_, bulletPosition, velocity);

	// 弾を登録する
	bullets3D_.push_back(newBullet3D);

	// 発射カウンタを更新
	fireCount_++;
}

Enemy3D::~Enemy3D() {
	for (EnemyBullet3D* bullet : bullets3D_) {
		delete bullet;
	}
}

void Enemy3D::Approach() {
	// 発射タイマーカウントダウン
	fireTimer++;
	// 指定時間に達した
	if (fireTimer == kFireInterval) {
		// 弾を発射
		Fire();
		// 発射タイマー
		fireTimer = 0;
	}
}

Vector3 Enemy3D::GetWorldPosition() {
	/// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;
	return worldPos;
}

void Enemy3D::OnCollision() {
	hp_--;

	if (hp_ <= 0) {
		hp_ = 0;
		isDead_ = true;
		finished_ = true;
	}
}
