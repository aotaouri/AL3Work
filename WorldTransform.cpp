#include "WorldTransform.h"
#include "MyMath.h"
#include "KamataEngine.h"

void WorldTransform::WorldTransformUpdate(KamataEngine::WorldTransform& worldTransform) 
{
	
	KamataEngine::Matrix4x4 scaleMatrix = MakeScaleMatrix(worldTransform.scale_);
	KamataEngine::Matrix4x4 rotationMatrix = MakeRotationMatrix(worldTransform.rotation_);
	KamataEngine::Matrix4x4 translationMatrix = MakeTranslateMatrix(worldTransform.translation_);

	worldTransform.matWorld_ = Multiply(scaleMatrix, rotationMatrix);
	worldTransform.matWorld_ = Multiply(worldTransform.matWorld_, translationMatrix);

	// 定数バッファへの書き込み
	worldTransform.TransferMatrix();
}

float Lerp(float start, float end, float tMax, float t)
{
	float newt = t / tMax;

	newt = 1.0f - newt;

	float easedt = newt * newt;

	return (1.0f - easedt) * start + easedt * end;
}