#pragma once

#define GRID_LEN 20
#define GRID_NUM 20

enum nColor
{
	NONE,
	WALL,
	START,
	END,
	OPEN,
	CLOSE
};

struct Node
{
	int _x = -999;
	int _y = -999;
	Node* _parent = nullptr;
	int _g = -999;
	int _h = -999;
	int _f = -999;
};

extern char g_map[GRID_NUM][GRID_NUM];
extern std::list<Node*> g_openList;// -Heap..F 작은 노드... x, y 검색
extern std::list<Node*> g_closeList; // -List..x, y 검색

void PathFind(int Sx, int Sy, int Ex, int Ey, int* X, int* Y);
bool CheckTile(int X, int Y);

bool checkRange(int X, int Y);