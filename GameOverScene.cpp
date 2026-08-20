#include "GameOverScene.h"
#include "Fade.h"
#include "KamataEngine.h"
#include "MyMath.h"
#include <numbers>

using namespace KamataEngine;

void GameOverScene::Initialize() {
	finished_ = false;

	
}

void GameOverScene::Update() {
	Input* input = Input::GetInstance();

	// SPACEキーでゲームオーバーを抜けてタイトルに戻る
	if (input->TriggerKey(DIK_Z)) {
		finished_ = true;
	}
}

void GameOverScene::Draw() {
	// ゲームオーバー画面の描画処理

}