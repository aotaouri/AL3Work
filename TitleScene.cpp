#include "TitleScene.h"
#include "KamataEngine.h" 
#include "MyMath.h"
#include <numbers>
#include "Fade.h"

using namespace KamataEngine;

void TitleScene::Initialize() 
{ 
	playerModel_ = Model::CreateFromOBJ("player",true);
	worldTransformPlayer_.Initialize();
	worldTransformPlayer_.rotation_.y = std::numbers::pi_v<float>;

	titleModel_ = Model::CreateFromOBJ("titleFont",true);
	worldTransformTitle_.Initialize();

	camera_.Initialize();
	camera_.translation_.z = -20.0f;

	fade_ = new Fade();
	fade_->Initialize();

	phase_ = Phase::kFadeIn;
	fade_->Start(Status::FadeIn, kFadeDuration);

}

void TitleScene::Update() 
{
	
	KamataEngine::Matrix4x4 scalePlayerMatrix = MakeScaleMatrix(worldTransformPlayer_.scale_);
	KamataEngine::Matrix4x4 rotationPlayerMatrix = MakeRotationMatrix(worldTransformPlayer_.rotation_);
	KamataEngine::Matrix4x4 translationPlayerMatrix = MakeTranslateMatrix(worldTransformPlayer_.translation_);

	worldTransformPlayer_.matWorld_ = Multiply(scalePlayerMatrix, rotationPlayerMatrix);
	worldTransformPlayer_.matWorld_ = Multiply(worldTransformPlayer_.matWorld_, translationPlayerMatrix);

	// 行列を定数バッファに転送
	worldTransformPlayer_.TransferMatrix();

	KamataEngine::Matrix4x4 scaleMatrix = MakeScaleMatrix(worldTransformTitle_.scale_);
	KamataEngine::Matrix4x4 rotationMatrix = MakeRotationMatrix(worldTransformTitle_.rotation_);
	KamataEngine::Matrix4x4 translationMatrix = MakeTranslateMatrix(worldTransformTitle_.translation_);

	worldTransformTitle_.matWorld_ = Multiply(scaleMatrix, rotationMatrix);
	worldTransformTitle_.matWorld_ = Multiply(worldTransformTitle_.matWorld_, translationMatrix);

	// 行列を定数バッファに転送
	worldTransformTitle_.TransferMatrix();

	// スペースキーが押されたらタイトル終了（ゲームシーンへ移行準備）
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		finished_ = true;
	}

	fade_->Update();

	// フェーズごとの処理分岐
	switch (phase_) {
	case Phase::kFadeIn:
		// フェードインが終わったらメインフェーズへ
		if (fade_->IsFinished()) {
			phase_ = Phase::kMain;
		}
		break;

	case Phase::kMain:
		// スペースキーが押されたらフェードアウトを開始してフェードアウトフェーズへ
		if (Input::GetInstance()->PushKey(DIK_SPACE)) {
			fade_->Start(Status::FadeOut, kFadeDuration);
			phase_ = Phase::kFadeOut;
		}
		break;

	case Phase::kFadeOut:
		// フェードアウトが終わったらタイトルシーンを終了
		if (fade_->IsFinished()) {
			finished_ = true;
		}
		break;
	}

}

void TitleScene::Draw() 
{
	Model::PreDraw();
	playerModel_->Draw(worldTransformPlayer_,camera_);
	titleModel_->Draw(worldTransformTitle_, camera_);
	fade_->Draw();
	Model::PostDraw();
}

TitleScene::~TitleScene()
{ 
	delete fade_;
}