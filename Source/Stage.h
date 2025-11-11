#pragma once
#include "../Library/Object2D.h"
#include <vector>

struct vertex {
	VECTOR2 position; // 位置情報
	int distance; // 距離
	std::vector<vertex> next;
};

struct way {
	VECTOR2 startPos = {0, 0};
	VECTOR2 endPos = {0, 0};
	int cost = 0;
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
	void CreateGoPos(float x, float y);

private:
	enum DIR {
		RIGHT,
		DOWN,
		LEFT,
		UP,
		MAX_DIR
	};

	bool IsWall(VECTOR2 pos);
	
	VECTOR2 dir_[4];
	std::vector<std::vector<int>> map_;

	// 経路探索の初期情報関連
	void SetVertexList();
	bool CheckVertex(int mapX, int mapY);
	void FindStartVertex();
	int GetCost(VECTOR2 startPos, VECTOR2 endPos);
	
	VECTOR2 goPos_;
	VECTOR2 start_;
	std::vector<vertex> vertexList_;
	std::vector<way> wayList_;
	std::vector<way> copyWayList_;

	// 経路探索
	void SetShortestWay(vertex start, vertex end, int cost);
	void DeleteWay(vertex start, vertex end);
	int cost_; // スタートからの距離
};