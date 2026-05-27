#include "Player.h"
#include "KamataEngine.h"
#include "MyMath.h"
#include "Matrix4x4.h"

void Player::Initialize(KamataEngine::Model* model, uint32_t textureHandle, KamataEngine::Camera* camera) {

	model_ = model;

	textureHandle_ = textureHandle;

	camera_ = camera;

	worldTransform_.Initialize();
}

void Player::Update() {

	// 各種行列
	KamataEngine::Matrix4x4 scaleMatrix = MakeScaleMatrix(worldTransform_.scale_);
	KamataEngine::Matrix4x4 rotationMatrix = MakeRotationMatrix(worldTransform_.rotation_);
	KamataEngine::Matrix4x4 translationMatrix = MakeTranslateMatrix(worldTransform_.translation_);

	worldTransform_.matWorld_ = Multiply(scaleMatrix, rotationMatrix);
	worldTransform_.matWorld_ = Multiply(worldTransform_.matWorld_, translationMatrix);

	//行列を定数バッファに転送
	worldTransform_.TransferMatrix();
	
}

void Player::Draw() 
{
	model_->PreDraw();

	model_->Draw(worldTransform_, *camera_, textureHandle_);

	model_->PostDraw();

}

