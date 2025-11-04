#pragma once
#include "../Library/Object2D.h"

class Stage;

class Player : public Object2D {
public:
	Player();
	~Player();
	void Update() override;
	void Draw() override;

private:
	Stage* stage_;
};