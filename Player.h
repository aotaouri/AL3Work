#pragma once
#include "KamataEngine.h"
#include "WorldTransform.h"
#include "MyMath.h"

using namespace KamataEngine;

class MapChipField;

// マップとの当たり判定情報
struct CollisionMapInfo {
	bool top = false;
	bool stand = false;
	bool hitWall = false;
	Vector3 velocity;
};

class Enemy;

class Player {

public:
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const Vector3& position);

	void Update();

	void Draw();

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

	const KamataEngine::WorldTransform& GetWorldTransform() const { return worldTransform_; }

	const KamataEngine::Vector3& GetVelocity() const { return velocity_; }

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	//AABBを取得
	AABB GetAABB();

	// 衝突応答
	void OnCollision(const Enemy* enemy);

	// 方向を表すenum
	enum class LRDirection {
		kRight,
		kLeft,
	};

private:

	void Move();

	void MapCollision(struct CollisionMapInfo& info);

	void MapCollisionTop(struct CollisionMapInfo& info);    // 上方向
	void MapCollisionBottom(struct CollisionMapInfo& info); // 下方向
	void MapCollisionRight(struct CollisionMapInfo& info);  // 右方向
	void MapCollisionLeft(struct CollisionMapInfo& info);   // 左方向

	void UpdateGroundState(const CollisionMapInfo& info);

	void RVelocity(const CollisionMapInfo& info);

	void topTache(const CollisionMapInfo& info);

	void MapCollisionWall(const CollisionMapInfo& info);

	// 角
	enum Corner {
		kRightBottom, // 右下
		kLeftBottom,  // 左下
		kRightTop,    // 右上
		kLeftTop,     // 左上

		kNumCorner // 要素数

	};

	Vector3 CornerPosition(const Vector3& center, Corner corner);

	KamataEngine::WorldTransform worldTransform_;

	KamataEngine::Model* model_ = nullptr;

	KamataEngine::Camera* camera_ = nullptr;

	Vector3 velocity_ = {};

	static inline const float kAcceleration = 0.05f;

	static inline const float kAttenuation = 0.2f;

	// 着地時の速度減速
	static inline const float kAttenuationLanding = 0.2f;

	static inline const float kLimitRunSpeed = 0.5f;

	LRDirection lrDirection_ = LRDirection::kRight;

	float turnFirstRotationY_ = 0.0f;

	float turnTimer_ = 0.0f;

	static inline const float kTimeTurn = 0.3f;

	static inline const float kAttenuationWall = 0.2f;

	MapChipField* mapChipField_ = nullptr;

	//ジャンプ
	bool onGround_ = true;
	static inline const float kGravityAcceleration = 0.05f;
	static inline const float kLimitFallSpeed = 1.0f;
	static inline const float kJumpAcceleration = 1.0f;

    static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

	static inline const float kBlank = 0.0f;


};
