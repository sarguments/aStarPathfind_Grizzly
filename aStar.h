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
extern std::list<Node*> g_openList;// -Heap..F ���� ���... x, y �˻�
extern std::list<Node*> g_closeList; // -List..x, y �˻�

void PathFind(int Sx, int Sy, int Ex, int Ey, int* X, int* Y);
bool CheckTile(int X, int Y);