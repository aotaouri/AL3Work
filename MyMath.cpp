#include "MyMath.h"
using namespace KamataEngine;

// 行列の積
KamataEngine::Matrix4x4 Multiply(KamataEngine::Matrix4x4 matrix1, KamataEngine::Matrix4x4 matrix2) {
	Matrix4x4 result;

	//---0行目---
	result.m[0][0] = matrix1.m[0][0] * matrix2.m[0][0] + matrix1.m[0][1] * matrix2.m[1][0] + matrix1.m[0][2] * matrix2.m[2][0] + matrix1.m[0][3] * matrix2.m[3][0];
	result.m[0][1] = matrix1.m[0][0] * matrix2.m[0][1] + matrix1.m[0][1] * matrix2.m[1][1] + matrix1.m[0][2] * matrix2.m[2][1] + matrix1.m[0][3] * matrix2.m[3][1];
	result.m[0][2] = matrix1.m[0][0] * matrix2.m[0][2] + matrix1.m[0][1] * matrix2.m[1][2] + matrix1.m[0][2] * matrix2.m[2][2] + matrix1.m[0][3] * matrix2.m[3][2];
	result.m[0][3] = matrix1.m[0][0] * matrix2.m[0][3] + matrix1.m[0][1] * matrix2.m[1][3] + matrix1.m[0][2] * matrix2.m[2][3] + matrix1.m[0][3] * matrix2.m[3][3];

	//---1行目---
	result.m[1][0] = matrix1.m[1][0] * matrix2.m[0][0] + matrix1.m[1][1] * matrix2.m[1][0] + matrix1.m[1][2] * matrix2.m[2][0] + matrix1.m[1][3] * matrix2.m[3][0];
	result.m[1][1] = matrix1.m[1][0] * matrix2.m[0][1] + matrix1.m[1][1] * matrix2.m[1][1] + matrix1.m[1][2] * matrix2.m[2][1] + matrix1.m[1][3] * matrix2.m[3][1];
	result.m[1][2] = matrix1.m[1][0] * matrix2.m[0][2] + matrix1.m[1][1] * matrix2.m[1][2] + matrix1.m[1][2] * matrix2.m[2][2] + matrix1.m[1][3] * matrix2.m[3][2];
	result.m[1][3] = matrix1.m[1][0] * matrix2.m[0][3] + matrix1.m[1][1] * matrix2.m[1][3] + matrix1.m[1][2] * matrix2.m[2][3] + matrix1.m[1][3] * matrix2.m[3][3];

	//---2行目---
	result.m[2][0] = matrix1.m[2][0] * matrix2.m[0][0] + matrix1.m[2][1] * matrix2.m[1][0] + matrix1.m[2][2] * matrix2.m[2][0] + matrix1.m[2][3] * matrix2.m[3][0];
	result.m[2][1] = matrix1.m[2][0] * matrix2.m[0][1] + matrix1.m[2][1] * matrix2.m[1][1] + matrix1.m[2][2] * matrix2.m[2][1] + matrix1.m[2][3] * matrix2.m[3][1];
	result.m[2][2] = matrix1.m[2][0] * matrix2.m[0][2] + matrix1.m[2][1] * matrix2.m[1][2] + matrix1.m[2][2] * matrix2.m[2][2] + matrix1.m[2][3] * matrix2.m[3][2];
	result.m[2][3] = matrix1.m[2][0] * matrix2.m[0][3] + matrix1.m[2][1] * matrix2.m[1][3] + matrix1.m[2][2] * matrix2.m[2][3] + matrix1.m[2][3] * matrix2.m[3][3];

	//---3行目---
	result.m[3][0] = matrix1.m[3][0] * matrix2.m[0][0] + matrix1.m[3][1] * matrix2.m[1][0] + matrix1.m[3][2] * matrix2.m[2][0] + matrix1.m[3][3] * matrix2.m[3][0];
	result.m[3][1] = matrix1.m[3][0] * matrix2.m[0][1] + matrix1.m[3][1] * matrix2.m[1][1] + matrix1.m[3][2] * matrix2.m[2][1] + matrix1.m[3][3] * matrix2.m[3][1];
	result.m[3][2] = matrix1.m[3][0] * matrix2.m[0][2] + matrix1.m[3][1] * matrix2.m[1][2] + matrix1.m[3][2] * matrix2.m[2][2] + matrix1.m[3][3] * matrix2.m[3][2];
	result.m[3][3] = matrix1.m[3][0] * matrix2.m[0][3] + matrix1.m[3][1] * matrix2.m[1][3] + matrix1.m[3][2] * matrix2.m[2][3] + matrix1.m[3][3] * matrix2.m[3][3];



	return result;
}

// 移動
KamataEngine::Matrix4x4 MakeTranslateMatrix(const KamataEngine::Vector3& translate)
{ 
	Matrix4x4 result;

	//1. まずは単位行列を作る(デフォルト数値を作る)
	result.m[0][0] = 1.0f;result.m[0][1] = 0.0f;result.m[0][2] = 0.0f;result.m[0][3]=0.0f;
	result.m[1][0] = 0.0f;result.m[1][1] = 1.0f;result.m[1][2] = 0.0f;result.m[1][3]=0.0f;
	result.m[2][0] = 0.0f;result.m[2][1] = 0.0f;result.m[2][2] = 1.0f;result.m[2][3]=0.0f;
	result.m[3][0] = 0.0f;result.m[3][1] = 0.0f;result.m[3][2] = 0.0f;result.m[3][3]=1.0f;
	
	//2.移動成分を代入する(行列の4行目の0、1、2、は動かしたいx、y、z) そういうルール
	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;

	return result;
}

KamataEngine::Matrix4x4 MakeRotationMatrix(const KamataEngine::Vector3& rotation)
{
	//1. 各軸のサイン・コサインを計算
	float sx = std::sin(rotation.x);float cx = std::cos(rotation.x);
	float sy = std::sin(rotation.y);float cy = std::cos(rotation.y);
	float sz = std::sin(rotation.z);float cz = std::cos(rotation.z);

	//2. X軸回転行列
	Matrix4x4 matX = {
	    1,0,0,0, 
		0,cx,sx,0, 
		0,-sx,cx,0, 
		0, 0, 0, 1

	};

	//3. Y軸回転行列
	Matrix4x4 matY = {
	    cy, 0,  -sy, 0, 
		0, 1, 0, 0, 
		sy,0,  cy, 0,  
		0, 0, 0, 1

	};

	//4. Z軸回転行列
	Matrix4x4 matZ = {
		cz,sz,0,0,
		-sz,cz,0,0,
		0,0,1,0,
		0,0,0,1
	};

	//5. 合成(回転順序はZ*X*Yが一般的です)
	Matrix4x4 result = Multiply(matZ, Multiply(matX, matY));
	return result;
}

KamataEngine::Matrix4x4 MakeScaleMatrix(const KamataEngine::Vector3& scale)
{ 

	Matrix4x4 result = {
		scale.x,0,0,0,
		0,scale.y,0,0,
		0,0,scale.z,0,
		0,0,0,1
	};

	return result;
}

bool IsCollision(const AABB& aabb1, const AABB& aabb2) {

	// 衝突判定のロジック
	if (aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x && aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y && aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z)
	{
		return true;
	}

	return false;
}
