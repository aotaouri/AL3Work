#define NOMINMAX
#include "CameraController.h"
#include "KamataEngine.h"
#include "Player.h"
#include <algorithm>


float Lerp(float start, float end, float t) { return start + (end - start) * t; }

using namespace KamataEngine;

void CameraController::Initialize(KamataEngine::Camera* camera) {

	camera_ = camera;

	worldTransform_.Initialize();
}

void CameraController::Update()
{
	const KamataEngine::WorldTransform& targetWorldTransform = target_->GetWorldTransform();

	Vector3 targetVelocity = target_->GetVelocity();

	Vector3 targetPosition;
	targetPosition.x = targetWorldTransform.translation_.x + targetOffset_.x + targetVelocity.x * kVelocityBias;
	targetPosition.y = targetWorldTransform.translation_.y + targetOffset_.y + targetVelocity.y * kVelocityBias;
	targetPosition.z = targetWorldTransform.translation_.z + targetOffset_.z + targetVelocity.z * kVelocityBias;

	camera_->translation_.x = Lerp(camera_->translation_.x, targetPosition.x, kInterpolationRate);
	camera_->translation_.y = Lerp(camera_->translation_.y, targetPosition.y, kInterpolationRate);
	camera_->translation_.z = Lerp(camera_->translation_.z, targetPosition.z, kInterpolationRate);

	// X軸の制限
	camera_->translation_.x = (std::max)(camera_->translation_.x, movableArea_.left+kMargin.left);
	camera_->translation_.x = (std::min)(camera_->translation_.x, movableArea_.right + kMargin.right);
	// Y軸の制限
	camera_->translation_.y = (std::max)(camera_->translation_.y, movableArea_.bottom+kMargin.bottom);
	camera_->translation_.y = (std::min)(camera_->translation_.y, movableArea_.top+kMargin.top);

	camera_->UpdateMatrix();
}

void CameraController::Reset()
{
	const KamataEngine::WorldTransform& targetWorldTransform = target_->GetWorldTransform();

	
	camera_->translation_.x = targetWorldTransform.translation_.x + targetOffset_.x;
	camera_->translation_.y = targetWorldTransform.translation_.y + targetOffset_.y;
	camera_->translation_.z = targetWorldTransform.translation_.z + targetOffset_.z;

	// X軸の制限
	camera_->translation_.x = (std::max)(camera_->translation_.x, movableArea_.left + kMargin.left);
	camera_->translation_.x = (std::min)(camera_->translation_.x, movableArea_.right + kMargin.right);
	// Y軸の制限
	camera_->translation_.y = (std::max)(camera_->translation_.y, movableArea_.bottom + kMargin.bottom);
	camera_->translation_.y = (std::min)(camera_->translation_.y, movableArea_.top + kMargin.top);

	camera_->UpdateMatrix();
}