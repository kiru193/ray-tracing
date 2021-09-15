﻿// ray-tracing.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "framework.h"
#include "ray-tracing.h"
#include <time.h>
#include <windows.h>

#define MAX_LOADSTRING 100

// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名
//二つ目のウィンドウのパラメータ
LPCWSTR szWindowClass2 = L"Second Window";

// このコード モジュールに含まれる関数の宣言を転送します:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

ATOM MyRegisterClass2(HINSTANCE hInstance);
LRESULT CALLBACK WndProc2(HWND, UINT, WPARAM, LPARAM);
	
HMONITOR hMonitor;//モニター情報を取得するためのハンドル
MONITORINFOEX MonitorInfoEx;
POINT pt = { 1920, 0 };
RECT rc = { 1920, 0, 2*1919, 1080 };

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: ここにコードを挿入してください。
	MyRegisterClass2(hInstance);

    hMonitor = MonitorFromPoint(pt, MONITOR_DEFAULTTONEAREST);
	//hMonitor = MonitorFromRect(&rc, MONITOR_DEFAULTTONEAREST);
	MonitorInfoEx.cbSize = sizeof(MonitorInfoEx);
	GetMonitorInfo(hMonitor, &MonitorInfoEx);

    // グローバル文字列を初期化する
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_RAYTRACING, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // アプリケーション初期化の実行:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_RAYTRACING));

    MSG msg;

    // メイン メッセージ ループ:
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
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RAYTRACING));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_RAYTRACING);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}
//二つ目のウィンドクラスの登録
ATOM MyRegisterClass2(HINSTANCE hInstance) {
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc2;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RAYTRACING));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 3);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_RAYTRACING);
	wcex.lpszClassName = szWindowClass2;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}
//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // グローバル変数にインスタンス ハンドルを格納する
 
   HWND hWnd = CreateWindowW(szWindowClass,	  //クラス名
	   TEXT("しょ～きが～めん～"),			  //ウィンドウ名
	   WS_OVERLAPPEDWINDOW,					  //ウィンドウスタイル
	   CW_USEDEFAULT,						  //X位置
	   0,									  //Y位置
	   CW_USEDEFAULT,						  //ウィンドウ幅
	   0,									  //ウィンドウ高さ
	   nullptr,								  //親ウィンドウのハンドル，親を作るときはnullptr
	   nullptr,								  //メニューハンドル，クラスメニューを使うときはnullptr
	   hInstance,							  //インスタンスハンドル
	   nullptr);							  //ウィンドウ作成データ

   HWND hWnd2 = CreateWindowW(szWindowClass2, //クラス名
	   TEXT("しょ～きが～めん～"),			//ウィンドウ名
	   WS_OVERLAPPEDWINDOW,					//ウィンドウスタイル
	   MonitorInfoEx.rcMonitor.left,		//X位置
	   MonitorInfoEx.rcMonitor.top,			//Y位置
	   MonitorInfoEx.rcMonitor.right,		//ウィンドウ幅
	   MonitorInfoEx.rcMonitor.bottom,		//ウィンドウ高さ
	   nullptr,								//親ウィンドウのハンドル，親を作るときはnullptr
	   nullptr,								//メニューハンドル，クラスメニューを使うときはnullptr
	   hInstance,							//インスタンスハンドル
	   nullptr);							//ウィンドウ作成データ

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   ShowWindow(hWnd2, nCmdShow);
   UpdateWindow(hWnd2);
   return TRUE;
}

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND  - アプリケーション メニューの処理
//  WM_PAINT    - メイン ウィンドウを描画する
//  WM_DESTROY  - 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
	HDC hdc_men;
	HBITMAP hBmp;
	BITMAP bmp_info;
	int w, h;


    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 選択されたメニューの解析:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_CREATE:
        srand((unsigned)time(NULL));
        break;
    case WM_PAINT:
        {
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: HDC を使用する描画コードをここに追加してください.
			hBmp = LoadBitmap(hInst, TEXT("MYBMP"));
			GetObject(hBmp, (int)sizeof(BITMAP), &bmp_info);//bitmap画像の取得
			w = bmp_info.bmWidth;
			h = bmp_info.bmHeight;
			hdc_men = CreateCompatibleDC(hdc);//メモリデバイスコンテキストやらを生成
			SelectObject(hdc_men, hBmp);
			BitBlt(hdc, (MonitorInfoEx.rcMonitor.right - MonitorInfoEx.rcMonitor.left) / 2 - w / 2, (MonitorInfoEx.rcMonitor.bottom - MonitorInfoEx.rcMonitor.top) / 2 - h / 2, w, h, hdc_men, 0, 0, SRCCOPY);
			//StretchBlt(hdc, w, 0, w * 2, h * 2, hdc_men, 0, 0, w, h, SRCCOPY);
			DeleteDC(hdc_men);
			DeleteObject(hBmp);

            EndPaint(hWnd, &ps);
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

//二つ目のウィンドウのCallBack
LRESULT CALLBACK WndProc2(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message) {
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 選択されたメニューの解析:
		switch (wmId) {
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 描画コードをここに追加してください...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// バージョン情報ボックスのメッセージ ハンドラーです。
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
