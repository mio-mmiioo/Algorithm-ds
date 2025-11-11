#pragma once
#include "../Library/Object2D.h"
#include <vector>

struct vInfo {
	int x_;
	int y_;
	VECTOR2 direction_;
};

struct vInfo1 {
	int x_;
	int y_;
};

struct vertex {
	VECTOR2 position; // 位置情報
	int distance; // 距離
	std::vector<vertex> next;
};

class Stage : public Object2D
{
public:
	Stage();
	~Stage();
	void Update() override;
	void Draw() override;

	int CheckRight(VECTOR2 pos);
	int CheckLeft(VECTOR2 pos);
	int CheckDown(VECTOR2 pos);
	int CheckUp(VECTOR2 pos);

	VECTOR2 GetGoPos() { return goPos_; }
	void SetStartVertex(VECTOR2 pos);

private:
	enum DIR {
		RIGHT,
		DOWN,
		LEFT,
		UP,
		MAX_DIR
	};

	bool IsWall(VECTOR2 pos);
	bool SetVertex(int mapX, int mapY);
	void CreateGoPos(float x, float y);
	void SetVertexDistance();
	void CheckDir(int x, int y);

	bool FindStartVertex(int* x, int* y);

	void SetVertexList();
	void CheckDirVertex(int checkNum);

	VECTOR2 dir_[4];
	std::vector<std::vector<int>> map_;

	// 経路探索関連
	VECTOR2 goPos_;

	std::vector<std::pair<vInfo, int>> vertexDistance_; // 割り当てられた頂点番号, 距離
	std::vector<std::pair<vInfo1, int>> vertexDistance1_; // 割り当てられた頂点番号, 距離 こっちで最終的にはかく

	vInfo start_;

	std::vector<vertex> vertexList_;

};