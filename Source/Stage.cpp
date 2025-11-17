#include "Stage.h"
#include "../Library/CsvReader.h"
#include <vector>
#include <queue>

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
	SetVertexList();
}

Stage::~Stage()
{
}

void Stage::Update()
{
	//CreateGoPos(10 * BOX_WIDTH - BOX_WIDTH / 2, 1 * BOX_HEIGHT + BOX_HEIGHT / 2);

	if (CheckHitKey(KEY_INPUT_C))
	{
		copyWayList_.resize(wayList_.size());
		copyWayList_.assign(wayList_.begin(), wayList_.end());
		for (int i = 0; i < vertexList_.size(); i++)
		{
			vertexList_[i].distance = 1000;
			vertexList_[i].isDicision = false;
			vertexList_[i].posList.clear();
		}

		vertex start = FindStartVertex();
		SetShortestWay(start);
		while (!checkVertexList_.empty())
		{
			SetShortestWay(checkVertexList_.front());
			checkVertexList_.erase(checkVertexList_.begin());
		}

	}

	if (CheckHitKey(KEY_INPUT_K))
	{
		int cost = 0;
		cost = GetCost(vertexList_[0].position, vertexList_[1].position);
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

	// 頂点情報の表示
	{
		int counter = 0;
		for (int i = 0; i < vertexList_.size(); i++)
		{
			DrawFormatString(500, (i + counter) * 30, GetColor(255, 255, 255), "x:%d, y:%d, distance:%d",
				(int)vertexList_[i].position.x, (int)vertexList_[i].position.y, vertexList_[i].distance);
			//for (int j = 0; j < vertexList_[i].next.size(); j++)
			//{
			//	counter += 1;
			//	DrawFormatString(500, (i + counter) * 30, GetColor(255, 255, 255), "next(x:%d, y:%d)",
			//		(int)vertexList_[i].next[j].position.x, (int)vertexList_[i].next[j].position.y);
			//}
		}
	}

	// 道情報の表示
	{
		for (int i = 0; i < wayList_.size(); i++)
		{
			DrawFormatString(800, i * 30, GetColor(255, 255, 255), "start(x:%d, y:%d), end(x:%d, y:%d), cost:%d",
				(int)wayList_[i].startPos.x, (int)wayList_[i].startPos.y,
				(int)wayList_[i].endPos.x, (int)wayList_[i].endPos.y,
				wayList_[i].cost);
		}
	}

	//int count = 0;
	//for (int i = 0; i < vertexList_.size(); i++)
	//{
	//	for (int j = 0; j < vertexList_[i].posList.size(); j++)
	//	{
	//		DrawFormatString(800, (i + count) * 30, GetColor(255, 255, 255), "%d:(x:%d, y:%d)",
	//			j, (int)vertexList_[i].posList[j].position.x, (int)vertexList_[i].posList[j].position.y);
	//		count += 1;
	//	}
	//}
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
	int x = pos.x / BOX_WIDTH;
	int y = pos.y / BOX_HEIGHT;
	start_ = { (float)x, (float)y };
}

void Stage::CreateGoPos(float x, float y)
{
	goPos_ = { x, y };
	int mapX = (int)x / (int)BOX_WIDTH;
	int mapY = (int)y / (int)BOX_HEIGHT;
	//map_[mapY][mapX] = 3;
}

std::vector<vertex> Stage::GetShortestWay(VECTOR2 pos)
{
	int x = pos.x / BOX_WIDTH;
	int y = pos.y / BOX_HEIGHT;

	for (int i = 0; i < vertexList_.size(); i++)
	{
		if (vertexList_[i].position.x == x)
		{
			if (vertexList_[i].position.y == y)
			{
				int checkNum = vertexList_[i].posList.size() - 1;
				while (vertexList_[i].posList[checkNum].position.x == vertexList_[i].posList[checkNum - 1].position.x &&
					vertexList_[i].posList[checkNum].position.y == vertexList_[i].posList[checkNum - 1].position.y)
				{
					vertexList_[i].posList.pop_back();
					checkNum -= 1;
				}

				SetWay(vertexList_[i].posList);
				return vertexList_[i].posList;
			}
		}
	}
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



void Stage::SetVertexList()
{
	// 分岐点を頂点の番号に変更、リストに追加
	for (int y = 0; y < map_.size(); y++)
	{
		for (int x = 0; x < map_[0].size(); x++)
		{
			if (map_[y][x] == 0)
			{
				if (CheckVertex(x, y) == true) // 一方通行じゃない→分岐地点
				{
					map_[y][x] = 2;
					vertex v = { VECTOR2{(float)x, (float)y}, 1000, vertexList_.size(), false, std::vector<vertex>()};
					vertexList_.push_back(v);
				}
			}
		}
	}

	// nextをセット 繰り返し3重はよくない
	for (int i = 0; i < vertexList_.size(); i++)
	{
		for (int direction = 0; direction < DIR::MAX_DIR; direction++)
		{
			VECTOR2 check = vertexList_[i].position + dir_[direction];

			int distance = 1;
			// 距離を求める式
			if (map_[(int)check.y][(int)check.x] != 1) // 壁じゃないなら
			{
				while (map_[(int)check.y][(int)check.x] != 2) // 頂点に到達していない場合くり返す
				{
					check = check + dir_[direction];
					distance += 1;
				}

				for (int j = 0; j < vertexList_.size(); j++)
				{
					if (vertexList_[j].position.x == check.x && vertexList_[j].position.y == check.y)
					{
						vertexList_[i].next.push_back(vertexList_[j]);
						wayList_.push_back(way{ vertexList_[i].position, vertexList_[j].position, distance });
						break;
					}
				}
			}
		}
	}
}

bool Stage::CheckVertex(int mapX, int mapY)
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

vertex Stage::FindStartVertex()
{
	vertex ret;

	for (int i = 0; i < vertexList_.size(); i++)
	{
		if (start_.x == (int)vertexList_[i].position.x && start_.y == (int)vertexList_[i].position.y)
		{
			vertexList_[i].distance = 0;
			vertexList_[i].isDicision = true;
			ret = vertexList_[i];
			break;
		}
	}
	return ret;
}

int Stage::GetCost(VECTOR2 startPos, VECTOR2 endPos)
{
	for (int i = 0; i < copyWayList_.size(); i++)
	{
		if (copyWayList_[i].startPos.x == startPos.x && copyWayList_[i].startPos.y == startPos.y)
		{
			if (copyWayList_[i].endPos.x == endPos.x && copyWayList_[i].endPos.y == endPos.y)
			{
				return copyWayList_[i].cost;
			}
		}
	}
	return 1000;
}

int Stage::GetCost(vertex start, vertex end)
{
	way ret;
	
	for (int i = 0; i < copyWayList_.size(); i++)
	{
		if (copyWayList_[i].startPos.x == start.position.x && copyWayList_[i].startPos.y == start.position.y)
		{
			if (copyWayList_[i].endPos.x == end.position.x && copyWayList_[i].endPos.y == end.position.y)
			{
				return copyWayList_[i].cost;
			}
		}
	}

	return 1000;
}

void Stage::SetWay(std::vector<vertex> vertexList)
{
	for (int i = 0; i < map_.size(); i++)
	{
		for (int j = 0; j < map_[i].size(); j++)
		{
			for (int k = 0; k < vertexList.size(); k++)
			{
				if (i == (int)vertexList[k].position.x && j == (int)vertexList[k].position.y)
				{
					map_[j][i] = 3;
				}
			}
		}
	}
}

void Stage::SetShortestWay(vertex start)
{
	// 今確認中の頂点を決定済みにする
	for (int i = 0; i < vertexList_.size(); i++)
	{
		if (vertexList_[i].position.x == start.position.x && vertexList_[i].position.y == start.position.y)
		{
			vertexList_[i].isDicision = true;
			vertexList_[i].posList.push_back(start);
		}
	}

	// 次の場所にcostを入れる
	for (int i = 0; i < vertexList_[start.number].next.size(); i++)
	{
		int checkDistance = vertexList_[start.number].distance + GetCost(vertexList_[start.number].position, vertexList_[start.number].next[i].position);
		if (vertexList_[start.number].next[i].distance > checkDistance)
		{
			for (int j = 0; j < vertexList_.size(); j++)
			{
				if (vertexList_[j].isDicision == false)
				{
					if (vertexList_[j].position.x == vertexList_[start.number].next[i].position.x && vertexList_[j].position.y == vertexList_[start.number].next[i].position.y)
					{
						vertexList_[j].distance = checkDistance;
						vertexList_[j].posList.resize(vertexList_[start.number].posList.size());
						vertexList_[j].posList.assign(vertexList_[start.number].posList.begin(), vertexList_[start.number].posList.end());
					}
				}
			}
		}/*
		DeleteWay(start, start.next[i]);
		DeleteWay(start.next[i], start);*/
	}

	// 現時点で最も近い場所を探す
	{
		std::vector<vertex> sortMinDistance;
		for (int i = 0; i < vertexList_[start.number].next.size(); i++)
		{
			if (vertexList_[vertexList_[start.number].next[i].number].isDicision == false)
			{
				sortMinDistance.push_back(vertexList_[start.number].next[i]);
			}
			// サイズが2以上ならソートする
			for (int j = sortMinDistance.size() - 2; j >= 0; j--)
			{
				if (sortMinDistance[j].distance > sortMinDistance[j + 1].distance)
				{
					std::swap(sortMinDistance[j], sortMinDistance[j + 1]);
				}
			}
			//for (int j = checkVertexList_.size() - 2; j >= 0; j--)
			//{
			//	if (checkVertexList_[j].distance > checkVertexList_[j + 1].distance)
			//	{
			//		std::swap(checkVertexList_[j].distance, checkVertexList_[j + 1].distance);
			//	}
			//}
		}

		bool isAdd = true;
		for (int i = 0; i < sortMinDistance.size(); i++)
		{
			checkVertexList_.push_back(sortMinDistance[i]);

			for (int j = checkVertexList_.size() - 2; j >= 0; j--)
			{
				if (checkVertexList_[j].distance > checkVertexList_[j + 1].distance)
				{
					std::swap(checkVertexList_[j].distance, checkVertexList_[j + 1].distance);
				}
			}
			//for (int j = 0; j < checkVertexList_.size(); j++)
			//{
			//	if (checkVertexList_[j].position.x == sortMinDistance[i].position.x && checkVertexList_[j].position.y == sortMinDistance[i].position.y)
			//	{
			//		isAdd = false;
			//	}
			//	else
			//	{
			//		isAdd = true;
			//	}
			//}
			//if (isAdd == true)
			//{
			//	checkVertexList_.push_back(sortMinDistance[i]);
			//}
		}
	}
}

void Stage::DeleteWay(vertex start, vertex end)
{
	int deleteNum = -1;
	for (int i = 0; i < copyWayList_.size(); i++)
	{
		if (copyWayList_[i].startPos.x == start.position.x && copyWayList_[i].startPos.y == start.position.y)
		{
			if (copyWayList_[i].endPos.x == end.position.x && copyWayList_[i].endPos.y == end.position.y)
			{
				deleteNum = i;
				break;
			}
		}
	}

	copyWayList_.erase(copyWayList_.begin() + deleteNum);
}

