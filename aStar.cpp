#include "stdafx.h"
#include "aStar.h"

// TODO : �޸�Ǯ
void PathFind(int Sx, int Sy, int Ex, int Ey, int * X, int * Y)
{
	//�������� ����� ��� ����, ���¸���Ʈ ����.
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