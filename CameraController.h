#pragma once
#include "KamataEngine.h"

using namespace KamataEngine;

struct Rect {
	float left = 0.0f;
	float right = 1.0f;
	float bottom = 1.0f;
	float top = 1.0f;
};

class Player;

class CameraController {

public:
	void Initialize(KamataEngine::Camera* camera);

	void Update();

	void SetTarget(Player* target) { target_ = target; }

	void Reset();

	void SetMovableArea(const Rect& area) { movableArea_ = area; }

private:
	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;

	// カメラ
	KamataEngine::Camera* camera_;

	Player* target_ = nullptr;

	// 追従対象とカメラの座標の差(オフセット)
	Vector3 targetOffset_ = {0, 0, -15.0f};

	Rect movableArea_ = {0, 100, 0, 100};

	KamataEngine::Vector3 targetPosition_ = {};

	static inline const float kInterpolationRate = 0.1f;

	static inline const float kVelocityBias = 0.2f;

	static inline const Rect kMargin = {-4.0f, 4.0f, -2.0f, 5.0f};

};
