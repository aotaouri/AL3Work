#define NOMINMAX
#include "FlyingEnemy.h"
#include "KamataEngine.h"
#include "MapChipField.h"
#include "Matrix4x4.h"
#include "MyMath.h"
#include "WorldTransform.h"
#include <cmath>
#include <numbers>

void FlyingEnemy::Initialize(Model* model, KamataEngine::Camera* camera, const Vector3& position) {
	model_ = model;
	camera_ = camera;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};

	startY_ = position.y; // 初期の高さ（揺れの中心）を記憶
	flyTimer_ = 0.0f;
}

void FlyingEnemy::Update() {
	// 1. タイマー加算
	flyTimer_ += 1.0f / 60.0f;

	// 2. sin関数を使ってY座標をなめらかに変化させる
	float sineValue = std::sin(2.0f * std::numbers::pi_v<float> * flyTimer_ / flyCycle_);
	worldTransform_.translation_.y = startY_ + (sineValue * amplitude_);

	// 3. 行列更新
	KamataEngine::Matrix4x4 scaleMatrix = MakeScaleMatrix(worldTransform_.scale_);
	KamataEngine::Matrix4x4 rotationMatrix = MakeRotationMatrix(worldTransform_.rotation_);
	KamataEngine::Matrix4x4 translationMatrix = MakeTranslateMatrix(worldTransform_.translation_);

	worldTransform_.matWorld_ = Multiply(scaleMatrix, rotationMatrix);
	worldTransform_.matWorld_ = Multiply(worldTransform_.matWorld_, translationMatrix);

	worldTransform_.TransferMatrix();
}

Vector3 FlyingEnemy::GetWorldPosition() const {
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

AABB FlyingEnemy::GetAABB() {
	Vector3 worldPos = GetWorldPosition();
	AABB aabb;
	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};
	return aabb;
}

void FlyingEnemy::Draw() {

	model_->PreDraw();
	model_->Draw(worldTransform_, *camera_);
	model_->PostDraw();
}