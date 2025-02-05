#include "TitleModel.h"

void TitleModel::Initialize(Model* model, ViewProjection* viewProjection)
{
	worldTransform_.Initialize();
	model_ = model;
	viewProjection_ = viewProjection;
}

void TitleModel::Update()
{
}

void TitleModel::Draw()
{
	// 3Dモデル描画
	model_->Draw(worldTransform_, *viewProjection_);
}
