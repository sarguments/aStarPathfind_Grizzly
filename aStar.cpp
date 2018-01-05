#include "stdafx.h"
#include "aStar.h"

bool PathFind(int Sx, int Sy, int Ex, int Ey)
{
	//�������� ����� ��� ����, ���¸���Ʈ ����.
	Node* startNode = new Node();
	startNode->_x = g_startPos._x;
	startNode->_y = g_startPos._y;
	startNode->_g = 0.f;
	startNode->_h = (float)abs(g_startPos._x - g_endPos._x) + (float)abs(g_startPos._y - g_endPos._y);
	startNode->_f = startNode->_g + startNode->_h;

	g_openList.push_front(startNode);

	while (1)
	{
		// TODO : �Ϸ� ����
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

		// TODO : ���ΰ�?
		if (popNode->_x == g_endPos._x &&
			popNode->_y == g_endPos._y)
		{
			g_endPos._parent = popNode;

			// TODO : ����..
			return true;
		}

		// 8���� ��� ����
		makeNode(popNode);

		// f���� ������ ������
		g_openList.sort([](Node* a, Node* b)
		{	// true �� ������, false�� �ڷ�
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

		// ��ġ�ϴ� ��带 ã��
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
			// ������ ã��
			delete newNode;
		}
		else
		{
			// TODO : g�� ū�� ã�� , Close��?
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
				// ã��
				(*iter)->_parent = newNode;

				// �θ� �ٲ����� ����
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

		// ��ġ�ϴ� ��带 ã��
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
			// ������ ã��
			delete newNode;
		}
		else
		{
			// TODO : g�� ū�� ã�� , Close��?
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
				// ã��
				(*iter)->_parent = newNode;

				// �θ� �ٲ����� ����
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

		// ��ġ�ϴ� ��带 ã��
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
			// ������ ã��
			delete newNode;
		}
		else
		{
			// TODO : g�� ū�� ã�� , Close��?
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
				// ã��
				(*iter)->_parent = newNode;

				// �θ� �ٲ����� ����
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

		// ��ġ�ϴ� ��带 ã��
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
			// ������ ã��
			delete newNode;
		}
		else
		{
			// TODO : g�� ū�� ã�� , Close��?
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
				// ã��
				(*iter)->_parent = newNode;

				// �θ� �ٲ����� ����
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

		// ��ġ�ϴ� ��带 ã��
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
			// ������ ã��
			delete newNode;
		}
		else
		{
			// TODO : g�� ū�� ã�� , Close��?
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
				// ã��
				(*iter)->_parent = newNode;

				// �θ� �ٲ����� ����
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

		// ��ġ�ϴ� ��带 ã��
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
			// ������ ã��
			delete newNode;
		}
		else
		{
			// TODO : g�� ū�� ã�� , Close��?
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
				// ã��
				(*iter)->_parent = newNode;

				// �θ� �ٲ����� ����
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

		// ��ġ�ϴ� ��带 ã��
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
			// ������ ã��
			delete newNode;
		}
		else
		{
			// TODO : g�� ū�� ã�� , Close��?
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
				// ã��
				(*iter)->_parent = newNode;

				// �θ� �ٲ����� ����
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

		// ��ġ�ϴ� ��带 ã��
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
			// ������ ã��
			delete newNode;
		}
		else
		{
			// TODO : g�� ū�� ã�� , Close��?
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
				// ã��
				(*iter)->_parent = newNode;

				// �θ� �ٲ����� ����
				(*iter)->_g = newNode->_g + 1;
				(*iter)->_h = (float)abs((*iter)->_x - g_endPos._x) + (float)abs((*iter)->_y - g_endPos._y);
				(*iter)->_f = (*iter)->_g + (*iter)->_h;
			}

			g_openList.push_front(newNode);
		}
	}
}