#include "stdafx.h"
#include "hoxy_header.h"
#include "aStarPathfind_Grizzly.h"
#include "aStar.h"

#define MAX_LOADSTRING 100
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

//////////////////////////////////////////////////////////////////////

char g_map[GRID_NUM][GRID_NUM * 2];

Node g_startPos;
Node g_endPos;

std::list<Node*> g_openList;// -Heap..F 작은 노드... x, y 검색
std::list<Node*> g_closeList; // -List..x, y 검색

// 전역 변수:
HINSTANCE g_hInst;
HWND g_hWnd;
HDC g_hdc;
HDC g_memDC;
HBITMAP g_memBit;
RECT g_rect;
HBRUSH g_oldBrush;
HPEN g_oldPen;
HPEN g_goalPen;
HBITMAP g_oldBit;
HBRUSH g_colorArr[10];

bool g_isClick;
bool g_isLButtonDown;
bool g_isRButtonDown;
bool g_isErase;
bool g_isEnd;

// 마우스 좌표
int g_mouseX;
int g_mouseY;

// 그리기 함수
void InitDraw(void);
void DrawGrid(void);
void FlipMemDC(HDC hdc);
void DrawRect(void);
void DrawLine(void);
void DrawClear(void);
void DrawAllClear(void);

//////////////////////////////////////////////////////////////////////

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	CCmdStart myCmd;

	// 전역 문자열을 초기화합니다.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_ASTARPATHFINDGRIZZLY, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 응용 프로그램 초기화를 수행합니다.
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ASTARPATHFINDGRIZZLY));

	MSG msg;

	///////////////////////// 그리기 초기화 //////////////////////////////////
	InitDraw();
	FlipMemDC(g_hdc);
	/////////////////////////////////////////////////////////////////////////

	// 기본 메시지 루프입니다.
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

void InitDraw()
{
	// 색상 초기화
	for (int i = 0; i < GRID_NUM; i++)
	{
		ZeroMemory(g_map[i], GRID_NUM * 2);
	}

	// TODO : 시작, 끝
	g_startPos._x = 3;
	g_startPos._y = 3;

	g_map[g_startPos._y][g_startPos._x] = nColor::START;

	g_endPos._x = 20;
	g_endPos._y = 15;

	g_map[g_endPos._y][g_endPos._x] = nColor::END;

	GetClientRect(g_hWnd, &g_rect);

	g_hdc = GetDC(g_hWnd);
	g_memDC = CreateCompatibleDC(g_hdc);
	g_memBit = CreateCompatibleBitmap(g_hdc, g_rect.right, g_rect.bottom);

	g_oldBit = (HBITMAP)SelectObject(g_memDC, g_memBit);

	g_colorArr[nColor::START] = CreateSolidBrush(RGB(0, 203, 0));
	g_colorArr[nColor::END] = CreateSolidBrush(RGB(238, 68, 0));
	g_colorArr[nColor::OPEN] = CreateSolidBrush(RGB(152, 251, 152));
	g_colorArr[nColor::CLOSE] = CreateSolidBrush(RGB(175, 238, 238));
	g_goalPen = CreatePen(PS_SOLID, 2, RGB(254, 255, 2));

	// 그리드 그리기
	DrawGrid();
	InvalidateRect(g_hWnd, NULL, FALSE);
}

void DrawGrid()
{
	HBRUSH localBrush = GetStockBrush(WHITE_BRUSH);
	FillRect(g_memDC, &g_rect, localBrush);

	for (int i = 0; i < GRID_NUM; i++)
	{
		for (int j = 0; j < GRID_NUM * 2; j++)
		{
			Rectangle(g_memDC, (GRID_LEN * j) - j, (GRID_LEN * i) - i,
				(GRID_LEN + GRID_LEN * j) - j, (GRID_LEN + GRID_LEN * i) - i);
		}
	}
}

void FlipMemDC(HDC hdc)
{
	BitBlt(hdc, 0, 0,
		g_rect.right, g_rect.bottom,
		g_memDC, 0, 0,
		SRCCOPY);
}

