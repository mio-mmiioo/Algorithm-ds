#include "Stage.h"
#include "../Library/CsvReader.h"

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
	
	for (int y = 0; y < map_.size(); y++)
	{
		for (int x = 0; x < map_[0].size(); x++)
		{
			if (map_[y][x] == 0)
			{
				if (CheckDir(x, y) == true) // 一方通行じゃない→分岐地点
				{
					map_[y][x] = 2;
				}

			}
		}
	}
}

Stage::~Stage()
{
}

void Stage::Update()
{
	CreateGoPos(10 * BOX_WIDTH - BOX_WIDTH / 2, 1 * BOX_HEIGHT + BOX_HEIGHT / 2);
}

void Stage::Draw()
{
	for (int y = 0; y < map_.size(); y++)
	{
		for (int x = 0; x < map_[0].size(); x++)
		{
			if (map_[y][x] == 1)
			{
				DrawBox(x * BOX_WIDTH, y * BOX_HEIGHT, x * BOX_WIDTH + BOX_WIDTH, y * BOX_HEIGHT + BOX_HEIGHT, GetColor(255, 255, 255), TRUE);
			}
			if (map_[y][x] == 2)
			{
				DrawBox(x * BOX_WIDTH, y * BOX_HEIGHT, x * BOX_WIDTH + BOX_WIDTH, y * BOX_HEIGHT + BOX_HEIGHT, GetColor(100, 200, 100), TRUE);
			}
			if (map_[y][x] == 3)
			{
				DrawBox(x * BOX_WIDTH, y * BOX_HEIGHT, x * BOX_WIDTH + BOX_WIDTH, y * BOX_HEIGHT + BOX_HEIGHT, GetColor(100, 100, 200), TRUE);
			}
			DrawLine(x * BOX_WIDTH, 0, x * BOX_WIDTH, map_.size() * BOX_HEIGHT, GetColor(0, 0, 0)); // 縦
			DrawLine(0, y * BOX_HEIGHT, map_[0].size() * BOX_WIDTH, y * BOX_HEIGHT, GetColor(0, 0, 0)); // 横
		}
	}
}

int Stage::CheckRight(VECTOR2 pos)
{
	if (IsWall(pos) == false) {
		return 0;
	}
	// チップにどれぐらいめり込んでいるかを返す
	int x = pos.x / BOX_WIDTH;
	int dx = pos.x - x * BOX_WIDTH; // チップの中の座標
	return dx + 1;
}

int Stage::CheckLeft(VECTOR2 pos)
{
	if (IsWall(pos) == false) {
		return 0;
	}
	// チップにどれぐらいめり込んでいるかを返す
	int x = pos.x / BOX_WIDTH;
	int dx = pos.x - x * BOX_WIDTH; // チップの中の座標
	return BOX_WIDTH - dx;
}

int Stage::CheckDown(VECTOR2 pos)
{
	if (IsWall(pos) == false) {
		return 0;
	}
	// チップにどれぐらいめり込んでいるかを返す
	int y = pos.y / BOX_HEIGHT;
	int dy = pos.y - y * BOX_HEIGHT; // チップの中の座標
	return dy + 1;
}

int Stage::CheckUp(VECTOR2 pos)
{
	if (IsWall(pos) == false) {
		return 0;
	}
	// チップにどれぐらいめり込んでいるかを返す
	int y = pos.y / BOX_HEIGHT;
	int dy = pos.y - y * BOX_HEIGHT; // チップの中の座標
	return BOX_HEIGHT - dy;
}

bool Stage::IsWall(VECTOR2 pos)
{
	// チップの場所を特定する
	int x = pos.x / BOX_WIDTH;
	int y = pos.y / BOX_HEIGHT;
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

bool Stage::CheckDir(int mapX, int mapY)
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
		int x = mapX + dir_[i].x;
		int y = mapY + dir_[i].y;
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

	// いらないかも
	if (ret == true)
	{
		for (int i = 0; i < DIR::MAX_DIR; i++)
		{
			if (checkDir[i] == true)
			{
				VECTOR2 v = { (float)mapX, (float)mapY };
				vertex_.push_back(v);

			}
		}
	}

	return ret;
}

void Stage::CreateGoPos(float x, float y)
{
	goPos_ = { x, y };
	int mapX = x / BOX_WIDTH;
	int mapY = y / BOX_HEIGHT;
	map_[mapY][mapX] = 3;
}

void Stage::SetVertexDistance(int x, int y, DIR dir)
{

}
