#pragma once
#include "../Library/Object2D.h"
#include <vector>

namespace {
	int BOX_SIZE = 30;
	int MAX_DISTANCE = 1000;
}

struct vertex {
	VECTOR2 position; // 位置情報
	int distance; // 距離
	int number;
	bool isDicision; // 決定しているか
	std::vector<vertex> next;
	std::vector<vertex> posList;
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
	VECTOR2 GetStartPos() { return startPos_; }
	bool IsVertexPosition(VECTOR2 pos);
	bool GetIsSetStartPos() { return isSetStartPos_; }
	void SetStartVertex(VECTOR2 pos);
	void CreateGoPos(float x, float y);

	std::vector<vertex> GetShortestWay(VECTOR2 pos);

private:
	enum DIR {
		RIGHT,
		DOWN,
		LEFT,
		UP,
		MAX_DIR
	};

	enum MAP_NUM {
		EMPTY,
		WALL,
		BRANCH,
		MAX_MAP_NUM
	};

	bool IsWall(VECTOR2 pos);
	
	VECTOR2 dir_[4];
	std::vector<std::vector<int>> map_;

	// 経路探索の初期情報関連
	bool isSetStartPos_;
	void SetVertexList();
	bool CheckVertex(int mapX, int mapY);
	vertex FindStartVertex();
	int GetCost(VECTOR2 startPos, VECTOR2 endPos);
	int GetCost(vertex start, vertex end);

	VECTOR2 goPos_;
	VECTOR2 startPos_;
	std::vector<vertex> vertexList_;
	std::vector<vertex> checkVertexList_;
	std::vector<way> wayList_;
	std::vector<way> copyWayList_;
	
	// 経路探索
	void SetShortestWay(vertex start);
	int cost_; // スタートからの距離
};