void DrawRect(void)
{
	// 배열 검사해서 번호에 맞는 색 사각형 그리기
	HBRUSH localBrush = NULL;
	HBRUSH oldBrush = NULL;

	for (int i = 0; i < GRID_NUM; i++)
	{
		for (int j = 0; j < GRID_NUM * 2; j++)
		{
			switch (g_map[i][j])
			{
			case nColor::START:
			{
				// 초록
				localBrush = g_colorArr[nColor::START];
				oldBrush = (HBRUSH)SelectObject(g_memDC, localBrush);
			}
			break;
			case nColor::END:
			{
				// 빨강
				localBrush = g_colorArr[nColor::END];
				oldBrush = (HBRUSH)SelectObject(g_memDC, localBrush);
			}
			break;
			case nColor::WALL:
			{
				// 회색
				localBrush = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
				oldBrush = (HBRUSH)SelectObject(g_memDC, localBrush);
			}
			break;
			case nColor::OPEN:
			{
				// 연두
				localBrush = g_colorArr[nColor::OPEN];
				oldBrush = (HBRUSH)SelectObject(g_memDC, localBrush);
			}
			break;
			case nColor::CLOSE:
			{
				// 하늘
				localBrush = g_colorArr[nColor::CLOSE];
				oldBrush = (HBRUSH)SelectObject(g_memDC, localBrush);
			}
			break;
			case nColor::NONE:
			{
				localBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
				oldBrush = (HBRUSH)SelectObject(g_memDC, localBrush);
			}
			break;
			}

			// 겹쳐지는 선 길이 만큼 뺀다
			Rectangle(g_memDC, (GRID_LEN * j) - j, (GRID_LEN * i) - i,
				GRID_LEN + (j * GRID_LEN) - j, GRID_LEN + (i * GRID_LEN) - i);

			SelectObject(g_memDC, oldBrush);
		}
	}
}

// 길찾기 완료 선 그리기
void DrawLine(void)
{
	if (g_endPos._parent == nullptr)
	{
		return;
	}

	int endNodeX = (GRID_LEN * g_endPos._x) - g_endPos._x + (GRID_LEN / 2);
	int endNodeY = (GRID_LEN * g_endPos._y) - g_endPos._y + (GRID_LEN / 2);

	int parNodeX = 0;
	int parNodeY = 0;

	g_oldPen = (HPEN)SelectObject(g_memDC, g_goalPen);

	MoveToEx(g_memDC, endNodeX, endNodeY, NULL);

	Node* nowNode = &g_endPos;

	while (nowNode->_parent != nullptr)
	{
		parNodeX = (GRID_LEN * nowNode->_parent->_x) - nowNode->_parent->_x + (GRID_LEN / 2);
		parNodeY = (GRID_LEN * nowNode->_parent->_y) - nowNode->_parent->_y + (GRID_LEN / 2);

		LineTo(g_memDC, parNodeX, parNodeY);
		nowNode = nowNode->_parent;
	}

	SelectObject(g_memDC, g_oldPen);
}

void DrawClear(void)
{
	releaseList();

	// 색상 초기화
	for (int i = 0; i < GRID_NUM; i++)
	{
		for (int j = 0; j < GRID_NUM * 2; j++)
		{
			if (g_map[i][j] == nColor::START ||
				g_map[i][j] == nColor::END ||
				g_map[i][j] == nColor::WALL)
			{
				continue;
			}

			g_map[i][j] = nColor::NONE;
		}
	}

	DrawGrid();

	InvalidateRect(g_hWnd, NULL, FALSE);
}

