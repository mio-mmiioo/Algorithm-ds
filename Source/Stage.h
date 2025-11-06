#pragma once
#include "../Library/Object2D.h"
#include <vector>
#include <list>

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
	
	VECTOR2 dir_[4];
	std::vector<std::vector<int>> map_;

	// 経路探索関連
	VECTOR2 goPos_;
	struct vInfo {
		int x_;
		int y_;
		VECTOR2 direction_;
	};
	//std::vector<VECTOR2> vertex_; // <頂点, 向き>
	std::vector<std::pair<vInfo, int>> vertexDistance_; // 割り当てられた頂点番号, 距離
};