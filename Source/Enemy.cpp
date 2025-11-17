#include "Enemy.h"
#include "Stage.h"
#include "../Library/Input.h"

Enemy::Enemy()
{
	hImage_ = LoadGraph("data/chara.png");
	position_ = VECTOR2(46.0f, 46.0f);

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
		way_.resize(stage_->GetShortestWay(position_).size());
		way_ = stage_->GetShortestWay(position_);
		endPos_ = way_[0].position * 30.0f;
	}

	if (!(((int)position_.x / 30 == (int)endPos_.x / 30) && ((int)position_.y / 30 == (int)endPos_.y / 30)))
	{
		if (way_.size() > 1)
		{
			move = way_[way_.size() - 2].position - way_[way_.size() - 1].position;
		}
	}

	position_ = position_ + move * 0.5f;

	if (way_.size() > 0)
	{
		if ((int)(position_.x / 30) == (int)way_[way_.size() - 1].position.x && (int)(position_.y / 30) == (int)way_[way_.size() - 1].position.y)
		{
			way_.erase(way_.begin() + way_.size() - 1);
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
			(int)way_[0].position.x, (int)way_[0].position.y);
	}

}

void Enemy::ToGo()
{

}
