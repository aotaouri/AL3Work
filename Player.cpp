#include "Player.h"
#include "KamataEngine.h"

void Player::Initialize(KamataEngine::Model* model, uint32_t textureHandle, KamataEngine::Camera* camera) {

	model_ = model;

	textureHandle_ = textureHandle;

	camera_ = camera;

	worldTransform_.Initialize();
}

void Player::Update() {
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
}

void Player::Draw() 
{
	model_->PreDraw();

	model_->Draw(worldTransform_, *camera_, textureHandle_);

	model_->PostDraw();

}

