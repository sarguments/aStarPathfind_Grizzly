#pragma once

#define GRID_LEN 20
#define GRID_NUM 30

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
	float _g = -999.f;
	float _h = -999.f;
	float _f = -999.f;
};

extern char g_map[GRID_NUM][GRID_NUM];
extern std::list<Node*> g_openList;// -Heap..F ���� ���... x, y �˻�
extern std::list<Node*> g_closeList; // -List..x, y �˻�
extern Node g_startPos;
extern Node g_endPos;

bool PathFind(int Sx, int Sy, int Ex, int Ey);
bool CheckTile(int X, int Y);

bool checkRange(int X, int Y);
void makeNode(Node* param);