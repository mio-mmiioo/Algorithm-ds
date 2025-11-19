#include "Player.h"
#include "Stage.h"

static const float Speed = 3.0f;
static const int playerSizeR = 5;

Player::Player()
{
	hImage_ = LoadGraph("data/chara.png");
	position_ = VECTOR2((float)(BOX_SIZE + BOX_SIZE / 2), (float)(BOX_SIZE + BOX_SIZE / 2));

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
		int push = stage_->CheckUp(position_ + VECTOR2(playerSizeR, -(playerSizeR - 1))); // 右上
		position_.y += push;
		push = stage_->CheckUp(position_ + VECTOR2(-playerSizeR, playerSizeR - 1)); // 左上
		position_.y += push;
	}
	if (CheckHitKey(KEY_INPUT_S)) {
		position_.y += 1;
		int push = stage_->CheckDown(position_ + VECTOR2(playerSizeR, playerSizeR - 1)); // 右下
		position_.y -= push;
		push = stage_->CheckDown(position_ + VECTOR2(-playerSizeR, playerSizeR - 1)); // 左下
		position_.y -= push;
	}
	if (CheckHitKey(KEY_INPUT_D)) {
		position_.x += 1;
		int push = stage_->CheckRight(position_ + VECTOR2(playerSizeR, -(playerSizeR - 1))); // 右上
		position_.x -= push;
		push = stage_->CheckRight(position_ + VECTOR2(playerSizeR, playerSizeR - 1)); // 右下
		position_.x -= push;
	}
	if (CheckHitKey(KEY_INPUT_A)) {
		position_.x -= 1;
		int push = stage_->CheckLeft(position_ + VECTOR2(-playerSizeR, -(playerSizeR - 1))); // 左上
		position_.x += push;
		push = stage_->CheckLeft(position_ + VECTOR2(-playerSizeR, playerSizeR - 1)); // 左下
		position_.x += push;
	}

	if (stage_->IsVertexPosition(position_) == true)
	{
		stage_->SetStartVertex(position_);
	}
}

void Player::Draw()
{
	DrawCircle(position_.x, position_.y, playerSizeR, GetColor(255, 0, 0), TRUE);
}
