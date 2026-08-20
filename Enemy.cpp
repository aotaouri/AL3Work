#define NOMINMAX
#include "Enemy.h"
#include <cmath>
#include "KamataEngine.h"
#include "MapChipField.h"
#include "Matrix4x4.h"
#include "MyMath.h"
#include "WorldTransform.h"
#include <algorithm>
#include <array>
#include <numbers>

void Enemy::Initialize(Model* model, KamataEngine::Camera* camera, const Vector3& position) { 
	model_ = model;

	camera_ = camera;

	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform_.rotation_.y = -std::numbers::pi_v<float> / 2.0f;

	velocity_ = {-kWalkSpeed, 0.0f, 0.0f};

	walkTimer_ = 0.0f;

}

void Enemy::Update() {
	// 1. 重力を適用
	velocity_.y += kGravity;
	if (velocity_.y < kLimitFallSpeed) {
		velocity_.y = kLimitFallSpeed;
	}

	// 2. X軸移動 & 判定
	worldTransform_.translation_.x += velocity_.x;
	CheckMapCollisionX();

	// 3. Y軸移動 & 判定（着地）
	worldTransform_.translation_.y += velocity_.y;
	CheckMapCollisionY();

	// 4. Z軸移動
	worldTransform_.translation_.z += velocity_.z;

	// 5. タイマーを加算（着地している時のみアニメーション進行）
	if (isOnGround_) {
		walkTimer_ += 1.0f / 60.0f;
	}

	// 回転アニメーション
	float param = std::sin(2.0f * std::numbers::pi_v<float> * walkTimer_ / kWalkMotionTime);
	float degree = kWalkMotionAngleStart + kWalkMotionAngleEnd * (param + 1.0f) / 2.0f;
	worldTransform_.rotation_.x = degree * (std::numbers::pi_v<float> / 180.0f);

	KamataEngine::Matrix4x4 scaleMatrix = MakeScaleMatrix(worldTransform_.scale_);
	KamataEngine::Matrix4x4 rotationMatrix = MakeRotationMatrix(worldTransform_.rotation_);
	KamataEngine::Matrix4x4 translationMatrix = MakeTranslateMatrix(worldTransform_.translation_);

	worldTransform_.matWorld_ = Multiply(scaleMatrix, rotationMatrix);
	worldTransform_.matWorld_ = Multiply(worldTransform_.matWorld_, translationMatrix);

	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
}

// ★ X軸の壁判定処理
void Enemy::CheckMapCollisionX() {
	if (!mapChipField_)
		return;

	IndexSet indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_);

	if (mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex) == MapChipType::kBlock) {
		// 移動を1フレーム分戻す
		worldTransform_.translation_.x -= velocity_.x;

		// 進行方向（移動速度）を反転させる
		velocity_.x *= -1.0f;

		// 敵の向き（Y軸回転）も反転させる
		worldTransform_.rotation_.y *= -1.0f;
	}
}

// ★ Y軸（足元・重力）の着地判定処理
void Enemy::CheckMapCollisionY() {
	if (!mapChipField_)
		return;

	// 敵の足元の座標を算出
	Vector3 footPosition = worldTransform_.translation_;
	footPosition.y -= kHeight / 2.0f;

	IndexSet indexSet = mapChipField_->GetMapChipIndexSetByPosition(footPosition);

	// 足元がブロックの場合
	if (mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex) == MapChipType::kBlock) {
		// 落下中（Y速度がマイナス）であれば着地処理
		if (velocity_.y < 0.0f) {
			// ブロックのRectを取得し、天面にめり込まないよう補正
			Lect blockRect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
			worldTransform_.translation_.y = blockRect.top + kHeight / 2.0f;

			velocity_.y = 0.0f;
			isOnGround_ = true;
		}
	} else {
		// 足下にブロックがなければ空中状態
		isOnGround_ = false;
	}
}

Vector3 Enemy::GetWorldPosition() const {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

AABB Enemy::GetAABB() 
{
	// 敵のワールド座標を取得
	Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	// 敵のサイズ定数（kWidth, kHeight）を使って計算
	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

void Enemy::OnCollision(const Player* player) 
{ 
	(void)player;
}

void Enemy::Draw()
{

	model_->PreDraw();

	model_->Draw(worldTransform_, *camera_);

	model_->PostDraw();

}