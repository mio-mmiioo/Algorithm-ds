#include "Stage.h"
#include "../Library/CsvReader.h"
#include <vector>
#include <queue>

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
	
	isSetStartPos_ = false;
	startPos_ = { -1, -1 };
	SetVertexList();
}

Stage::~Stage()
{
}

void Stage::Update()
{
	if (isSetStartPos_ == true)
	{
		copyWayList_.resize(wayList_.size());
		copyWayList_.assign(wayList_.begin(), wayList_.end());
		for (int i = 0; i < vertexList_.size(); i++)
		{
			vertexList_[i].distance = MAX_DISTANCE;
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

		isSetStartPos_ = false;
	}
}

void Stage::Draw()
{
	for (int y = 0; y < map_.size(); y++)
	{
		for (int x = 0; x < map_[0].size(); x++)
		{
			if (map_[y][x] == MAP_NUM::WALL)
			{
				DrawBox(x * (int)BOX_SIZE, y * (int)BOX_SIZE, x * (int)BOX_SIZE + (int)BOX_SIZE, y * (int)BOX_SIZE + (int)BOX_SIZE, GetColor(255, 255, 255), TRUE);
			}
			if (map_[y][x] == MAP_NUM::BRANCH)
			{
				DrawBox(x * (int)BOX_SIZE, y * (int)BOX_SIZE, x * (int)BOX_SIZE + (int)BOX_SIZE, y * (int)BOX_SIZE + (int)BOX_SIZE, GetColor(100, 200, 100), TRUE);
			}
			DrawLine(x * (int)BOX_SIZE, 0, x * (int)BOX_SIZE, (int)map_.size() * (int)BOX_SIZE, GetColor(0, 0, 0)); // 縦
			DrawLine(0, y * (int)BOX_SIZE, (int)map_[0].size() * (int)BOX_SIZE, y * (int)BOX_SIZE, GetColor(0, 0, 0)); // 横
		}
	}

	// 消す・表示のために書いてる
	{
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
}

int Stage::CheckRight(VECTOR2 pos)
{
	if (IsWall(pos) == false) {
		return 0;
	}
	// チップにどれぐらいめり込んでいるかを返す
	int x = (int)pos.x / (int)BOX_SIZE;
	int dx = (int)pos.x - x * (int)BOX_SIZE; // チップの中の座標
	return dx + 1;
}

int Stage::CheckLeft(VECTOR2 pos)
{
	if (IsWall(pos) == false) {
		return 0;
	}
	// チップにどれぐらいめり込んでいるかを返す
	int x = (int)pos.x / (int)BOX_SIZE;
	int dx = (int)pos.x - x * (int)BOX_SIZE; // チップの中の座標
	return (int)BOX_SIZE - dx;
}

int Stage::CheckDown(VECTOR2 pos)
{
	if (IsWall(pos) == false) {
		return 0;
	}
	// チップにどれぐらいめり込んでいるかを返す
	int y = (int)pos.y / (int)BOX_SIZE;
	int dy = (int)pos.y - y * (int)BOX_SIZE; // チップの中の座標
	return dy + 1;
}

int Stage::CheckUp(VECTOR2 pos)
{
	if (IsWall(pos) == false) {
		return 0;
	}
	// チップにどれぐらいめり込んでいるかを返す
	int y = (int)pos.y / (int)BOX_SIZE;
	int dy = (int)pos.y - y * (int)BOX_SIZE; // チップの中の座標
	return (int)BOX_SIZE - dy;
}


bool Stage::IsVertexPosition(VECTOR2 pos)
{
	int x = pos.x / BOX_SIZE;
	int y = pos.y / BOX_SIZE;
	for (int i = 0; i < vertexList_.size(); i++)
	{
		if (vertexList_[i].position.x == x && vertexList_[i].position.y == y)
		{
			return true;
		}
	}
	return false;
}

void Stage::SetStartVertex(VECTOR2 pos)
{
	int x = pos.x / BOX_SIZE;
	int y = pos.y / BOX_SIZE;
	startPos_ = { (float)x, (float)y };
	isSetStartPos_ = true;
}

void Stage::CreateGoPos(float x, float y)
{
	goPos_ = { x, y };
	int mapX = (int)x / (int)BOX_SIZE;
	int mapY = (int)y / (int)BOX_SIZE;
}

std::vector<vertex> Stage::GetShortestWay(VECTOR2 pos)
{
	int x = pos.x / BOX_SIZE;
	int y = pos.y / BOX_SIZE;

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

				return vertexList_[i].posList;
			}
		}
	}
}

bool Stage::IsWall(VECTOR2 pos)
{
	// チップの場所を特定する
	int x = (int)pos.x / (int)BOX_SIZE;
	int y = (int)pos.y / (int)BOX_SIZE;
	if (y < 0 || y >= map_.size()) {
		return false;
	}
	if (x < 0 || x >= map_[y].size()) {
		return false;
	}
	// チップの番号を見て、壁かどうか確定する
	switch (map_[y][x]) {
	case MAP_NUM::EMPTY:
	case MAP_NUM::BRANCH:
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
					map_[y][x] = MAP_NUM::BRANCH;
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
			if (map_[(int)check.y][(int)check.x] != MAP_NUM::WALL) // 壁じゃないなら
			{
				while (map_[(int)check.y][(int)check.x] != MAP_NUM::BRANCH) // 頂点に到達していない場合くり返す
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
		if (map_[y][x] == MAP_NUM::EMPTY || map_[y][x] == MAP_NUM::BRANCH)
		{
			checkDir[i] = true;
			counter += 1;
		}
		else
		{
			checkDir[i] = false;
		}
	}

	if (counter > MAP_NUM::BRANCH)
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
		if (startPos_.x == (int)vertexList_[i].position.x && startPos_.y == (int)vertexList_[i].position.y)
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
	return MAX_DISTANCE;
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

	return MAX_DISTANCE;
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
		}
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
		}
	}
}
