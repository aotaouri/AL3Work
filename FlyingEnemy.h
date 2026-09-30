#pragma once
#include "KamataEngine.h"
#include "MyMath.h"
#include "WorldTransform.h"

using namespace KamataEngine;

class Player;
class MapChipField;

class FlyingEnemy {
public:
	void Initialize(Model* model, KamataEngine::Camera* camera, const Vector3& position);
	void Update();
	void Draw();

	Vector3 GetWorldPosition() const;
	AABB GetAABB();

	void OnCollision(const Player*) {}
	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

private:
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera* camera_ = nullptr;
	MapChipField* mapChipField_ = nullptr;

	// サイズ（判定用）
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

	// 上下浮遊用のパラメータ
	float startY_ = 0.0f;    // 出現時のY座標（中心）
	float flyTimer_ = 0.0f;  // タイマー
	float amplitude_ = 2.0f; // 上下に揺れる振幅（片道2マス）
	float flyCycle_ = 2.0f;  // 1往復にかかる時間（秒）
};