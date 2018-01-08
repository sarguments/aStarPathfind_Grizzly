#pragma once

#define GRID_LEN 20
#define GRID_NUM 28

enum nColor
{
	NONE,
	WALL,
	START,
	END,
	OPEN,
	CLOSE
};

struct Node
{
	int _x = -999;
	int _y = -999;
	Node* _parent = nullptr;
	float _g = -999.f;
	float _h = -999.f;
	float _f = -999.f;
};

extern char g_map[GRID_NUM][GRID_NUM * 2];
extern std::list<Node*> g_openList;// -Heap..F ���� ���... x, y �˻�
extern std::list<Node*> g_closeList; // -List..x, y �˻�
extern Node g_startPos;
extern Node g_endPos;
extern HWND g_hWnd;

bool PathFind(int Sx, int Sy, int Ex, int Ey);
bool CheckTile(int X, int Y);

bool checkRange(int X, int Y);
void makeNode(Node* parent);
void in_makeNode(Node * parent, int X, int Y);
void releaseList(void);

/*
	g = ���۳�� ������ �Ÿ�
	h = ������ ��� ������ �Ÿ�(����ź)
	f = g + h

	1. '���� ���'��  '���¸���Ʈ'�� �ϳ� �ְ�

	2.  �� �ؼ� �������� 'Ÿ�� üũ' �� ���� ���� ��尡 �̹� ���� / Ŭ���� ����Ʈ�� �ִ��� Ȯ�� �� ������ 
		���¸���Ʈ�� ��� �����ؼ� �ְ� ���¸���Ʈ�� f���� ���� ������ ����.���� ���� Ŭ���� ����Ʈ�� �ִ´�.

	3. 1.�� �ݺ�

	4. ���� 2.���� '���� ������ ����� ��ġ�� �ش��ϴ� ���'�� �̹� ���� / Ŭ���� ����Ʈ�� ������ ��带 ���� 
		������ ���� '�̹� ����Ʈ�� �ִ� ���'�� g���� �� ū�� üũ.

	5. '�̹� ����Ʈ�� �ִ� ���'�� g���� �� ũ�ٸ� '���� ���� ���� ������ ��ġ�� �θ���'(�θ� ��带 ���ϴ� ��)��
		�� �������κ��� ����� ��(���� ��)�̹Ƿ� '�̹� ����Ʈ�� �ִ� �����' �θ� '���� ���� ���� ������ ��ġ'�� 
		�θ�� �����Ѵ�.�׸��� ������ �θ� �������� g, f ���� ����.

	6. ���� ����Ʈ�� ����� 0�̰ų� �� �������� ��尡 ������ ����� ����
*/