#pragma once
#include "../Library/Object2D.h"
#include <vector>

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
	bool IsWall(VECTOR2 pos);
	bool CheckDir(int mapX, int mapY);
	void CreateGoPos(float x, float y);
	void SetVertexDistance();
	
	VECTOR2 dir[4];
	std::vector<std::vector<int>> map_;

	// Œo˜H’TõŠÖ˜A
	VECTOR2 goPos_;
	std::vector<std::vector<int>> vertex_; // Š„‚è“–‚Ä‚ç‚ê‚½’¸“_”Ô†, ‹——£
};