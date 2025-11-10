#include "Stage.h"
#include "../Library/CsvReader.h"
#include <vector>
#include <queue>

using cr_pair = std::pair<int, vInfo1>;

namespace {
	float BOX_WIDTH = 30;
	float BOX_HEIGHT = 30;

}

Stage::Stage()
{
	dir_[RIGHT] = {  1,  0 };
	dir_[DOWN]  = {  0,  1 };
	dir_[LEFT]  = { -1,  0 };
	dir_[UP]    = {  0, -1 };

	char filename[64];
	sprintf_s<64>(filename, "data/stage%02d.csv", 1);
	map_.clear();

	// ステージデータの読み込み
	CsvReader* csv = new CsvReader(filename);
	for (int line = 0; line < csv->GetLines(); line++) {
		std::vector<int> mapLine;
		for (int column = 0; column < csv->GetColumns(line); column++) {
			int c = csv->GetInt(line, column);
			mapLine.push_back(c);
		}
		map_.push_back(mapLine);
	}
	delete csv;
	
	start_ = { 5,1 }; // マジックナンバー、消すこと
	vertexCount_ = 0;
	SetVertexDistance();
	
}

Stage::~Stage()
{
}

void Stage::Update()
{
	CreateGoPos(10 * BOX_WIDTH - BOX_WIDTH / 2, 1 * BOX_HEIGHT + BOX_HEIGHT / 2);

	if (CheckHitKey(KEY_INPUT_C))
	{
		DecisionShortestWay();
	}
}

void Stage::Draw()
{
	for (int y = 0; y < map_.size(); y++)
	{
		for (int x = 0; x < map_[0].size(); x++)
		{
			if (map_[y][x] == 1)
			{
				DrawBox(x * (int)BOX_WIDTH, y * (int)BOX_HEIGHT, x * (int)BOX_WIDTH + (int)BOX_WIDTH, y * (int)BOX_HEIGHT + (int)BOX_HEIGHT, GetColor(255, 255, 255), TRUE);
			}
			if (map_[y][x] == 2)
			{
				DrawBox(x * (int)BOX_WIDTH, y * (int)BOX_HEIGHT, x * (int)BOX_WIDTH + (int)BOX_WIDTH, y * (int)BOX_HEIGHT + (int)BOX_HEIGHT, GetColor(100, 200, 100), TRUE);
			}
			if (map_[y][x] == 3)
			{
				DrawBox(x * (int)BOX_WIDTH, y * (int)BOX_HEIGHT, x * (int)BOX_WIDTH + (int)BOX_WIDTH, y * (int)BOX_HEIGHT + (int)BOX_HEIGHT, GetColor(100, 100, 200), TRUE);
			}
			DrawLine(x * (int)BOX_WIDTH, 0, x * (int)BOX_WIDTH, (int)map_.size() * (int)BOX_HEIGHT, GetColor(0, 0, 0)); // 縦
			DrawLine(0, y * (int)BOX_HEIGHT, (int)map_[0].size() * (int)BOX_WIDTH, y * (int)BOX_HEIGHT, GetColor(0, 0, 0)); // 横
		}
	}

	// 情報を表示
	for (int i = 0; i < vertexDistance_.size(); i++)
	{
		DrawFormatString(500, i * 30, GetColor(255, 255, 255), "x:%d, y:%d, 向き:(%2d, %2d), distance:%d",
			vertexDistance_[i].first.x_, vertexDistance_[i].first.y_, 
			(int)vertexDistance_[i].first.direction_.x, (int)vertexDistance_[i].first.direction_.y, vertexDistance_[i].second);
	}
}

int Stage::CheckRight(VECTOR2 pos)
{
	if (IsWall(pos) == false) {
		return 0;
	}
	// チップにどれぐらいめり込んでいるかを返す
	int x = (int)pos.x / (int)BOX_WIDTH;
	int dx = (int)pos.x - x * (int)BOX_WIDTH; // チップの中の座標
	return dx + 1;
}

