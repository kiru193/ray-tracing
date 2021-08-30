// ray-tracing.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "framework.h"
#include "ray-tracing.h"
#include <time.h>

#define MAX_LOADSTRING 100

// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名

// このコード モジュールに含まれる関数の宣言を転送します:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

LPCTSTR lpszClasName = TEXT("drag01");
BOOL MyAdjustWindow(HWND, int, int);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: ここにコードを挿入してください。

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

   HWND hWnd = CreateWindowW(szWindowClass, TEXT("しょ～きが～めん～"), WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
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
	HDC hdc;
    PAINTSTRUCT ps;

    int id, rcw = 80, rch = 40, mousex, mousey;
	static int catw, cath;
	static HDC hdc_mem, hdc_cat;
	HBITMAP hBmp;
	static HBITMAP hCat;
	static RECT rc1, rc2;
	static BOOL bDrag, bDragCat;
	static POINT ptStart;
	BITMAP bmp_info;

    switch (message) {
	case WM_COMMAND: {
		int wmId = LOWORD(wParam);
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
	} break;
	case WM_CREATE:
		MyAdjustWindow(hWnd, 600, 400);

		hdc_mem = CreateCompatibleDC(NULL);
		hdc = GetDC(hWnd);
		hBmp = CreateCompatibleBitmap(hdc, 600, 400);
		SelectObject(hdc_mem, hBmp);
		PatBlt(hdc_mem, 0, 0, 600, 400, WHITENESS);
		rc1.left = 10;
		rc1.top = 10;
		rc1.right = rc1.left + rcw;
		rc1.bottom = rc1.top + rch;
		Rectangle(hdc_mem, rc1.left, rc1.top, rc1.right, rc1.bottom);
		hCat = (HBITMAP)LoadImage(hInst, TEXT("MANA1"), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
		GetObject(hCat, (int)sizeof(bmp_info), &bmp_info);
		catw = bmp_info.bmWidth;
		cath = bmp_info.bmHeight;
		hdc_cat = CreateCompatibleDC(NULL);
		SelectObject(hdc_cat, hCat);
		rc2.left = 100;
		rc2.top = 100;
		rc2.right = rc2.left + catw;
		rc2.bottom = rc2.top + cath;
		BitBlt(hdc_mem, rc2.left, rc2.top, catw, cath, hdc_cat, 0, 0, SRCCOPY);

		DeleteObject(hBmp);
		ReleaseDC(hWnd, hdc);
		break;
	case WM_PAINT: {
		hdc = BeginPaint(hWnd, &ps);
		// TODO: HDC を使用する描画コードをここに追加してください.
		BitBlt(hdc, 0, 0, 600, 400, hdc_mem, 0, 0, SRCCOPY);
		EndPaint(hWnd, &ps);
	} break;
	case WM_LBUTTONDOWN: {
		mousex = LOWORD(lParam);
		mousey = HIWORD(lParam);
		if (mousex > rc1.left && mousex < rc1.right && mousey > rc1.top && mousey < rc1.bottom)
			bDrag = TRUE;

		if (mousex > rc2.left && mousex < rc2.right && mousey > rc2.top && mousey < rc2.bottom)
			bDragCat = TRUE;

		if (!bDrag && !bDragCat)
			return DefWindowProc(hWnd, message, wParam, lParam);

		ptStart.x = mousex;
		ptStart.y = mousey;
		SetCapture(hWnd);
		break;
	}
	case WM_MOUSEMOVE:
		if (!bDrag && !bDragCat)
			return DefWindowProc(hWnd, message, wParam, lParam);

		mousex = LOWORD(lParam);
		mousey = HIWORD(lParam);
		if (bDrag) {
			rc1.left += mousex - ptStart.x;
			rc1.top += mousey - ptStart.y;
			rc1.right = rc1.left + rcw;
			rc1.bottom = rc1.top + rch;
		}
		if (bDragCat) {
			rc2.left += mousex - ptStart.x;
			rc2.top += mousey - ptStart.y;
			rc2.right = rc2.left + catw;
			rc2.bottom = rc2.top + cath;
		}
		PatBlt(hdc_mem, 0, 0, 600, 400, WHITENESS);
		Rectangle(hdc_mem, rc1.left, rc1.top, rc1.right, rc1.bottom);
		BitBlt(hdc_mem, rc2.left, rc2.top,catw,cath,hdc_cat,0,0,SRCCOPY);
		InvalidateRect(hWnd, NULL, FALSE);
		ptStart.x = mousex;
		ptStart.y = mousey;	
		break;
	case WM_LBUTTONUP:
		if (!bDrag && !bDragCat)
			return DefWindowProc(hWnd, message, wParam, lParam);
		ReleaseCapture();
		if (bDrag)
			bDrag = FALSE;
		if (bDragCat)
			bDragCat = FALSE;
		break;
	case WM_CLOSE:
		id = MessageBox(hWnd, TEXT("終了しても？"), TEXT("かっくに―ん"), MB_YESNO | MB_ICONQUESTION);
		if (id == IDYES)
			DestroyWindow(hWnd);
		break;
    case WM_DESTROY:
		if (bDrag || bDragCat)
			ReleaseCapture();
		DeleteObject(hCat);
		DeleteDC(hdc_mem);
		DeleteDC(hdc_cat);
		PostQuitMessage(0);

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

BOOL MyAdjustWindow(HWND hWnd, int bmpw, int bmph) {
	RECT rc;
	int x, y, w, h, winx, winy;

	w = GetSystemMetrics(SM_CXSCREEN);
	h = GetSystemMetrics(SM_CYSCREEN);

	rc.left = 0;
	rc.right = bmpw;
	rc.top = 0;
	rc.bottom = bmph;
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	winx = rc.right - rc.left;
	winy = rc.bottom - rc.top;
	x = (w - winx) / 2;
	y = (h - winy) / 2;
	MoveWindow(hWnd, x, y, winx, winy, FALSE);
	return TRUE;
}