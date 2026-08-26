#include "GameClearScene.h"
#include "Fade.h"
#include "KamataEngine.h"
#include "MyMath.h"
#include <cmath>
#include <numbers>

using namespace KamataEngine;

void GameClearScene::Initialize() {
	finished_ = false;

	// カメラの生成と初期化
	camera_ = new Camera();
	camera_->Initialize();

	// クリア用モデルの読み込み ("clearModel" は実際のOBJフォルダ名に合わせて変更してください)
	modelClear_ = Model::CreateFromOBJ("GAMECLEAR", true);

	// トランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
}

void GameClearScene::Update() {
	Input* input = Input::GetInstance();

	// Zキーでタイトルへ戻る
	if (input->TriggerKey(DIK_Z)) {
		finished_ = true;
	}



	// Y軸回転＋ぴょんぴょん跳ねるクリア演出
	static float timer = 0.0f;
	timer += 0.05f;
	worldTransform_.translation_.y = std::abs(std::sin(timer)) * 0.5f;

	// 行列の計算と転送
	Matrix4x4 translationMatrix = MakeTranslateMatrix(worldTransform_.translation_);
	Matrix4x4 rotationMatrix = MakeRotationMatrix(worldTransform_.rotation_);
	Matrix4x4 scaleMatrix = MakeScaleMatrix(worldTransform_.scale_);

	Matrix4x4 worldMatrix = Multiply(scaleMatrix, rotationMatrix);
	worldMatrix = Multiply(worldMatrix, translationMatrix);

	worldTransform_.matWorld_ = worldMatrix;
	worldTransform_.TransferMatrix();
}

void GameClearScene::Draw() {
	// 3Dモデル描画前処理
	KamataEngine::Model::PreDraw();

	// クリアモデルの描画
	if (modelClear_) {
		modelClear_->Draw(worldTransform_, *camera_);
	}

	// 3Dモデル描画後処理
	KamataEngine::Model::PostDraw();
}