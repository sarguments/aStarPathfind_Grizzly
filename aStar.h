#pragma once

#define GRID_LEN 20
#define GRID_NUM 28

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

extern char g_map[GRID_NUM][GRID_NUM * 2];
extern std::list<Node*> g_openList;// -Heap..F 작은 노드... x, y 검색
extern std::list<Node*> g_closeList; // -List..x, y 검색
extern Node g_startPos;
extern Node g_endPos;
extern HWND g_hWnd;

bool PathFind(int Sx, int Sy, int Ex, int Ey);
bool CheckTile(int X, int Y);

bool checkRange(int X, int Y);
void makeNode(Node* parent);
void in_makeNode(Node * parent, int X, int Y);
void releaseList(void);