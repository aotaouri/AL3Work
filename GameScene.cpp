#include "GameScene.h"
#include "2d/ImGuiManager.h"
#include "KamataEngine.h"

void GameScene::Initialize() {
	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = KamataEngine::TextureManager::Load("uvchecker.png");
	// スプライトインスタンスの生成
	sprite_ = KamataEngine::Sprite::Create(textureHandle_, {100, 50});
	// ファイル名を指定してテクスチャを読み込む
	textureHandle_2 = KamataEngine::TextureManager::Load("cube.jpg");
	// 3Dモデルの生成
	model_ = KamataEngine::Model::Create();
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// カメラの初期化
	camera_.Initialize();
	// サウンドデータの読み込み
	soundDataHandle_ = KamataEngine::Audio::GetInstance()->LoadWave("fanfare.wav");
	// 音声再生
	voiceHandle_ = KamataEngine::Audio::GetInstance()->PlayWave(soundDataHandle_, true);
	// ライン描画が参照するカメラを指定する(アドレス渡し)
	KamataEngine::PrimitiveDrawer::GetInstance()->SetCamera(&camera_);

	// デバックカメラの生成
	debugCamera_ = new KamataEngine::DebugCamera(1280, 720);

	//軸方向表示の表示を有効にする
	KamataEngine::AxisIndicator::GetInstance()->SetVisible(true);

	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	KamataEngine::AxisIndicator::GetInstance()->SetTargetCamera(&debugCamera_->GetCamera());

}

void GameScene::Update() {
	// スプライトの今の座標を取得
	KamataEngine::Vector2 position = sprite_->GetPosition();
	// 座標を{2,1}移動
	position.x += 2.0f;
	position.y += 1.0f;
	// 移動した座標をスプライトに反射
	sprite_->SetPosition(position);
	// スペースキーを押した瞬間
	if (KamataEngine::Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		// 音声停止
		KamataEngine::Audio::GetInstance()->StopWave(voiceHandle_);
	}

#ifdef _DEBUG
	// float3入力ボックス
	ImGui::InputFloat3("InputFloat3", inputFloat3);
	// float3スライダー
	ImGui::SliderFloat3("SliderFloat3", inputFloat3, 0.0f, 1.0f);
	// デモウインドウの表示を有効化
	ImGui::ShowDemoWindow();
	// デバックテキストの表示
	ImGui::Text("Kamata Tarou %d.%d.%d", 2050, 12, 31);
#endif

	// デバックカメラの更新
	debugCamera_->Update();
}

void GameScene::Draw() {
	// スプライト描画前処理
	KamataEngine::Sprite::PreDraw();

	sprite_->Draw();
	// スプライト描画後処理
	KamataEngine::Sprite::PostDraw();
	// 3Dモデル描画前処理
	KamataEngine::Model::PreDraw();
	// 3Dモデル描画
	model_->Draw(worldTransform_, camera_, textureHandle_2);
	model_->Draw(worldTransform_, debugCamera_->GetCamera(), textureHandle_2);

	// 3Dモデル描画後処理
	KamataEngine::Model::PostDraw();


	// ライン描画する
	KamataEngine::PrimitiveDrawer::GetInstance()->SetCamera(&debugCamera_->GetCamera());
	const float kGridSize = 15.0f;
	const int kSubdivision = 20;

	const float kSpace = (kGridSize * 2.0f) / kSubdivision;

	for (int i = 0; i <= kSubdivision; i++)
	{
		float offset = -kGridSize + (static_cast<float>(i) * kSpace);

		KamataEngine::PrimitiveDrawer::GetInstance()->DrawLine3d({-kGridSize, 0.0f, offset}, {kGridSize, 0.0f, offset}, {0.8f, 0.0f, 0.0f, 1.0f});

		KamataEngine::PrimitiveDrawer::GetInstance()->DrawLine3d({offset, 0.0f, -kGridSize}, {offset, 0.0f, kGridSize}, {0.0f, 0.0f, 0.8f, 1.0f});


	}

}

GameScene::~GameScene() {
	delete sprite_;

	delete model_;

	delete debugCamera_;
}