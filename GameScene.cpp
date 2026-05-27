#include "GameScene.h"
#include "KamataEngine.h"
#include "MyMath.h"
#include "Matrix4x4.h"

void GameScene::Initialize() {

	textureHandle_ = KamataEngine::TextureManager::Load("uvChecker.png");
	textureHandle_2 = KamataEngine::TextureManager::Load("cube.jpg");


	model_ = KamataEngine::Model::Create();

	camera_ = new KamataEngine::Camera();
	camera_->Initialize();

	// 3Dモデルの生成(天球)
	modelSkydome_ = KamataEngine::Model::CreateFromOBJ("skydome", true);

	// 自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	player_->Initialize(model_,textureHandle_,camera_);

	//天球の生成」
	Skydome_ = new Skydome();
	//天球の初期化
	Skydome_->Initialize(modelSkydome_, camera_);

	//カメラの初期化
	camera_->farZ = 1000.0f;
	camera_->Initialize();

	int mapData[10][20] = {
	    {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
        {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
        {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
	    {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
        {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
        {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
	    {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
        {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
        {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
	    {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
	};

	//要素数
	const uint32_t kNumBlockVirtical = 10;
	const uint32_t kNumBlockHorizontal = 20;
	//ブロック1個分の横幅
	const float kBlockWidth = 2.0f;
	const float kBlockHeight = 2.0f;

	//要素数を変更する
	//配列を設定(縦方向のブロック数)
	worldTransformBlocks_.resize(kNumBlockVirtical);
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i)
	{
		//1列の要素数を設定(横方向のブロック数)
		worldTransformBlocks_[i].resize(kNumBlockHorizontal);

	}

	//キューブの生成
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {

			if (mapData[i][j] == 1) {
				worldTransformBlocks_[i][j] = new KamataEngine::WorldTransform();
				worldTransformBlocks_[i][j]->Initialize();
				worldTransformBlocks_[i][j]->translation_.x = kBlockWidth * j;
				worldTransformBlocks_[i][j]->translation_.y = kBlockHeight * i;
			}
			else {
				
				worldTransformBlocks_[i][j] = nullptr;
			}
		}
	}

	//デバッグカメラの生成
	debugCamera_ = new KamataEngine::DebugCamera(1280, 720);
	isDebugCameraActive_ = false;


}

void GameScene::Update() {
	// 自キャラの更新
	player_->Update();

	Skydome_->Update();

	for (std::vector<KamataEngine::WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {

		// ブロックの更新
		for (KamataEngine::WorldTransform* worldTransformBlock : worldTransformBlockLine) {

			if (!worldTransformBlock)
			{
				continue;
			}

			KamataEngine::Matrix4x4 translationMatrix = MakeTranslateMatrix(worldTransformBlock->translation_);
			KamataEngine::Matrix4x4 rotationMatrix = MakeRotationMatrix(worldTransformBlock->rotation_);
			KamataEngine::Matrix4x4 scaleMatrix = MakeScaleMatrix(worldTransformBlock->scale_);

			// アフィン変換
			KamataEngine::Matrix4x4 worldMatrix = Multiply(scaleMatrix, rotationMatrix);
			worldMatrix = Multiply(worldMatrix, translationMatrix);

			// 定数バッファに転送する
			worldTransformBlock->matWorld_ = worldMatrix;
			worldTransformBlock->TransferMatrix();
		}
	}

	//デバッグカメラの更新
	debugCamera_->Update();

#ifdef _DEBUG

	if (KamataEngine::Input::GetInstance()->TriggerKey(DIK_E))
	{
		isDebugCameraActive_ = !isDebugCameraActive_;
	}

#endif

	//カメラの処理
	if (isDebugCameraActive_)
	{
		debugCamera_->Update();
		
		
		camera_->matView = debugCamera_->GetCamera().matView;
		camera_->matProjection = debugCamera_->GetCamera().matProjection;
		//ビュープロジェンクション行列の転送
		camera_->TransferMatrix();
	} else {
		//ビュープロジェンクション行列の更新と転送
		camera_->UpdateMatrix();
	}

}

void GameScene::Draw() {

	Skydome_->Draw();

	KamataEngine::Model::PreDraw();

	for (std::vector<KamataEngine::WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		// ブロックの描画
		for (KamataEngine::WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			
			if (worldTransformBlock == nullptr) {
				continue;
			}

			model_->Draw(*worldTransformBlock, *camera_);

			
		}
	}
	KamataEngine::Model::PostDraw();
	
	// 自キャラの描画
	player_->Draw();

}

GameScene::~GameScene() {

	delete model_;
	delete player_;
	delete camera_;
	delete debugCamera_;
	delete modelSkydome_;

	for (std::vector<KamataEngine::WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {

		for (KamataEngine::WorldTransform* worldTransformBlocks : worldTransformBlockLine) {
			delete worldTransformBlocks;
		}
	}
	worldTransformBlocks_.clear();

}