#define NOMINMAX
#include "Player.h"
#include "KamataEngine.h"
#include "MyMath.h"
#include "Matrix4x4.h"
#include <numbers>
#include <algorithm>
#include "WorldTransform.h"
#include <array>
#include "MapChipField.h"


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
	Move();

	//衝突情報を初期化
	CollisionMapInfo collisionMapInfo;

	//移動量に速度の値をコピー
	collisionMapInfo.velocity = velocity_;

	MapCollision(collisionMapInfo);

	topTache(collisionMapInfo);

	RVelocity(collisionMapInfo);
}

void Player::MapCollision(CollisionMapInfo& info)
{
	MapCollisionTop(info);    
	MapCollisionBottom(info);
	MapCollisionRight(info);
	MapCollisionLeft(info);
}

Vector3 Player::CornerPosition(const Vector3& center, Corner corner)
{ 
	
	Vector3 offsetTable[4] = {
	    {+kWidth / 2.0f, -kHeight / 2.0f, 0.0f}, // kRightBottom
	    {-kWidth / 2.0f, -kHeight / 2.0f, 0.0f}, // kLeftBottom
	    {+kWidth / 2.0f, +kHeight / 2.0f, 0.0f}, // kRightTop
	    {-kWidth / 2.0f, +kHeight / 2.0f, 0.0f}  // kLeftTop
	};
	// 2. 現在指定されている角（corner）のインデックスを取得
	uint32_t index = static_cast<uint32_t>(corner);

	// 3. ⭐️【安全対策】直接足し算せず、XYZ成分ごとに足し算して返す
	// これなら「二項演算子 '+'」のエラーを完璧に回避して、スライドと同じ結果を得られます！
	Vector3 result;
	result.x = center.x + offsetTable[index].x;
	result.y = center.y + offsetTable[index].y;
	result.z = center.z + offsetTable[index].z;

	return result;
}

void Player::MapCollisionTop(CollisionMapInfo& info)
{ 
	Vector3 nextCenter;
	nextCenter.x = worldTransform_.translation_.x + info.velocity.x;
	nextCenter.y = worldTransform_.translation_.y + info.velocity.y;
	nextCenter.z = worldTransform_.translation_.z + info.velocity.z;

	std::array<Vector3, 4> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		// i を Corner 型（kRightBottom〜kLeftTop）に変換して関数に渡す
		positionsNew[i] = CornerPosition(nextCenter, static_cast<Corner>(i));
	}

	if (info.velocity.y <= 0)
	{
		return;
	}

	MapChipType mapChipType;

	bool hit = false;

	struct IndexSet indexSet;

	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[static_cast<uint32_t>(Corner::kLeftTop)]);

	// マス番号からその場所のブロックの種類（空白かブロックか）を取得
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);

	// もしブロック（kBlock）だったら、ぶつかった！
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {
		// めり込みを排除する方向に移動量を設定する
		// 移動後の自キャラ上端（左上点）の座標を渡してブロックのインデックスを再取得
		 indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[static_cast<uint32_t>(Corner::kLeftTop)]);

		// めり込み先ブロックの範囲矩形（Rect）を取得
		Lect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);

		// ⭐️天井にぶつかった時の押し戻しY移動量を計算
		float calcVelocityY = rect.bottom - worldTransform_.translation_.y - kHeight / 2.0f;

		// スライド通り std::max を使って安全にめり込みを排除
		info.velocity.y =
		    std::min(0.0f, calcVelocityY); // 上方向への移動を阻止するので通常は min や直接代入が安全です。授業の指定が max(0.0f, ...) の場合は std::max(0.0f, calcVelocityY); にしてください

		// 天井に当たったことを記録するフラグを true に
		info.top = true;
	}

}

void Player::RVelocity(const CollisionMapInfo& info) {

	worldTransform_.translation_.x += info.velocity.x;
	worldTransform_.translation_.y += info.velocity.y;
	worldTransform_.translation_.z += info.velocity.z;

}

void Player::topTache(const CollisionMapInfo& info)
{   
	if (info.top)
	{
		DebugText::GetInstance()->ConsolePrintf("hit ceiling\n");
		velocity_.y = 0;
	}
}

void Player::Move()
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

		velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);

	} else {
		velocity_.x *= (1.0f - kAttenuation);
	}

	if (onGround_) {

		// ジャンプ
		if (Input::GetInstance()->PushKey(DIK_UP)) {
			velocity_.y += kJumpAcceleration;
		}
	} else {
		velocity_.y += -kGravityAcceleration;

		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
	}

	bool landing = false;
	if (velocity_.y < 0) {
		if (worldTransform_.translation_.y <= 1.0f) {
			landing = true;
		}
	}

	if (onGround_) {
		if (velocity_.y > 0.0f) {
			onGround_ = false;
		}
	} else {
		if (landing) {
			worldTransform_.translation_.y = 1.0f;

			velocity_.x *= (1.0f - kAttenuation);

			velocity_.y = 0.0f;

			onGround_ = true;
		}
	}

	// 旋回制御
	if (kTimeTurn > 0.0f) {
		turnTimer_ -= 1.0f / 60.0f;

		float destinationRotationYTable[] = {std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};

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

// --- Player.cpp の一番下 ---

void Player::MapCollisionBottom(CollisionMapInfo& info) {
	(void)info; // ⭐️「この変数はあえて使っていません」とコンパイラに伝えて警告を消す
}

void Player::MapCollisionRight(CollisionMapInfo& info) {
	(void)info; // ⭐️同様に警告消し
}

void Player::MapCollisionLeft(CollisionMapInfo& info) {
	(void)info; // ⭐️同様に警告消し
}