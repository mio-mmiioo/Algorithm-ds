#pragma once
#include "../Library/Object2D.h"
#include <vector>

struct vertex {
	VECTOR2 position; // à íuèÓïÒ
	int distance; // ãóó£
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
	
	void SetVertexList();
	bool CheckVertex(int mapX, int mapY);

	void FindStartVertex();
	int GetCost(VECTOR2 startPos, VECTOR2 endPos);

	VECTOR2 dir_[4];
	std::vector<std::vector<int>> map_;

	// åoòHíTçıä÷òA
	VECTOR2 goPos_;
	VECTOR2 start_;

	std::vector<vertex> vertexList_;
	std::vector<std::vector<VECTOR2>> wayList2_;
	std::vector<way> wayList_;
};