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
INT_PTR CALLBACK MyDlgProc(HWND, UINT, WPARAM, LPARAM);

HWND hDlg;
TCHAR szName[32];
HWND hMain;

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
	BOOL bRet;

    // メイン メッセージ ループ:
    while ((bRet = GetMessage(&msg, nullptr, 0, 0)) != 0)
    {
		if (bRet == -1) {
			break;
		}
		else {
			if (!hDlg || !IsDialogMessage(hDlg, &msg)) {
				if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
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
   hMain = hWnd;
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
	TCHAR szBuf[64];

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
			case IDM_END:
				SendMessage(hWnd, WM_CLOSE, 0, 0);
				break;
			case IDM_DLG:
				//DialogBox(hInst, MAKEINTRESOURCE(MYDLG), hWnd, MyDlgProc);
				hDlg = CreateDialog(hInst, MAKEINTRESOURCE(MYDLG), hWnd, MyDlgProc);
				ShowWindow(hDlg, SW_NORMAL);
				break;
			case IDM_CLOSEDLG:
				DestroyWindow(hDlg);
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
		} break;
	case WM_CLOSE:
		if (IsWindow(hWnd)) {
			MessageBox(hWnd, TEXT("ダイアログを破棄します"), TEXT("破棄"), MB_OK);
			DestroyWindow(hDlg);
        }
		DestroyWindow(hWnd);
        break;
	case WM_CREATE:
		hMenu = GetMenu(hWnd);
		break;
	case WM_INITMENU:
		if (IsWindow(hDlg)) {
			EnableMenuItem(hMenu, IDM_DLG, MF_BYCOMMAND | MF_GRAYED);
			EnableMenuItem(hMenu, IDM_CLOSEDLG, MF_BYCOMMAND | MF_ENABLED);

        }
		else {
			EnableMenuItem(hMenu, IDM_DLG, MF_BYCOMMAND | MF_ENABLED);
			EnableMenuItem(hMenu, IDM_CLOSEDLG, MF_BYCOMMAND | MF_GRAYED);
        }
		DrawMenuBar(hWnd);
        break;

	case WM_PAINT: {
		if (lstrcmp(szName, TEXT("")) == 0) {
			lstrcpy(szBuf, TEXT("まだ名前の入力が有りません"));
		}
		else {
			wsprintf(szBuf, TEXT("入力された氏名は%sさんです"), szName);
		}
        hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, 10, 10, szBuf, lstrlen(szBuf));
		
		// TODO: HDC を使用する描画コードをここに追加してください.
		EndPaint(hWnd, &ps);
	} break;

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

INT_PTR CALLBACK MyDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {	
	static HWND hParent;
	switch (message) {
		case WM_INITDIALOG:
			hParent = GetParent(hDlg);
			return (INT_PTR)TRUE;
			
		case WM_COMMAND: {
			switch (LOWORD(wParam)) {
			case IDOK:
				GetDlgItemText(hDlg, IDC_EDIT1, szName, (int)sizeof(szName) - 1);
				DestroyWindow(hDlg);
				InvalidateRect(hParent, NULL, TRUE);
				return TRUE;
			case IDCANCEL:
				SetDlgItemText(hDlg, IDC_EDIT1, TEXT(""));
				return TRUE;
			case IDC_CLOSE:
				DestroyWindow(hDlg);
				return TRUE;
			}
		}
	return FALSE;
    }
return FALSE;	
}
