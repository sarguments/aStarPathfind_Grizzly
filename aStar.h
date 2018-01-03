#pragma once

struct Node
{
	int _x;
	int _y;
	Node* _parent;
	int _g;
	int _h;
	int _f;
};

extern char g_map[300][300];
extern std::list<Node*> g_openList;// -Heap..F 작은 노드... x, y 검색
extern std::list<Node*> g_closeList; // -List..x, y 검색

void PathFind(int Sx, int Sy, int Ex, int Ey, int* X, int* Y);
bool CheckTile(int X, int Y);