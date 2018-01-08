#include "stdafx.h"
#include "aStar.h"

// TODO : out변수 둬서 결과 리턴하게

bool PathFind(int Sx, int Sy, int Ex, int Ey)
{
	// 못 찾는 경우
	if (g_openList.size() == 0)
	{
		return false;
	}

	// openList에서 pop해서 유효한 타일에 대해 노드 생성후 삽입
	std::list<Node*>::iterator iter = g_openList.begin();
	Node* popNode = *iter;
	g_openList.pop_front();

	// closeList로 다시 넣는다
	g_closeList.push_front(popNode);

	if (g_map[popNode->_y][popNode->_x] != nColor::START &&
		g_map[popNode->_y][popNode->_x] != nColor::END)
	{
		g_map[popNode->_y][popNode->_x] = nColor::CLOSE;
	}

	// 끝인가
	if (popNode->_x == g_endPos._x &&
		popNode->_y == g_endPos._y)
	{
		g_endPos._parent = popNode;

		// 찾기 결과 그리기 위한 연결
		return true;
	}

	// 8방향 노드 생성
	makeNode(popNode);

	// f값이 작은게 앞으로
	g_openList.sort([](Node* a, Node* b)
	{
		// true 가 앞으로, false가 뒤로
		return a->_f < b->_f;
	});

	return false;
}

bool CheckTile(int X, int Y)
{
	if (g_map[Y][X] == nColor::WALL ||
		!checkRange(X, Y))
	{
		return false;
	}

	return true;
}

bool checkRange(int X, int Y)
{
	if (X < 0 || X > GRID_NUM * 2 - 1 || Y < 0 || Y > GRID_NUM - 1)
	{
		return false;
	}

	return true;
}

void makeNode(Node * parent)
{
	int parX = parent->_x;
	int parY = parent->_y;

	in_makeNode(parent, parX, parY + 1);
	in_makeNode(parent, parX, parY - 1);
	in_makeNode(parent, parX + 1, parY);
	in_makeNode(parent, parX + 1, parY + 1);
	in_makeNode(parent, parX + 1, parY - 1);
	in_makeNode(parent, parX - 1, parY);
	in_makeNode(parent, parX - 1, parY + 1);
	in_makeNode(parent, parX - 1, parY - 1);
}

void in_makeNode(Node * parent, int X, int Y)
{
	if (CheckTile(X, Y))
	{
		Node localNode;
		localNode._parent = parent;
		localNode._x = X;
		localNode._y = Y;

		if (X != parent->_x && Y != parent->_y)
		{
			localNode._g = parent->_g + 1.4f;
		}
		else
		{
			localNode._g = parent->_g + 1;
		}

		localNode._h = (float)abs(X - g_endPos._x) + (float)abs(Y - g_endPos._y);
		localNode._f = localNode._g + localNode._h;

		// 일치하는 노드를 찾고
		auto iter = std::find_if(g_openList.begin(), g_openList.end(),
			[=](Node* param) {
			if (param->_x == localNode._x &&
				param->_y == localNode._y)
			{
				return true;
			}
			return false;
		});

		if (iter != g_openList.end())
		{
			// 같은거 찾음

			while (1)
			{
				// TODO : g값 큰것 찾음
				auto iter = std::find_if(g_openList.begin(), g_openList.end(),
					[=](Node* param) {
					if (param->_x == localNode._x &&
						param->_y == localNode._y &&
						param->_g > localNode._g)
					{
						return true;
					}
					return false;
				});

				if (iter != g_openList.end())
				{
					// TODO : 찾음
					// 해당 노드의 부모를 localNode(x,y 위치는 같지만 g가 더 작은)의 부모로 변경
					(*iter)->_parent = localNode._parent;

					// 부모 바꿨으면 재계산
					(*iter)->_g = localNode._g;
					(*iter)->_f = (*iter)->_g + (*iter)->_h;

					// 재정렬.. f값이 작은게 앞으로
					g_openList.sort([](Node* a, Node* b)
					{
						// true 가 앞으로, false가 뒤로
						return a->_f < b->_f;
					});
				}
				else
				{
					break;
				}
			}

			return;
		}

		// 일치하는 노드를 찾고
		iter = std::find_if(g_closeList.begin(), g_closeList.end(),
			[=](Node* param) {
			if (param->_x == localNode._x &&
				param->_y == localNode._y)
			{
				return true;
			}
			return false;
		});

		if (iter != g_closeList.end())
		{
			// 같은거 찾음

			while (1)
			{
				// TODO : g값 큰것 찾음
				auto iter = std::find_if(g_closeList.begin(), g_closeList.end(),
					[=](Node* param) {
					if (param->_x == localNode._x &&
						param->_y == localNode._y &&
						param->_g > localNode._g)
					{
						return true;
					}
					return false;
				});

				if (iter != g_closeList.end())
				{
					// TODO : 찾음
					// 해당 노드의 부모를 localNode(x,y 위치는 같지만 g가 더 작은)의 부모로 변경
					(*iter)->_parent = localNode._parent;

					// 부모 바꿨으면 재계산
					(*iter)->_g = localNode._g;
					(*iter)->_f = (*iter)->_g + (*iter)->_h;

					// 재정렬.. f값이 작은게 앞으로
					g_closeList.sort([](Node* a, Node* b)
					{	// true 가 앞으로, false가 뒤로
						return a->_f < b->_f;
					});
				}
				else
				{
					break;
				}
			}

			return;
		}

		// 노드 실제로 만들어서 오픈리스트에 넣는다
		Node* newNode = new Node();
		newNode->_parent = localNode._parent;
		newNode->_x = localNode._x;
		newNode->_y = localNode._y;
		newNode->_g = localNode._g;
		newNode->_h = localNode._h;
		newNode->_f = localNode._f;

		g_openList.push_front(newNode);

		// 색지정
		if (g_map[newNode->_y][newNode->_x] != nColor::START &&
			g_map[newNode->_y][newNode->_x] != nColor::END)
		{
			g_map[newNode->_y][newNode->_x] = nColor::OPEN;
		}
	}
}

void releaseList(void)
{
	std::list<Node*>::iterator nowIter = g_openList.begin();
	std::list<Node*>::iterator endIter = g_openList.end();

	while (nowIter != endIter)
	{
		if ((*nowIter) == &g_startPos ||
			(*nowIter) == &g_endPos)
		{
			nowIter++;
			continue;
		}

		delete (*nowIter);
		nowIter = g_openList.erase(nowIter);
	}

	nowIter = g_closeList.begin();
	endIter = g_closeList.end();

	while (nowIter != endIter)
	{
		if ((*nowIter) == &g_startPos ||
			(*nowIter) == &g_endPos)
		{
			nowIter++;
			continue;
		}

		delete (*nowIter);
		nowIter = g_closeList.erase(nowIter);
	}

	g_endPos._parent = nullptr;
}