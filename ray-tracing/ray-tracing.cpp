// ray-tracing.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "framework.h"
#include "ray-tracing.h"
#include <time.h>

#define MAX_LOADSTRING 100

#define BMPW 300
#define BMPH 50

// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名

// このコード モジュールに含まれる関数の宣言を転送します:
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

    int id;
	static HDC hdc_men;
	HBITMAP hBmp;
	TCHAR szBuf[64], szUmu[8], szFormat[] = TEXT("(x = %d,y = %d)Capture = %s");
	static BOOL bCap;
	POINT pts;

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
        {
			hdc = GetDC(hWnd);
		    hdc_men = CreateCompatibleDC(NULL);
			hBmp = CreateCompatibleBitmap(hdc, BMPW, BMPH);
			SelectObject(hdc_men, hBmp);
			//PatBlt(hdc_men, 0, 0, BMPW, BMPH,WHITENESS);
			wsprintf(szBuf, szFormat, 0, 0, TEXT("なし"));
			TextOut(hdc_men, 10, 10, szBuf, lstrlen(szBuf));

            ReleaseDC(hWnd, hdc);
			DeleteObject(hBmp);
	    }
			break;
	case WM_LBUTTONDOWN:
			if (!bCap) {
            
                SetCapture(hWnd);
				bCap = TRUE;
            }
			pts.x = LOWORD(lParam);
			pts.y = HIWORD(lParam);

            wsprintf(szBuf, szFormat, pts.x, pts.y, TEXT("有り"));
			//PatBlt(hdc_men, 0, 0, BMPW, BMPH,WHITENESS);
			TextOut(hdc_men, 10, 10, szBuf, lstrlen(szBuf));
			InvalidateRect(hWnd, NULL, FALSE);

        break;
	case WM_LBUTTONUP:
		if (bCap) {
			ReleaseCapture();
			bCap = FALSE;
		}

		pts.x = LOWORD(lParam);
		pts.y = HIWORD(lParam);
		
        wsprintf(szBuf, szFormat, pts.x, pts.y, TEXT("無し"));
		//PatBlt(hdc_men, 0, 0, BMPW, BMPH, WHITENESS);
		TextOut(hdc_men, 10, 10, szBuf, lstrlen(szBuf));
		InvalidateRect(hWnd, NULL, FALSE);
		
        break;
	case WM_MOUSEMOVE:
		pts.x = LOWORD(lParam);
		pts.y = HIWORD(lParam);
		if (bCap) {
			lstrcpy(szUmu, TEXT("有り"));
		}
		else {
			lstrcpy(szUmu, TEXT("無し"));
        }
		wsprintf(szBuf, szFormat, pts.x, pts.y, szUmu);
		//PatBlt(hdc_men, 0, 0, BMPW, BMPH, WHITENESS);
		TextOut(hdc_men, 10, 10, szBuf, lstrlen(szBuf));
		InvalidateRect(hWnd, NULL, FALSE);
		break;
    case WM_PAINT:
        {
            hdc = BeginPaint(hWnd, &ps);
            // TODO: HDC を使用する描画コードをここに追加してください.
			BitBlt(hdc, 0, 0, BMPW, BMPH, hdc_men, 0, 0, SRCCOPY);
            EndPaint(hWnd, &ps);
        }
        break;
	case WM_CLOSE: 
        {
			id = MessageBox(hWnd, TEXT("終了するぞ？"), TEXT("確認！"), MB_YESNO | MB_ICONQUESTION);
		if (id == IDYES) {
				DestroyWindow(hWnd);
        }
	    }
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