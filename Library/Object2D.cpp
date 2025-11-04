#include "Object2D.h"

Object2D::Object2D()
{
	hImage_ = -1;
	position_.x = 0;
	position_.y = 0;
	imageSize_.x = 1;
	imageSize_.y = 1;
	anim_ = 0;
	animY_ = 0;
}

Object2D::~Object2D()
{
}

void Object2D::Update()
{
}

void Object2D::Draw()
{
	if (hImage_ > 0) {
		int x = position_.x - imageSize_.x / 2.0f;
		int y = position_.y - imageSize_.y / 2.0f;
		DrawRectGraph(x, y, anim_ * imageSize_.x, animY_ * imageSize_.y, imageSize_.x, imageSize_.y, hImage_, TRUE);
	}
}