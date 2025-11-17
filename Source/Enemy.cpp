#include "Enemy.h"
#include "Stage.h"
#include "../Library/Input.h"

Enemy::Enemy()
{
	hImage_ = LoadGraph("data/chara.png");
	position_ = VECTOR2(46.0f, 46.0f);
	isArrive_ = false;

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
		way_.resize(stage_->GetShortestWay(position_).size());
		way_ = stage_->GetShortestWay(position_);
		endPos_ = way_[0].position * 30.0f;
	}

	if (isArrive_ == true)
	{
		return;
	}

	if (!(((int)position_.x / 30 == (int)endPos_.x / 30) && ((int)position_.y / 30 == (int)endPos_.y / 30)))
	{
		if (way_.size() > 0)
		{
			int x = position_.x / 30;
			int y = position_.y / 30;
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
		if ((int)(position_.x / 30) == (int)way_[way_.size() - 1].position.x && (int)(position_.y / 30) == (int)way_[way_.size() - 1].position.y)
		{
			position_.x = way_[way_.size() - 1].position.x * 30.0f + 15.0f;
			position_.y = way_[way_.size() - 1].position.y * 30.0f + 15.0f;
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
	}
}

void Enemy::ToGo()
{

}
