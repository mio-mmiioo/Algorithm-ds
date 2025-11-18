#include "Enemy.h"
#include "Stage.h"
#include "../Library/Input.h"

Enemy::Enemy()
{
	hImage_ = LoadGraph("data/chara.png");
	position_ = VECTOR2((float)(BOX_SIZE + BOX_SIZE / 2), (float)(BOX_SIZE + BOX_SIZE / 2));
	isArrive_ = false;
	isNextSetWay_ = false;
	stage_ = FindGameObject<Stage>();
}

Enemy::~Enemy()
{
	if (hImage_ > 0) {
		DeleteGraph(hImage_);
		hImage_ = -1;
	}
}

void Enemy::Update()
{
	if (Input::IsKeyDown(KEY_INPUT_J))
	{
		isArrive_ = false;
		endPos_ = way_[0].position * (float)BOX_SIZE;
	}
	
	if (isNextSetWay_ == true && stage_->IsVertexPosition(position_) == true)
	{
		isNextSetWay_ = false;
		if (!(((int)(stage_->GetStartPos().x) == (int)(position_.x / BOX_SIZE)) && ((int)(stage_->GetStartPos().y) == (int)(position_.y / BOX_SIZE))))
		{
			way_.resize(stage_->GetShortestWay(position_).size());
			way_ = stage_->GetShortestWay(position_);
		}
	}

	if (stage_->GetIsSetStartPos() == true)
	{
		isNextSetWay_ = true;
	}

	if (isArrive_ == true)
	{
		return;
	}

	if (!(((int)position_.x / BOX_SIZE == (int)endPos_.x / BOX_SIZE) && ((int)position_.y / BOX_SIZE == (int)endPos_.y / BOX_SIZE)))
	{
		if (way_.size() > 0)
		{
			int x = position_.x / BOX_SIZE;
			int y = position_.y / BOX_SIZE;
			if (way_[way_.size() - 1].position.x == x && way_[way_.size() - 1].position.y == y)
			{
				way_.pop_back();
			}
			move = way_[way_.size() - 1].position - VECTOR2(x, y);
		}
		else
		{
			move = { 0, 0 };
			isArrive_ = true;
		}
	}

	position_ = position_ + move;

	if (way_.size() > 0)
	{
		if ((int)(position_.x / BOX_SIZE) == (int)way_[way_.size() - 1].position.x && (int)(position_.y / BOX_SIZE) == (int)way_[way_.size() - 1].position.y)
		{
			position_.x = way_[way_.size() - 1].position.x * (float)BOX_SIZE + (float)BOX_SIZE / 2;
			position_.y = way_[way_.size() - 1].position.y * (float)BOX_SIZE + (float)BOX_SIZE / 2;
		}
	}
	else
	{
		move = { 0.0f, 0.0f };
	}
}

void Enemy::Draw()
{
	DrawCircle(position_.x, position_.y, 5, GetColor(0, 255, 0), TRUE);
	if (way_.size() > 0)
	{
		DrawFormatString(10, 500, GetColor(255, 255, 255), "size:%d", way_.size());
		DrawFormatString(10, 530, GetColor(255, 255, 255), "toGo:(%d, %d)",
			(int)way_[way_.size() - 1].position.x, (int)way_[way_.size() - 1].position.y);
	}

	if (way_.size() > 0)
	{
		for (int i = 0; i < way_.size(); i++)
		{
			DrawFormatString(10, 560 + i * 30, GetColor(255, 255, 255), "%d:(%d, %d)", i, (int)way_[i].position.x, (int)way_[i].position.y);
		}

		// ’Ê‚è“¹‚Ì•\Ž¦
		for (int i = 0; i < way_.size(); i++)
		{
			if (i == way_.size() - 1)
			{
				DrawLine(position_.x, position_.y,
					(int)way_[i].position.x * BOX_SIZE + BOX_SIZE / 2, (int)way_[i].position.y * BOX_SIZE + BOX_SIZE / 2,
					GetColor(0, 0, 255), TRUE);
			}
			else if (i < way_.size() - 1)
			{
				DrawLine((int)way_[i].position.x * BOX_SIZE + BOX_SIZE / 2, (int)way_[i].position.y * BOX_SIZE + BOX_SIZE / 2,
					(int)way_[i + 1].position.x * BOX_SIZE + BOX_SIZE / 2, (int)way_[i + 1].position.y * BOX_SIZE + BOX_SIZE / 2,
					GetColor(0, 0, 255), TRUE);
			}
		}		
	}
}

