#include "stdafx.h"
#include "aStar.h"

bool PathFind(int Sx, int Sy, int Ex, int Ey)
{
	//시작전에 출발점 노드 생성, 오픈리스트 삽입.
	Node* startNode = new Node();
	startNode->_x = g_startPos._x;
	startNode->_y = g_startPos._y;
	startNode->_g = 0.f;
	startNode->_h = (float)abs(g_startPos._x - g_endPos._x) + (float)abs(g_startPos._y - g_endPos._y);
	startNode->_f = startNode->_g + startNode->_h;

	g_openList.push_front(startNode);

	while (1)
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
		{	// true 가 앞으로, false가 뒤로
			return a->_f < b->_f;
		});
	}

	return true;
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
	if (X < 0 || X > GRID_NUM || Y < 0 || Y > GRID_NUM)
	{
		return false;
	}

	return true;
}

void makeNode(Node * param)
{
	int parX = param->_x;
	int parY = param->_y;

	if (CheckTile(parX - 1, parY))
	{
		Node* newNode = new Node();
		newNode->_parent = param;
		newNode->_x = parX - 1;
		newNode->_y = parY;
		newNode->_g = param->_g + 1;
		newNode->_h = (float)abs(g_startPos._x - g_endPos._x) + (float)abs(g_startPos._y - g_endPos._y);
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
			delete newNode;
		}
		else
		{
			// TODO : g값 큰것 찾음 , Close는?
			auto iter = std::find_if(g_openList.begin(), g_openList.end(),
				[=](Node* param) {
				if (param->_g > newNode->_g)
				{
					return true;
				}
				return false;
			});

			if (iter != g_openList.end())
			{
				// 찾음
				(*iter)->_parent = newNode;

				// 부모 바꿨으면 재계산
				(*iter)->_g = newNode->_g + 1;
				(*iter)->_h = (float)abs((*iter)->_x - g_endPos._x) + (float)abs((*iter)->_y - g_endPos._y);
				(*iter)->_f = (*iter)->_g + (*iter)->_h;
			}

			g_openList.push_front(newNode);
		}
	}

	if (CheckTile(parX - 1, parY - 1))
	{
		Node* newNode = new Node();
		newNode->_parent = param;
		newNode->_x = parX - 1;
		newNode->_y = parY - 1;
		newNode->_g = param->_g + 1.4;
		newNode->_h = (float)abs(g_startPos._x - g_endPos._x) + (float)abs(g_startPos._y - g_endPos._y);
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
			delete newNode;
		}
		else
		{
			// TODO : g값 큰것 찾음 , Close는?
			auto iter = std::find_if(g_openList.begin(), g_openList.end(),
				[=](Node* param) {
				if (param->_g > newNode->_g)
				{
					return true;
				}
				return false;
			});

			if (iter != g_openList.end())
			{
				// 찾음
				(*iter)->_parent = newNode;

				// 부모 바꿨으면 재계산
				(*iter)->_g = newNode->_g + 1;
				(*iter)->_h = (float)abs((*iter)->_x - g_endPos._x) + (float)abs((*iter)->_y - g_endPos._y);
				(*iter)->_f = (*iter)->_g + (*iter)->_h;
			}

			g_openList.push_front(newNode);
		}
	}

	if (CheckTile(parX - 1, parY + 1))
	{
		Node* newNode = new Node();
		newNode->_parent = param;
		newNode->_x = parX - 1;
		newNode->_y = parY + 1;
		newNode->_g = param->_g + 1;
		newNode->_h = (float)abs(g_startPos._x - g_endPos._x) + (float)abs(g_startPos._y - g_endPos._y);
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
			delete newNode;
		}
		else
		{
			// TODO : g값 큰것 찾음 , Close는?
			auto iter = std::find_if(g_openList.begin(), g_openList.end(),
				[=](Node* param) {
				if (param->_g > newNode->_g)
				{
					return true;
				}
				return false;
			});

			if (iter != g_openList.end())
			{
				// 찾음
				(*iter)->_parent = newNode;

				// 부모 바꿨으면 재계산
				(*iter)->_g = newNode->_g + 1;
				(*iter)->_h = (float)abs((*iter)->_x - g_endPos._x) + (float)abs((*iter)->_y - g_endPos._y);
				(*iter)->_f = (*iter)->_g + (*iter)->_h;

				g_openList.push_front(newNode);
			}
		}
	}

	if (CheckTile(parX, parY - 1))
	{
		Node* newNode = new Node();
		newNode->_parent = param;
		newNode->_x = parX;
		newNode->_y = parY - 1;
		newNode->_g = param->_g + 1;
		newNode->_h = (float)abs(g_startPos._x - g_endPos._x) + (float)abs(g_startPos._y - g_endPos._y);
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
			delete newNode;
		}
		else
		{
			// TODO : g값 큰것 찾음 , Close는?
			auto iter = std::find_if(g_openList.begin(), g_openList.end(),
				[=](Node* param) {
				if (param->_g > newNode->_g)
				{
					return true;
				}
				return false;
			});

			if (iter != g_openList.end())
			{
				// 찾음
				(*iter)->_parent = newNode;

				// 부모 바꿨으면 재계산
				(*iter)->_g = newNode->_g + 1;
				(*iter)->_h = (float)abs((*iter)->_x - g_endPos._x) + (float)abs((*iter)->_y - g_endPos._y);
				(*iter)->_f = (*iter)->_g + (*iter)->_h;
			}

			g_openList.push_front(newNode);
		}
	}

	if (CheckTile(parX + 1, parY))
	{
		Node* newNode = new Node();
		newNode->_parent = param;
		newNode->_x = parX + 1;
		newNode->_y = parY;
		newNode->_g = param->_g + 1;
		newNode->_h = (float)abs(g_startPos._x - g_endPos._x) + (float)abs(g_startPos._y - g_endPos._y);
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
			delete newNode;
		}
		else
		{
			// TODO : g값 큰것 찾음 , Close는?
			auto iter = std::find_if(g_openList.begin(), g_openList.end(),
				[=](Node* param) {
				if (param->_g > newNode->_g)
				{
					return true;
				}
				return false;
			});

			if (iter != g_openList.end())
			{
				// 찾음
				(*iter)->_parent = newNode;

				// 부모 바꿨으면 재계산
				(*iter)->_g = newNode->_g + 1;
				(*iter)->_h = (float)abs((*iter)->_x - g_endPos._x) + (float)abs((*iter)->_y - g_endPos._y);
				(*iter)->_f = (*iter)->_g + (*iter)->_h;
			}

			g_openList.push_front(newNode);
		}
	}

	if (CheckTile(parX + 1, parY - 1))
	{
		Node* newNode = new Node();
		newNode->_parent = param;
		newNode->_x = parX + 1;
		newNode->_y = parY - 1;
		newNode->_g = param->_g + 1;
		newNode->_h = (float)abs(g_startPos._x - g_endPos._x) + (float)abs(g_startPos._y - g_endPos._y);
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
			delete newNode;
		}
		else
		{
			// TODO : g값 큰것 찾음 , Close는?
			auto iter = std::find_if(g_openList.begin(), g_openList.end(),
				[=](Node* param) {
				if (param->_g > newNode->_g)
				{
					return true;
				}
				return false;
			});

			if (iter != g_openList.end())
			{
				// 찾음
				(*iter)->_parent = newNode;

				// 부모 바꿨으면 재계산
				(*iter)->_g = newNode->_g + 1;
				(*iter)->_h = (float)abs((*iter)->_x - g_endPos._x) + (float)abs((*iter)->_y - g_endPos._y);
				(*iter)->_f = (*iter)->_g + (*iter)->_h;
			}

			g_openList.push_front(newNode);
		}
	}

	if (CheckTile(parX + 1, parY + 1))
	{
		Node* newNode = new Node();
		newNode->_parent = param;
		newNode->_x = parX + 1;
		newNode->_y = parY + 1;
		newNode->_g = param->_g + 1;
		newNode->_h = (float)abs(g_startPos._x - g_endPos._x) + (float)abs(g_startPos._y - g_endPos._y);
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
			delete newNode;
		}
		else
		{
			// TODO : g값 큰것 찾음 , Close는?
			auto iter = std::find_if(g_openList.begin(), g_openList.end(),
				[=](Node* param) {
				if (param->_g > newNode->_g)
				{
					return true;
				}
				return false;
			});

			if (iter != g_openList.end())
			{
				// 찾음
				(*iter)->_parent = newNode;

				// 부모 바꿨으면 재계산
				(*iter)->_g = newNode->_g + 1;
				(*iter)->_h = (float)abs((*iter)->_x - g_endPos._x) + (float)abs((*iter)->_y - g_endPos._y);
				(*iter)->_f = (*iter)->_g + (*iter)->_h;
			}

			g_openList.push_front(newNode);
		}
	}

	if (CheckTile(parX, parY + 1))
	{
		Node* newNode = new Node();
		newNode->_parent = param;
		newNode->_x = parX;
		newNode->_y = parY + 1;
		newNode->_g = param->_g + 1;
		newNode->_h = (float)abs(g_startPos._x - g_endPos._x) + (float)abs(g_startPos._y - g_endPos._y);
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
			delete newNode;
		}
		else
		{
			// TODO : g값 큰것 찾음 , Close는?
			auto iter = std::find_if(g_openList.begin(), g_openList.end(),
				[=](Node* param) {
				if (param->_g > newNode->_g)
				{
					return true;
				}
				return false;
			});

			if (iter != g_openList.end())
			{
				// 찾음
				(*iter)->_parent = newNode;

				// 부모 바꿨으면 재계산
				(*iter)->_g = newNode->_g + 1;
				(*iter)->_h = (float)abs((*iter)->_x - g_endPos._x) + (float)abs((*iter)->_y - g_endPos._y);
				(*iter)->_f = (*iter)->_g + (*iter)->_h;
			}

			g_openList.push_front(newNode);
		}
	}
}