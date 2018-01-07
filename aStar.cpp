#include "stdafx.h"
#include "aStar.h"

// TODO : 생성 나중에 하게 바꾸기
// TODO : 다음 노드 리턴하게

bool PathFind(int Sx, int Sy, int Ex, int Ey)
{
	// TODO : 완료 여부
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

	// TODO : 끝인가?
	if (popNode->_x == g_endPos._x &&
		popNode->_y == g_endPos._y)
	{
		g_endPos._parent = popNode;

		// TODO : 연결..
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
		Node* newNode = new Node();
		newNode->_parent = parent;
		newNode->_x = X;
		newNode->_y = Y;

		if (X != parent->_x && Y != parent->_y)
		{
			newNode->_g = parent->_g + 1.4f;
		}
		else
		{
			newNode->_g = parent->_g + 1;
		}

		newNode->_h = (float)abs(X - g_endPos._x) + (float)abs(Y - g_endPos._y);
		newNode->_f = newNode->_g + newNode->_h;

		// 일치하는 노드를 찾고
		auto iter = std::find_if(g_openList.begin(), g_openList.end(),
			[=](Node* param) {
			if (param->_x == newNode->_x &&
				param->_y == newNode->_y)
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
				// TODO : g값 큰것 찾음 , Close는?
				auto iter = std::find_if(g_openList.begin(), g_openList.end(),
					[=](Node* param) {
					if (param->_x == newNode->_x &&
						param->_y == newNode->_y &&
						param->_g > newNode->_g)
					{
						return true;
					}
					return false;
				});

				if (iter != g_openList.end())
				{
					// TODO : 찾음
					(*iter)->_parent = newNode->_parent;

					// 부모 바꿨으면 재계산
					(*iter)->_g = newNode->_g;
					//(*iter)->_h = (float)abs((*iter)->_x - g_endPos._x) + (float)abs((*iter)->_y - g_endPos._y);
					(*iter)->_f = (*iter)->_g + (*iter)->_h;

					// 재정렬.. f값이 작은게 앞으로
					g_openList.sort([](Node* a, Node* b)
					{	// true 가 앞으로, false가 뒤로
						return a->_f < b->_f;
					});
				}
				else
				{
					break;
				}
			}

			delete newNode;
			return;
		}

		// 일치하는 노드를 찾고
		iter = std::find_if(g_closeList.begin(), g_closeList.end(),
			[=](Node* param) {
			if (param->_x == newNode->_x &&
				param->_y == newNode->_y)
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
				// TODO : g값 큰것 찾음 , Close는?
				auto iter = std::find_if(g_closeList.begin(), g_closeList.end(),
					[=](Node* param) {
					if (param->_x == newNode->_x &&
						param->_y == newNode->_y &&
						param->_g > newNode->_g)
					{
						return true;
					}
					return false;
				});

				if (iter != g_closeList.end())
				{
					// TODO : 찾음
					(*iter)->_parent = newNode->_parent;

					// 부모 바꿨으면 재계산
					(*iter)->_g = newNode->_g;
					//(*iter)->_h = (float)abs((*iter)->_x - g_endPos._x) + (float)abs((*iter)->_y - g_endPos._y);
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

			delete newNode;
			return;
		}

		g_openList.push_front(newNode);

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