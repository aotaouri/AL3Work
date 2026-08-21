#include "GameClearScene.h"

using namespace KamataEngine;

void GameClearScene::Initialize() { finished_ = false; }

void GameClearScene::Update() {
	Input* input = Input::GetInstance();

	// SPACEキーでタイトルへ戻る
	if (input->TriggerKey(DIK_Z)) {
		finished_ = true;
	}
}

void GameClearScene::Draw() {
	// ★ 描画コマンド発行用のセットアップを入れる
	KamataEngine::Model::PreDraw();

	// (将来的にクリア画面の3Dモデルなどを描画する場合はここに記述)

	KamataEngine::Model::PostDraw();
}