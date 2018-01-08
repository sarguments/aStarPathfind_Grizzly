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
extern std::list<Node*> g_openList;// -Heap..F 작은 노드... x, y 검색
extern std::list<Node*> g_closeList; // -List..x, y 검색
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
	g = 시작노드 부터의 거리
	h = 목적지 노드 까지의 거리(맨하탄)
	f = g + h

	1. '시작 노드'를  '오픈리스트'에 하나 넣고

	2.  팝 해서 여덟방향 '타일 체크' 후 새로 넣을 노드가 이미 오픈 / 클로즈 리스트에 있는지 확인 후 없으면 
		오픈리스트에 노드 생성해서 넣고 오픈리스트를 f값이 작은 순으로 정렬.팝한 노드는 클로즈 리스트에 넣는다.

	3. 1.을 반복

	4. 만약 2.에서 '새로 생성할 노드의 위치에 해당하는 노드'가 이미 오픈 / 클로즈 리스트에 있으면 노드를 새로 
		만들지 말고 '이미 리스트에 있는 노드'의 g값이 더 큰지 체크.

	5. '이미 리스트에 있는 노드'의 g값이 더 크다면 '내가 원래 새로 생성할 위치의 부모노드'(부모 노드를 통하는 길)가
		더 시작으로부터 가까운 길(좋은 길)이므로 '이미 리스트에 있는 노드의' 부모를 '내가 원래 새로 생성할 위치'의 
		부모로 변경한다.그리고 변경한 부모를 바탕으로 g, f 값도 갱신.

	6. 오픈 리스트의 사이즈가 0이거나 팝 했을때의 노드가 목적지 노드라면 종료
*/