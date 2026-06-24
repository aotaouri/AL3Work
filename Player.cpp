#define NOMINMAX
#include "Player.h"
#include "KamataEngine.h"
#include "MyMath.h"
#include "Matrix4x4.h"
#include <numbers>
#include <algorithm>
#include "WorldTransform.h"

using namespace KamataEngine;

void Player::Initialize(KamataEngine::Model* model,KamataEngine::Camera* camera, const Vector3& position) {

	model_ = model;

	camera_ = camera;
	//ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
}

void Player::Update() 
{

		// 移動入力
		// 左右移動操作
		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {

			// 左右加速
			Vector3 acceleration = {};
			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
				// 左入力中の右入力
				if (velocity_.x < 0.0f) {
					// 速度と逆方向に入力中は急ブレーキ
					velocity_.x *= (1.0f - kAcceleration);
				}

				acceleration.x += kAcceleration;

				if (lrDirection_ != LRDirection::kRight) {
					lrDirection_ = LRDirection::kRight;

					turnFirstRotationY_ = worldTransform_.rotation_.y;

					turnTimer_ = kTimeTurn;
				}

			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
				// 左入力中の右入力
				if (velocity_.x > 0.0f) {
					// 速度と逆方向に入力中は急ブレーキ
					velocity_.x *= (1.0f - kAcceleration);
				}

				acceleration.x -= kAcceleration;

				if (lrDirection_ != LRDirection::kLeft) {
					lrDirection_ = LRDirection::kLeft;

					turnFirstRotationY_ = worldTransform_.rotation_.y;

					turnTimer_ = kTimeTurn;
				}
			}
			// 加速/原則
			velocity_.x += acceleration.x;
			velocity_.y += acceleration.y;
			velocity_.z += acceleration.z;

			velocity_.x = std::clamp(velocity_.x,-kLimitRunSpeed, kLimitRunSpeed);

		}
		else
		{
			velocity_.x *= (1.0f - kAttenuation);
		}

		if (onGround_) {

		    // ジャンプ
		    if (Input::GetInstance()->PushKey(DIK_UP)) {
			    velocity_.y += kJumpAcceleration;
		    }
	    }else 
	    {
		velocity_.y +=-kGravityAcceleration;

		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
	    }

	bool landing = false;
	if (velocity_.y<0)
	{
		if (worldTransform_.translation_.y <= 1.0f)
		{
			landing = true;
		}
	}

	if (onGround_)
	{
		if (velocity_.y > 0.0f)
		{
			onGround_ = false;
		}
	}else{
		if (landing)
		{
			worldTransform_.translation_.y = 1.0f;

			velocity_.x *= (1.0f - kAttenuation);

			velocity_.y = 0.0f;

			onGround_ = true;

		}
	}

	////移動
	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;

	// 旋回制御
	if (kTimeTurn > 0.0f) 
	{
		turnTimer_ -= 1.0f / 60.0f;

		float destinationRotationYTable[] = {
			std::numbers::pi_v<float> / 2.0f,
			std::numbers::pi_v<float> * 3.0f / 2.0f
		};

		// 状態に応じた角度を取得する
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		// 自キャラの角度を設定する
		worldTransform_.rotation_.y = destinationRotationY;
	}

	KamataEngine::Matrix4x4 scaleMatrix = MakeScaleMatrix(worldTransform_.scale_);
	KamataEngine::Matrix4x4 rotationMatrix = MakeRotationMatrix(worldTransform_.rotation_);
	KamataEngine::Matrix4x4 translationMatrix = MakeTranslateMatrix(worldTransform_.translation_);

	worldTransform_.matWorld_ = Multiply(scaleMatrix, rotationMatrix);
	worldTransform_.matWorld_ = Multiply(worldTransform_.matWorld_, translationMatrix);

	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
	
}

void Player::Draw() 
{
	model_->PreDraw();

	model_->Draw(worldTransform_, *camera_);

	model_->PostDraw();

}

