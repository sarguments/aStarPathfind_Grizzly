#include "stdafx.h"
#include "aStar.h"

// TODO : 메모리풀
void PathFind(int Sx, int Sy, int Ex, int Ey, int * X, int * Y)
{
	//시작전에 출발점 노드 생성, 오픈리스트 삽입.
	while (1)
	{
	}
}

bool CheckTile(int X, int Y)
{
	return false;
}

bool checkRange(int X, int Y)
{
	if (X < 0 || X > GRID_NUM || Y < 0 || Y > GRID_NUM)
	{
		return false;
	}

	return true;
}