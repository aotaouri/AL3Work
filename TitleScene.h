#pragma once
#include "KamataEngine.h"
#include "Fade.h"


class TitleScene {

public:

	enum class Phase {
		kFadeIn,
		kMain,
		kFadeOut,
	};

	void Initialize();

	void Update();

	void Draw();

	bool IsFinished() const { return finished_; }

	~TitleScene();

private:

	// フェード時間定数（1.0秒）
	static inline const float kFadeDuration = 1.0f;

	KamataEngine::Camera camera_;

	KamataEngine::Model* titleModel_ = nullptr;
	KamataEngine::WorldTransform worldTransformTitle_;

	KamataEngine::Model* playerModel_ = nullptr;
	KamataEngine::WorldTransform worldTransformPlayer_;

	// 終了フラグ
	bool finished_ = false;

	Fade* fade_ = nullptr;

	Phase phase_ = Phase::kFadeIn;
};
