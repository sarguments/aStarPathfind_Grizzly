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

void PathFind(int Sx, int Sy, int Ex, int Ey, int* X, int* Y);
bool CheckTile(int X, int Y);

//OpenlList<Node*> -Heap..F ���� ���... x, y �˻�
//CloseList<Node*> -List..x, y �˻