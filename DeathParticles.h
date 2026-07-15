#pragma once
#include "KamataEngine.h"
#include <array>
#include <numbers>

class DeathParticles 
{
public:

	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);

	void Update();

	void Draw();

private:

	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera* camera_ = nullptr;

	//パーティクルの個数
	static inline const uint32_t kNumParticles = 8;
	std::array<KamataEngine::WorldTransform, kNumParticles> worldTransform_;

	//存続時間(消滅までの時間)<秒>
	static inline const float kDuration = 1.0f;
	//移動の速さ
	static inline const float kSpeed = 0.1f;
	//分割した1個分の角度
	static inline const float kAngleUnit = (2.0f* std::numbers::pi_v<float>) / 8.0f;
	//終了フラグ
	bool isFinished_ = false;
	//経過時間カウント
	float counter_ = 0.0f;

	//色変更オブジェクト
	KamataEngine::ObjectColor objectColor_;
	//色の数値
	KamataEngine::Vector4 color_;

};