int Stage::CheckLeft(VECTOR2 pos)
{
	if (IsWall(pos) == false) {
		return 0;
	}
	// チップにどれぐらいめり込んでいるかを返す
	int x = (int)pos.x / (int)BOX_WIDTH;
	int dx = (int)pos.x - x * (int)BOX_WIDTH; // チップの中の座標
	return (int)BOX_WIDTH - dx;
}

int Stage::CheckDown(VECTOR2 pos)
{
	if (IsWall(pos) == false) {
		return 0;
	}
	// チップにどれぐらいめり込んでいるかを返す
	int y = (int)pos.y / (int)BOX_HEIGHT;
	int dy = (int)pos.y - y * (int)BOX_HEIGHT; // チップの中の座標
	return dy + 1;
}

int Stage::CheckUp(VECTOR2 pos)
{
	if (IsWall(pos) == false) {
		return 0;
	}
	// チップにどれぐらいめり込んでいるかを返す
	int y = (int)pos.y / (int)BOX_HEIGHT;
	int dy = (int)pos.y - y * (int)BOX_HEIGHT; // チップの中の座標
	return (int)BOX_HEIGHT - dy;
}

void Stage::SetStartVertex(VECTOR2 pos)
{
	int x = (int)pos.x / (int)BOX_WIDTH;
	int y = (int)pos.y / (int)BOX_HEIGHT;
	start_ = { x, y };
}

bool Stage::IsWall(VECTOR2 pos)
{
	// チップの場所を特定する
	int x = (int)pos.x / (int)BOX_WIDTH;
	int y = (int)pos.y / (int)BOX_HEIGHT;
	if (y < 0 || y >= map_.size()) {
		return false;
	}
	if (x < 0 || x >= map_[y].size()) {
		return false;
	}
	// チップの番号を見て、壁かどうか確定する
	switch (map_[y][x]) {
	case 0:
	case 2:
	case 3:
		return false;
	}
	return true;
}

bool Stage::SetVertex(int mapX, int mapY)
{
	int counter = 0;
	bool ret = false;
	if (mapX == 0 || mapY == 0 || mapX == map_[mapY].size() - 1 || mapY == map_.size() - 1)
	{
		return ret;
	}

	bool checkDir[DIR::MAX_DIR];
	
	for (int i = 0; i < DIR::MAX_DIR; i++)
	{
		int x = mapX + (int)dir_[i].x;
		int y = mapY + (int)dir_[i].y;
		if (map_[y][x] == 0 || map_[y][x] == 2)
		{
			checkDir[i] = true;
			counter += 1;
		}
		else
		{
			checkDir[i] = false;
		}
	}

	if (counter > 2)
	{
		ret = true;
	}

	// 直角に曲がる
	for (int i = 0; i < DIR::MAX_DIR; i++)
	{
		if (i != DIR::MAX_DIR - 1)
		{
			if (checkDir[i] == checkDir[i + 1])
			{
				ret = true;
			}
		}
		else
		{
			if (checkDir[i] == checkDir[0])
			{
				ret = true;
			}
		}
	}

	return ret;
}

void Stage::CreateGoPos(float x, float y)
{
	goPos_ = { x, y };
	int mapX = (int)x / (int)BOX_WIDTH;
	int mapY = (int)y / (int)BOX_HEIGHT;
	map_[mapY][mapX] = 3;
}

void Stage::SetVertexDistance()
{
	// 分岐点を頂点にする
	for (int y = 0; y < map_.size(); y++)
	{
		for (int x = 0; x < map_[0].size(); x++)
		{
			if (map_[y][x] == 0)
			{
				if (SetVertex(x, y) == true) // 一方通行じゃない→分岐地点
				{
					map_[y][x] = 2;
				}

			}
		}
	}

	// 頂点と距離を求めて、代入する
	for (int y = 0; y < map_.size(); y++)
	{
		for (int x = 0; x < map_[0].size(); x++)
		{
			if (map_[y][x] == 2)
			{
				CheckDir(x, y);
			}
		}
	}
}

