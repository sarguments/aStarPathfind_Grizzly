#include "stdafx.h"
#include "aStar.h"

// TODO : out���� �ּ� ��� �����ϰ�

bool PathFind(int Sx, int Sy, int Ex, int Ey)
{
	// �� ã�� ���
	if (g_openList.size() == 0)
	{
		return false;
	}

	// openList���� pop�ؼ� ��ȿ�� Ÿ�Ͽ� ���� ��� ������ ����
	std::list<Node*>::iterator iter = g_openList.begin();
	Node* popNode = *iter;
	g_openList.pop_front();

	// closeList�� �ٽ� �ִ´�
	g_closeList.push_front(popNode);

	if (g_map[popNode->_y][popNode->_x] != nColor::START &&
		g_map[popNode->_y][popNode->_x] != nColor::END)
	{
		g_map[popNode->_y][popNode->_x] = nColor::CLOSE;
	}

	// ���ΰ�
	if (popNode->_x == g_endPos._x &&
		popNode->_y == g_endPos._y)
	{
		g_endPos._parent = popNode;

		// ã�� ��� �׸��� ���� ����
		return true;
	}

	// 8���� ��� ����
	makeNode(popNode);

	// f���� ������ ������
	g_openList.sort([](Node* a, Node* b)
	{
		// true �� ������, false�� �ڷ�
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

		// ��ġ�ϴ� ��带 ã��
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
			// ������ ã��

			while (1)
			{
				// TODO : g�� ū�� ã��
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
					// TODO : ã��
					// �ش� ����� �θ� localNode(x,y ��ġ�� ������ g�� �� ����)�� �θ�� ����
					(*iter)->_parent = localNode._parent;

					// �θ� �ٲ����� ����
					(*iter)->_g = localNode._g;
					(*iter)->_f = (*iter)->_g + (*iter)->_h;

					// ������.. f���� ������ ������
					g_openList.sort([](Node* a, Node* b)
					{
						// true �� ������, false�� �ڷ�
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

		// ��ġ�ϴ� ��带 ã��
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
			// ������ ã��

			while (1)
			{
				// TODO : g�� ū�� ã��
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
					// TODO : ã��
					// �ش� ����� �θ� localNode(x,y ��ġ�� ������ g�� �� ����)�� �θ�� ����
					(*iter)->_parent = localNode._parent;

					// �θ� �ٲ����� ����
					(*iter)->_g = localNode._g;
					(*iter)->_f = (*iter)->_g + (*iter)->_h;

					// ������.. f���� ������ ������
					g_closeList.sort([](Node* a, Node* b)
					{	// true �� ������, false�� �ڷ�
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

		// ��� ������ ���� ���¸���Ʈ�� �ִ´�
		Node* newNode = new Node();
		newNode->_parent = localNode._parent;
		newNode->_x = localNode._x;
		newNode->_y = localNode._y;
		newNode->_g = localNode._g;
		newNode->_h = localNode._h;
		newNode->_f = localNode._f;

		g_openList.push_front(newNode);

		// ������
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