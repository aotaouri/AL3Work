#pragma once
#include "KamataEngine.h"



// 行列の積
KamataEngine::Matrix4x4 Multiply(KamataEngine::Matrix4x4 matrix1, KamataEngine::Matrix4x4 matrix2);

// 行列変換関数
// 移動
KamataEngine::Matrix4x4 MakeTranslateMatrix(const KamataEngine::Vector3& translate);

// 回転
KamataEngine::Matrix4x4 MakeRotationMatrix(const KamataEngine::Vector3& rotation);

// 拡大
KamataEngine::Matrix4x4 MakeScaleMatrix(const KamataEngine::Vector3& scale);
