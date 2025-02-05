#include "EnemyBullet3D.h"
#include "MathUtilityForText.h"
#include <TextureManager.h>

void EnemyBullet3D::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	// NULLポインタチェック
	assert(model);

	model_ = model;
	textureHandle_ = TextureManager::Load("Blue.png");
	// ワールド変換の初期化
	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
	// 引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;

	worldTransform_.rotation_.y = 3.1412f / 2.0f;
}

void EnemyBullet3D::Update() {
	// 時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	// 座標を移動させる（1フレーム分の移動量を足しこむ）
	worldTransform_.translation_ += velocity_;

	// ワールド変換の更新
	worldTransform_.UpdateMatrix();

}

void EnemyBullet3D::Draw(const ViewProjection& viewProjection) { model_->Draw(worldTransform_, viewProjection,textureHandle_); }

void EnemyBullet3D::OnCollision() { isDead_ = true; }

Vector3 EnemyBullet3D::GetWorldPosition() { /// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;
	return worldPos;
}
