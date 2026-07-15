#pragma once
#include "KamataEngine.h"

// 行列の積
KamataEngine::Matrix4x4 Multiply(KamataEngine::Matrix4x4 matrix1, KamataEngine::Matrix4x4 matrix2);

KamataEngine::Vector3 Transform(const KamataEngine::Vector3& vector, const KamataEngine::Matrix4x4& mat);

// 行列変換関数
// 移動
KamataEngine::Matrix4x4 MakeTranslateMatrix(const KamataEngine::Vector3& translate);

KamataEngine::Matrix4x4 MakeTranslateZMatrix(const float angle);

// 回転
KamataEngine::Matrix4x4 MakeRotationMatrix(const KamataEngine::Vector3& rotation);

// 拡大
KamataEngine::Matrix4x4 MakeScaleMatrix(const KamataEngine::Vector3& scale);

struct AABB 
{
	KamataEngine::Vector3 min;
	KamataEngine::Vector3 max;

};


bool IsCollision(const AABB& aabb1, const AABB& aabb2);
