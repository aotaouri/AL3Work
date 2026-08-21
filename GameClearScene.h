#pragma once
#include "KamataEngine.h"

class GameClearScene {
public:
	GameClearScene() = default;
	~GameClearScene() = default;

	void Initialize();
	void Update();
	void Draw();

	bool IsFinished() const { return finished_; }

private:
	bool finished_ = false;
};