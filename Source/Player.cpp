#include "Player.h"
#include "Stage.h"

static const float Speed = 1.5f;

Player::Player()
{
	hImage_ = LoadGraph("data/chara.png");
	position_ = VECTOR2(46.0f, 46.0f);

	stage_ = FindGameObject<Stage>();
}

Player::~Player()
{
	if (hImage_ > 0) {
		DeleteGraph(hImage_);
		hImage_ = -1;
	}
}

void Player::Update()
{
	VECTOR2 move;

	if (CheckHitKey(KEY_INPUT_W)) {
		position_.y -= 1;
		int push = stage_->CheckUp(position_ + VECTOR2(5, -4)); // 右上
		position_.y += push;
		push = stage_->CheckUp(position_ + VECTOR2(-5, 4)); // 左上
		position_.y += push;
	}
	if (CheckHitKey(KEY_INPUT_S)) {
		position_.y += 1;
		int push = stage_->CheckDown(position_ + VECTOR2(5, 4)); // 右下
		position_.y -= push;
		push = stage_->CheckDown(position_ + VECTOR2(-5, 4)); // 左下
		position_.y -= push;
	}
	if (CheckHitKey(KEY_INPUT_D)) {
		position_.x += 1;
		int push = stage_->CheckRight(position_ + VECTOR2(5, -4)); // 右上
		position_.x -= push;
		push = stage_->CheckRight(position_ + VECTOR2(5, 4)); // 右下
		position_.x -= push;
	}
	if (CheckHitKey(KEY_INPUT_A)) {
		position_.x -= 1;
		int push = stage_->CheckLeft(position_ + VECTOR2(-5, -4)); // 左上
		position_.x += push;
		push = stage_->CheckLeft(position_ + VECTOR2(-5, 4)); // 左下
		position_.x += push;
	}

	if (CheckHitKey(KEY_INPUT_M))
	{
		stage_->SetStartVertex(position_);
	}

	//if (move.x != 0 || move.y != 0)
	//{
	//	position_ = position_ + move * Speed;
	//}
}

void Player::Draw()
{
	DrawCircle(position_.x, position_.y, 5, GetColor(255, 0, 0), TRUE);
}
