#include "Skydome.h"

void Skydome::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera) {
	modelSkydome_ = model;

	camera_ = camera;

	worldTransform_.Initialize();
}

void Skydome::Update() { worldTransform_.TransferMatrix(); }

void Skydome::Draw()
{
	KamataEngine::Model::PreDraw();

	modelSkydome_->Draw(worldTransform_, *camera_);

	KamataEngine::Model::PostDraw();
}
