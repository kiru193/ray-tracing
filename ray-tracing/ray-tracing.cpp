// ray-tracing.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "framework.h"
#include "ray-tracing.h"
#include <windows.h>
#include <Mmsystem.h>
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
int TypeStart(HWND);

TCHAR szClassName[] = TEXT("ket01");
TCHAR szMondai[32], szInput[32], szCheck[32];
int iMon;
DWORD dwStart, dwEnd;
BOOL bStart = FALSE, bSeikai = TRUE;

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
	static HMENU hMenu;
	MMTIME mm;

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
		    hMenu = GetMenu(hWnd);
			break;
	case WM_CHAR:
		if (wParam == VK_SPACE && !bStart) {
			bStart = TRUE;
			TypeStart(hWnd);
			break;
        }
		if (bStart == FALSE)
			return DefWindowProc(hWnd, message, wParam, lParam);
		if (wParam == VK_ESCAPE) {
			wsprintf(szMondai, TEXT(" "));
			lstrcpy(szInput, TEXT(" "));
			lstrcpy(szCheck, TEXT(" "));
			InvalidateRect(hWnd, NULL, TRUE);
			bStart = FALSE;
			break;
        }
		wsprintf(szInput, TEXT("あなたの入力 = \"%c\" "), (int)wParam);
		if (iMon == (int)wParam) {
			bSeikai = TRUE;

            mm.wType = TIME_MS;
			timeGetSystemTime(&mm, sizeof(MMTIME));
			dwEnd = mm.u.ms;

            wsprintf(szCheck, TEXT("反応時間[%d ミリ秒]"), dwEnd - dwStart);
			TypeStart(hWnd);
		}
		else {
			bSeikai = FALSE;
			MessageBeep(MB_OK);
			lstrcpy(szCheck, TEXT("タイプミス！"));
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
    case WM_PAINT:
        {
            hdc = BeginPaint(hWnd, &ps);
            // TODO: HDC を使用する描画コードをここに追加してください.
			TextOut(hdc, 0, 0, szMondai, lstrlen(szMondai));
			TextOut(hdc, 0, 40, szInput, lstrlen(szInput));
			if (bSeikai)
				SetTextColor(hdc, RGB(0, 0, 0));
			else
				SetTextColor(hdc, RGB(255, 0, 0));
			TextOut(hdc, 0, 80, szCheck, lstrlen(szCheck));

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

int TypeStart(HWND hWnd) {
	int n;
	MMTIME mm;

    n = rand() % 26;
	iMon = 'a' + n;
	wsprintf(szMondai, TEXT("問題 = \"%c\" "), iMon);

    mm.wType = TIME_MS;
	timeGetSystemTime(&mm, sizeof(MMTIME));
	dwStart = mm.u.ms;
	
    InvalidateRect(hWnd, NULL, TRUE);
	return 0;
}