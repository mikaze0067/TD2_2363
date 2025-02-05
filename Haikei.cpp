#include "Haikei.h"

void Haikei::Initialize(Model* model, ViewProjection* viewProjection) {
	worldTransform_.Initialize();
	model_ = model;
	viewProjection_ = viewProjection;
}

void Haikei::Update() {
	worldTransform_.UpdateMatrix();
	worldTransform_.rotation_.y -= 0.02f;
	worldTransform_.translation_.y = -15.0f;
}

void Haikei::Draw() {
	// 3Dモデル描画
	model_->Draw(worldTransform_, *viewProjection_);
}