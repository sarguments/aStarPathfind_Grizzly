#include "stdafx.h"
#include "hoxy_header.h"
#include "aStarPathfind_Grizzly.h"
#include "aStar.h"

#define MAX_LOADSTRING 100
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

//////////////////////////////////////////////////////////////////////

char g_map[GRID_NUM][GRID_NUM];

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
HBITMAP g_oldBit;

bool g_isClick = false;
bool g_isLButtonDown = false;
bool g_isRButtonDown = false;

// 마우스 좌표
int g_mouseX = 0;
int g_mouseY = 0;

// 그리기 함수
void InitDraw();
void DrawGrid();
void FlipMemDC(HDC hdc);

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
    if (!InitInstance (hInstance, nCmdShow))
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

    return (int) msg.wParam;
}


void InitDraw()
{
	GetClientRect(g_hWnd, &g_rect);

	g_hdc = GetDC(g_hWnd);
	g_memDC = CreateCompatibleDC(g_hdc);
	g_memBit = CreateCompatibleBitmap(g_hdc, g_rect.right, g_rect.bottom);

	g_oldBit = (HBITMAP)SelectObject(g_memDC, g_memBit);
	DrawGrid();
}

void DrawGrid()
{
	HBRUSH localBrush = GetStockBrush(WHITE_BRUSH);
	FillRect(g_memDC, &g_rect, localBrush);

	for (int i = 0; i < GRID_NUM; i++)
	{
		for (int j = 0; j < GRID_NUM; j++)
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

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ASTARPATHFINDGRIZZLY));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_ASTARPATHFINDGRIZZLY);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
	case WM_MOUSEMOVE:
	{
		g_mouseX = GET_X_LPARAM(lParam);
		g_mouseY = GET_Y_LPARAM(lParam);

		// TODO : 임시 드래그
		if (g_isLButtonDown)
		{
			int xCount = (g_mouseX + (g_mouseX / GRID_LEN)) / GRID_LEN;
			int yCount = (g_mouseY + (g_mouseY / GRID_LEN)) / GRID_LEN;

			HBRUSH localBrush = (HBRUSH)GetStockObject(GRAY_BRUSH);
			HBRUSH oldBrush = (HBRUSH)SelectObject(g_memDC, localBrush);

			// 겹쳐지는 선 길이 만큼 뺀다
			Rectangle(g_memDC, (GRID_LEN * xCount) - xCount, (GRID_LEN * yCount) - yCount,
				GRID_LEN + (xCount * GRID_LEN) - xCount, GRID_LEN + (yCount * GRID_LEN) - yCount);

			SelectObject(g_memDC, oldBrush);

			FlipMemDC(g_hdc);
		}
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

		HBRUSH localBrush = (HBRUSH)GetStockObject(GRAY_BRUSH);
		HBRUSH oldBrush = (HBRUSH)SelectObject(g_memDC, localBrush);

		// 겹쳐지는 선 길이 만큼 뺀다
		Rectangle(g_memDC, (GRID_LEN * xCount) - xCount, (GRID_LEN * yCount) - yCount,
			GRID_LEN + (xCount * GRID_LEN) - xCount, GRID_LEN + (yCount * GRID_LEN) - yCount);

		SelectObject(g_memDC, oldBrush);

		FlipMemDC(g_hdc);
	}
	break;
	case WM_LBUTTONUP:
	{
		wcout << L"L button up" << endl;
		g_isLButtonDown = false;
	}
	break;
	case WM_RBUTTONDOWN:
	{
		wcout << L"R button DOWN" << endl;
		g_isRButtonDown = true;
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

			FlipMemDC(g_hdc);

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
		// TODO : 리소스 해제
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