void DrawAllClear(void)
{
	releaseList();

	// 색상 초기화
	for (int i = 0; i < GRID_NUM; i++)
	{
		for (int j = 0; j < GRID_NUM * 2; j++)
		{
			if (g_map[i][j] == nColor::START ||
				g_map[i][j] == nColor::END)
			{
				continue;
			}

			g_map[i][j] = nColor::NONE;
		}
	}

	DrawGrid();

	InvalidateRect(g_hWnd, NULL, FALSE);
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ASTARPATHFINDGRIZZLY));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_ASTARPATHFINDGRIZZLY);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	g_hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	g_hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!g_hWnd)
	{
		return FALSE;
	}

	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_TIMER:
	{
		g_isEnd = PathFind(g_startPos._x, g_startPos._y, g_endPos._x, g_endPos._y);
		InvalidateRect(g_hWnd, NULL, FALSE);
	}
	break;
	case WM_MOUSEMOVE:
	{
		g_mouseX = GET_X_LPARAM(lParam);
		g_mouseY = GET_Y_LPARAM(lParam);

		if (!g_isLButtonDown)
		{
			break;
		}

		int xCount = (g_mouseX + (g_mouseX / GRID_LEN)) / GRID_LEN;
		int yCount = (g_mouseY + (g_mouseY / GRID_LEN)) / GRID_LEN;

		// 시작, 끝점은 벽 그리기에서 제외
		if (g_map[yCount][xCount] == nColor::START ||
			g_map[yCount][xCount] == nColor::END ||
			!checkRange(xCount, yCount))
		{
			break;
		}

		if (g_map[yCount][xCount] == nColor::WALL &&
			g_isErase == true)
		{
			g_map[yCount][xCount] = nColor::NONE;
		}
		else if (g_isErase == false)
		{
			g_map[yCount][xCount] = nColor::WALL;
		}
		else
		{
			break;
		}

		InvalidateRect(g_hWnd, NULL, FALSE);
		//wcout << L"MOUSE MOVE // x : " << g_mouseX << L"// y : " << g_mouseY << endl;
	}
	break;
	case WM_LBUTTONDOWN:
	{
		wcout << L"L button DOWN" << endl;
		g_isLButtonDown = true;

		// 겹쳐지는 선 길이 만큼 더해서 GRID_LEN 으로 나누기
		int xCount = (g_mouseX + (g_mouseX / GRID_LEN)) / GRID_LEN;
		int yCount = (g_mouseY + (g_mouseY / GRID_LEN)) / GRID_LEN;

		wcout << L"xCount : " << xCount << L" // yCount : " << yCount << endl;

		// 시작, 끝점은 벽 그리기에서 제외
		if (g_map[yCount][xCount] == nColor::START ||
			g_map[yCount][xCount] == nColor::END ||
			!checkRange(xCount, yCount))
		{
			break;
		}

		if (g_map[yCount][xCount] == nColor::WALL)
		{
			g_map[yCount][xCount] = nColor::NONE;
			g_isErase = true;
		}
		else
		{
			g_map[yCount][xCount] = nColor::WALL;
		}

		InvalidateRect(g_hWnd, NULL, FALSE);
	}
	break;
	case WM_LBUTTONDBLCLK:
	{
		// 시작점 설정
		int xCount = (g_mouseX + (g_mouseX / GRID_LEN)) / GRID_LEN;
		int yCount = (g_mouseY + (g_mouseY / GRID_LEN)) / GRID_LEN;

		g_map[g_startPos._y][g_startPos._x] = nColor::NONE;

		g_startPos._x = xCount;
		g_startPos._y = yCount;

		g_map[g_startPos._y][g_startPos._x] = nColor::START;

		wcout << L"L double CLICK" << endl;

		InvalidateRect(g_hWnd, NULL, FALSE);
	}
	break;
	case WM_RBUTTONDBLCLK:
	{
		// 목적지 설정
		int xCount = (g_mouseX + (g_mouseX / GRID_LEN)) / GRID_LEN;
		int yCount = (g_mouseY + (g_mouseY / GRID_LEN)) / GRID_LEN;

		g_map[g_endPos._y][g_endPos._x] = nColor::NONE;

		g_endPos._x = xCount;
		g_endPos._y = yCount;

		g_map[g_endPos._y][g_endPos._x] = nColor::END;

		// TODO : endPos = nullptr, 선그리기 제어용
		g_endPos._parent = nullptr;

		wcout << L"R double CLICK" << endl;

		InvalidateRect(g_hWnd, NULL, FALSE);
	}
	break;
	case WM_LBUTTONUP:
	{
		wcout << L"L button up" << endl;
		g_isLButtonDown = false;
		g_isErase = false;
	}
	break;
	case WM_RBUTTONDOWN:
	{
		wcout << L"R button DOWN" << endl;
		g_isRButtonDown = true;

		DrawClear();

		//시작전에 출발점 노드 생성, 오픈리스트 삽입.
		Node* startNode = new Node();
		startNode->_x = g_startPos._x;
		startNode->_y = g_startPos._y;
		startNode->_g = 0.f;
		startNode->_h = (float)abs(g_startPos._x - g_endPos._x) + (float)abs(g_startPos._y - g_endPos._y);
		startNode->_f = startNode->_g + startNode->_h;

		g_openList.push_front(startNode);

		SetTimer(hWnd, 1, 20, NULL);
	}
	break;
	case WM_RBUTTONUP:
	{
		wcout << L"R button up" << endl;
		g_isRButtonDown = false;
	}
	break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 메뉴 선택을 구문 분석합니다.
		switch (wmId)
		{
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_32772:
		{
			DrawAllClear();
		}
		break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다.
		DrawRect();

		if (g_isEnd)
		{
			DrawLine();
			KillTimer(hWnd, 1);

			g_isEnd = false;
		}

		FlipMemDC(g_hdc);

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		// TODO : 리소스 해제
		DeleteObject(g_colorArr[nColor::START]);
		DeleteObject(g_colorArr[nColor::END]);
		DeleteObject(g_colorArr[nColor::OPEN]);
		DeleteObject(g_colorArr[nColor::CLOSE]);
		DeleteObject(g_goalPen);

		ReleaseDC(g_hWnd, g_hdc);
		DeleteDC(g_memDC);
		DeleteObject(g_memBit);

		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}