void Stage::CheckDir(int x, int y)
{
	vertexCount_ += 1;
	for (int i = 0; i < DIR::MAX_DIR; i++)
	{
		VECTOR2 check = { (float)x, (float)y };
		check = check + dir_[i];

		int distance = 1;
		// 距離を求める式
		if (map_[(int)check.y][(int)check.x] != 1) // 壁じゃないなら
		{
			while (map_[(int)check.y][(int)check.x] != 2) // 頂点に到達していない場合くり返す
			{
				check = check + dir_[i];
				distance += 1;
			}
			vInfo current = { x, y, dir_[i] }; // x座標、y座標、方向
			vInfo1 c = { x, y };
			vertexDistance_.push_back(std::make_pair(current, distance));
			vertexDistance1_.push_back(std::make_pair(c, distance));
		}
	}
}

void Stage::DecisionShortestWay()
{
	dist.clear();
	dist.resize(vertexCount_);
	// 大きい値で初期化
	for (int i = 0; i < vertexCount_; i++)
	{
		dist[i] = 1000;
	}

	int x, y;

	FindStartVertex(&x, &y);

	// 壁じゃないほうに線を伸ばす

	

}

bool Stage::FindStartVertex(int* x, int* y)
{
	int counter = 0;
	int prevX = -1;
	int prevY = -1;
	for (int i = 0; i < vertexDistance1_.size(); i++)
	{
		if (start_.x_ == vertexDistance1_[i].first.x_)
		{
			if (start_.y_ == vertexDistance1_[i].first.y_)
			{
				dist[counter] = 0;
				*x = vertexDistance1_[i].first.x_;
				*y = vertexDistance1_[i].first.y_;
				return true;
			}
		}
		// 違う座標か確認する
		if (!(prevX == vertexDistance1_[i].first.x_ && prevY == vertexDistance1_[i].first.y_))
		{
			prevX = vertexDistance1_[i].first.x_;
			prevY = vertexDistance1_[i].first.y_;
			counter += 1;
		}
	}
	return false;
}

void Stage::DecisionWay()
{
	way_.clear();
	way_.resize(vertexDistance_.size());
	way_ = vertexDistance_;


	dicisionVertex_.clear();
	// 大きい値で初期化
	for (int i = 0; i < vertexDistance_.size(); i++)
	{
		dicisionVertex_.push_back(std::make_pair())
		way_[i].second = 1000;
	}

	// スタートポジションを見つける
	for (int i = 0; i < way_.size(); i++)
	{
		if (start_.x_ == way_[i].first.x_)
		{
			if (start_.y_ == way_[i].first.y_)
			{
				way_[i].second = 0;
				dicisionVertex_.push_back(std::make_pair(way_[i].first, 0));
				break;
			}
		}
	}

	// ルートはいったん置いておく

	// よくない書き方だけど、ごり押しでとりあえず書く

	// 確認してる頂点を求める

	// 壁じゃない方向に線を伸ばし、次の頂点の位置を求め、距離を求める

	// 頂点情報の距離が現在の距離より大きかったら入れ替える

	int x = dicisionVertex_[i].first.x_;
	int y = dicisionVertex_[i].first.y_;
	for (int i = 0; i < DIR::MAX_DIR; i++)
	{
		VECTOR2 check = { (float)x, (float)y };
		check = check + dir_[i];

		int distance = 1;
		// 距離を求める式
		if (map_[(int)check.y][(int)check.x] != 1) // 壁じゃないなら
		{
			while (map_[(int)check.y][(int)check.x] != 2) // 頂点に到達していない場合くり返す
			{
				check = check + dir_[i];
				distance += 1;
			}
			vInfo current = { x, y, dir_[i] }; // x座標、y座標、方向
			dicisionVertex_.push_back(std::make_pair(current, 0));
		}
	}

}


