#pragma once
#include "KamataEngine.h"

class WorldTransform {
public:

	void WorldTransformUpdate(KamataEngine::WorldTransform& worldTransform);
};
float Lerp(float start, float end, float tMax, float t);