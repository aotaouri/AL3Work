#include "DeathParticles.h"
#include "MyMath.h"
#include<algorithm>

using namespace KamataEngine;

void DeathParticles::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position) 
{
	model_ = model;
	camera_ = camera;

	//ワールド変換の初期化
	for (WorldTransform& worldTransform : worldTransform_)
	{
		worldTransform.Initialize();
		worldTransform.translation_ = position;
	}

	objectColor_.Initialize();
	color_ = {1, 1, 1, 1};

}

void DeathParticles::Update()
{
	// 終了なら何もしない
	if (isFinished_) {
		return;
	}

	// カウンターを1フレーム分の秒数進める;
	counter_ += 1.0f / 60.0f;

	// 存続時間の上限に達したら
	if (counter_ >= kDuration) {
		counter_ = kDuration;
		// 終了扱いにする
		isFinished_ = true;
	}

	color_.w = std::clamp(1.0f - (counter_ / kDuration), 0.0f, 1.0f);
	objectColor_.SetColor(color_);

	for (int i = 0; i < kNumParticles; ++i)
	{
		//基本となる速度ベクトル
		Vector3 velocity = {kSpeed, 0.0f, 0.0f};
		//回転角を計算する
		float angle = kAngleUnit * i;
		//Z軸まわり回転行列
		Matrix4x4 matrixRotation = MakeTranslateZMatrix(angle);
		//基本ベクトルを回転させて速度ベクトルを得る
		velocity = Transform(velocity, matrixRotation);
		//移動処理
		worldTransform_[i].translation_.x += velocity.x;
		worldTransform_[i].translation_.y += velocity.y;
		worldTransform_[i].translation_.z += velocity.z;
	}

	for (WorldTransform& worldTransform : worldTransform_) 
	{
		KamataEngine::Matrix4x4 scaleMatrix = MakeScaleMatrix(worldTransform.scale_);
		KamataEngine::Matrix4x4 rotationMatrix = MakeRotationMatrix(worldTransform.rotation_);
		KamataEngine::Matrix4x4 translationMatrix = MakeTranslateMatrix(worldTransform.translation_);

		worldTransform.matWorld_ = Multiply(scaleMatrix, rotationMatrix);
		worldTransform.matWorld_ = Multiply(worldTransform.matWorld_, translationMatrix);

		// 定数バッファへの書き込み
		worldTransform.TransferMatrix();
	}

}

void DeathParticles::Draw() 
{

	// 終了なら何もしない
	if (isFinished_) {
		return;
	}

	for (WorldTransform& worldTransform : worldTransform_)
	{
			model_->Draw(worldTransform, *camera_,&objectColor_);
	}

}