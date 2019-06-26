// Кнопка.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "button.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY));

    MSG msg;

    // Main message loop:
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



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      100, 100, 1000, 800, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//

struct moveWindow
{
	HWND button;
	HWND hparent;
	bool mWind;
}moveMyWind;

LRESULT CALLBACK ChildProc(HWND, UINT, WPARAM, LPARAM);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
		moveMyWind.hparent = hWnd;
		moveMyWind.mWind = true;
		WNDCLASS w;
		memset(&w, 0, sizeof(WNDCLASS));
		w.lpfnWndProc = ChildProc;
		w.hInstance = hInst;
		w.hbrBackground = (HBRUSH)(COLOR_WINDOW + 6);
		w.lpszClassName = TEXT("ChildWClass");
		w.hCursor = LoadCursor(nullptr, IDC_ARROW);
		RegisterClass(&w);
		moveMyWind.button = CreateWindow(
			TEXT("ChildWClass"),
			TEXT(""),
			WS_CHILD | WS_BORDER | WS_VISIBLE,
			100, 100,
			85, 20, 
			hWnd,
			(HMENU)(int)(ID_FIRSTCHILD), 
			hInst,
			NULL);
			ShowWindow(moveMyWind.button, SW_NORMAL);
			UpdateWindow(moveMyWind.button);
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
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
            PAINTSTRUCT ps, psbutton;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
			HDC hdcbutton = BeginPaint(moveMyWind.button, &psbutton);
			TextOut(hdcbutton, 0, 0, L"Нажми меня", lstrlen(L"Нажми меня"));
			EndPaint(moveMyWind.button, &psbutton);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void convertMy(HWND w, RECT&r)
{
	POINT tmp;
	tmp.x = r.left;
	tmp.y = r.top;
	ScreenToClient(w, &tmp);
	r.left = tmp.x;
	r.top = tmp.y;
	tmp.x = r.right;
	tmp.y = r.bottom;
	ScreenToClient(w, &tmp);
	r.right = tmp.x;
	r.bottom = tmp.y;
}

void MoveMyWindow(float a, float b)
{
	RECT rc, rec;
	int x = 1, y = 1;
	GetWindowRect(moveMyWind.hparent, &rec);
	srand(time(NULL));
	if (!a)
		a = (rand() % 21 - 10) / 10.;
	if (!b)
		b = (rand() % 21 - 10) / 10.;
	for (int i = 15; i > 0; i--)
	{
		GetWindowRect(moveMyWind.button, &rc);
		convertMy(moveMyWind.hparent, rc);
		if (rc.left < 10)
			x = -1;
		if (rec.right - rec.left < rc.right + 10)
			x = -1;
		if (rc.top < 10)
			y = -1;
		if (rec.bottom - rec.top < rc.bottom + 10)
			y = -1;

		MoveWindow(moveMyWind.button, rc.left + (i * a * x),
			rc.top + (i  * b * y),
			rc.right - rc.left,
			rc.bottom - rc.top, true);
		Sleep(20 - i);
	}
	moveMyWind.mWind = true;
}

LRESULT CALLBACK ChildProc(HWND hwnd, UINT Message, WPARAM wparam, LPARAM lparam)
{
	static POINT pstart;
	RECT rec;
	GetWindowRect(moveMyWind.button, &rec);
	convertMy(moveMyWind.hparent, rec);
	static HANDLE hMoveWindow1, hMoveWindow2, hMoveWindow3, hMoveWindow4;
	pstart.x = LOWORD(lparam);
	pstart.y = HIWORD(lparam);
	switch (Message)
	{
	case WM_MOUSEMOVE:
		if (moveMyWind.mWind)
		{
			if (pstart.x < (rec.right - rec.left) / 2)
			{
				moveMyWind.mWind = false;
				MoveMyWindow(1, 0);
			}
			else if (pstart.x >(rec.right - rec.left) / 2)
			{
				moveMyWind.mWind = false;
				MoveMyWindow(-1, 0);
			}
			else if (pstart.y < (rec.bottom - rec.top) / 2)
			{
				moveMyWind.mWind = false;
				MoveMyWindow(0, 1);
			}
			else if (pstart.y > (rec.bottom - rec.top) / 2)
			{
				moveMyWind.mWind = false;
				MoveMyWindow(0, -1);
			}
		}
		break;
	}
	if (Message == WM_DESTROY)
	{
		return 0;
	}
	return DefWindowProc(hwnd, Message, wparam, lparam);
}
