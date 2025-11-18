#pragma once
#include "../Library/Object2D.h"
#include <vector>

class Stage;
struct vertex;

class Enemy : public Object2D {
public:
	Enemy();
	~Enemy();
	void Update() override;
	void Draw() override;

private:
	std::vector<vertex> way_;
	VECTOR2 endPos_;
	Stage* stage_;
	VECTOR2 move;

	bool isArrive_;
};