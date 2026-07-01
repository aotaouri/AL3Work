#include "GameScene.h"
#include "KamataEngine.h"
#include "MapChipField.h"
#include "CameraController.h"
#include "Matrix4x4.h"
#include "MyMath.h"

using namespace KamataEngine;

void GameScene::Initialize() {

	modelblock_ = KamataEngine::Model::CreateFromOBJ("block", true);
	// 3Dモデルの生成(天球)
	modelSkydome_ = KamataEngine::Model::CreateFromOBJ("skydome", true);
	modelplayer_ = KamataEngine::Model::CreateFromOBJ("player", true);

	//カメラ
	camera_ = new KamataEngine::Camera();
	camera_->farZ = 1000.0f;
	camera_->Initialize();

	// マップチップ
	mapChipField_ = new MapChipField();
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");

	// 自キャラの生成
	player_ = new Player();
	// 座標をマップチップ番号で指定
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(1, 18);
	// 自キャラの初期化
	player_->Initialize(modelplayer_,camera_,playerPosition);

	player_->SetMapChipField(mapChipField_);

	// カメラコントローラの生成と初期化
	cameraController_ = new CameraController();
	cameraController_->Initialize(camera_);
	cameraController_->SetTarget(player_);
	Rect area = {14.5f, 84.5f, 8.0f, 20.0f};
	cameraController_->SetMovableArea(area);
	cameraController_->Reset();

	// 天球の生成」
	Skydome_ = new Skydome();
	// 天球の初期化
	Skydome_->Initialize(modelSkydome_, camera_);

	GenerateBlocks();

	// デバッグカメラの生成
	debugCamera_ = new KamataEngine::DebugCamera(1280, 720);
	isDebugCameraActive_ = false;
}

void GameScene::GenerateBlocks()
{

	//要素数
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	// 要素数を変更する
	// 配列を設定(縦方向のブロック数)
	worldTransformBlocks_.resize(numBlockVirtical);
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		// 1列の要素数を設定(横方向のブロック数)
		worldTransformBlocks_[i].resize(numBlockHorizontal);
	}

	//ブロックの生成
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				KamataEngine::WorldTransform* worldTransform = new KamataEngine::WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}

}

void GameScene::Update() {
	// 自キャラの更新
	player_->Update();

	if (!isDebugCameraActive_ && cameraController_) {
		cameraController_->Update();
	}

	Skydome_->Update();

	for (std::vector<KamataEngine::WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {

		// ブロックの更新
		for (KamataEngine::WorldTransform* worldTransformBlock : worldTransformBlockLine) {

			if (!worldTransformBlock) {
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

	// デバッグカメラの更新
	debugCamera_->Update();

#ifdef _DEBUG

	if (KamataEngine::Input::GetInstance()->TriggerKey(DIK_E)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}

#endif

	// カメラの処理
	if (isDebugCameraActive_) {
		

		camera_->matView = debugCamera_->GetCamera().matView;
		camera_->matProjection = debugCamera_->GetCamera().matProjection;
		// ビュープロジェンクション行列の転送
		camera_->TransferMatrix();
	} else {
		// ビュープロジェンクション行列の更新と転送
		
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

			modelblock_->Draw(*worldTransformBlock, *camera_);
		}
	}
	KamataEngine::Model::PostDraw();

	// 自キャラの描画
	player_->Draw();
}

GameScene::~GameScene() {

	delete player_;
	delete cameraController_;
	delete Skydome_;
	delete camera_;
	delete debugCamera_;
	delete mapChipField_;
	delete modelplayer_;
	delete modelSkydome_;
	delete modelblock_;


	for (std::vector<KamataEngine::WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {

		for (KamataEngine::WorldTransform* worldTransformBlocks : worldTransformBlockLine) {
			delete worldTransformBlocks;
		}
	}
	worldTransformBlocks_.clear();
}