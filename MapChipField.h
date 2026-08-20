#pragma once
#include "KamataEngine.h"
#include <cstdint>
#include <vector>
#include <string>
using namespace KamataEngine;

enum class MapChipType {
	kBlank, // 空白
	kBlock, // ブロック
	kEnemy, // 2: 敵
	kFlyingEnemy, // 3: 上下する敵
};

struct MapChipData {
	std::vector<std::vector<MapChipType>> data;
};

struct IndexSet {
	uint32_t xIndex;
	uint32_t yIndex;

};

struct Lect {
	float left;  //左端
	float right; //右端
	float bottom;//下端
	float top;   //上端

};

class MapChipField {
public:

	//1ブロックのサイズ
	static inline const float kBlockWidth = 1.0f;
	static inline const float kBlockHeight = 1.0f;

	//ブロックの個数
	static inline const uint32_t kNumBlockVirtical = 20;
	static inline const uint32_t kNumBlockHorizontal = 100;

	void ResetMapChipData();
	void LoadMapChipCsv(const std::string& filePath);
	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);

    Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);

	uint32_t GetNumBlockVirtical() const { return kNumBlockVirtical; }
    
    // 横方向のブロック数を返す関数
    uint32_t GetNumBlockHorizontal() const { return kNumBlockHorizontal; }

	IndexSet GetMapChipIndexSetByPosition(const Vector3& position);
	
	Lect GetRectByIndex(uint32_t xIndex, uint32_t yIndex);

	private:

	MapChipData mapChipData_;

};