#pragma once
#include "KamataEngine.h"
#include "WorldTransform.h"
#include "MyMath.h"

using namespace KamataEngine;

// 前方宣言
class Player;

class Enemy {

public:
	void Initialize(Model* model, KamataEngine::Camera* camera, const Vector3& position);

	void Update();

	void Draw();

	Vector3 GetWorldPosition() const;

	AABB GetAABB();

	// 衝突応答
	void OnCollision(const Player* player);

private:
	KamataEngine::WorldTransform worldTransform_;

	KamataEngine::Model* model_ = nullptr;

	KamataEngine::Camera* camera_ = nullptr;

	static inline const float kWalkSpeed = 0.03f;

	static inline const float kWalkMotionAngleStart = 30.0f;
	static inline const float kWalkMotionAngleEnd = -30.0f;
	static inline const float kWalkMotionTime = 1.0f;

	 static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

	Vector3 velocity_ = {};

	//経過時間
	float walkTimer_ = 0.0f;

};
