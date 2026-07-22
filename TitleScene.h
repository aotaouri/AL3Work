#pragma once
#include "KamataEngine.h"

class TitleScene {

public:
	void Initialize();

	void Update();

	void Draw();

	bool IsFinished() const { return finished_; }

private:
	KamataEngine::Camera camera_;

	KamataEngine::Model* titleModel_ = nullptr;
	KamataEngine::WorldTransform worldTransformTitle_;

	KamataEngine::Model* playerModel_ = nullptr;
	KamataEngine::WorldTransform worldTransformPlayer_;

	// 終了フラグ
	bool finished_ = false;
};
