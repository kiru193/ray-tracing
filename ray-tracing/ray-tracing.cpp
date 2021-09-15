// ray-tracing.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "framework.h"
#include "ray-tracing.h"
#include <time.h>
#include <windows.h>
#include <windowsx.h>

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

TCHAR szText[6][64];
int nSex, nNation, n20, nPet, nAddress, nPos = 50;

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
    while (GetMessage(&msg,nullptr,0,0))
    {
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
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
			case IDM_DLG:
				//MessageBox(hWnd, TEXT("選択はされました"), TEXT("謎"), MB_OK);
				DialogBox(hInst, MAKEINTRESOURCE(My_DLG), hWnd, MyDlgProc);
				break;
			case IDM_END:
				SendMessage(hWnd, WM_CLOSE, 0, 0);
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
		} break;
	case WM_PAINT: {
		BeginPaint(hWnd, &ps);
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
	static HWND hMale, hFemale, hJapan, hNot_japan;
	static HWND hTwenty_over,hPrefecture, hAnimal, hPhone_num, hScroll;
	TCHAR szBuf[64];
	TCHAR szAnimal[5][16] = { TEXT("飼っていない"),
		TEXT("犬"),
		TEXT("猫"),
		TEXT("ネズミ"),
		TEXT("馬") };
	TCHAR szPrefecture[6][16] = { TEXT("北海道"),
		TEXT("本州"),
		TEXT("四国"),
		TEXT("九州"),
		TEXT("沖縄"),
		TEXT("その他") };
	static HWND hMain;
	int id, n;
	BOOL bSuccess;

    switch (message) {
	case WM_HSCROLL:
		if (lParam != (LPARAM)hScroll)
			return FALSE;
		switch (LOWORD(wParam)) {
		case SB_LEFT:
			nPos = 0;
			break;
		case SB_RIGHT:
			nPos = 100;
			break;
		case SB_LINELEFT:
			nPos--;
			if (nPos < 0)
				nPos = 0;
            break;
		case SB_LINERIGHT:
			nPos++;
			if (nPos > 100)
				nPos = 100;
			break;
		case SB_PAGELEFT:
			nPos -= 10;
			if (nPos < 0)
				nPos = 0;
			break;
		case SB_PAGERIGHT:
			nPos += 10;
			if (nPos > 100)
				nPos = 100;
			break;
		case SB_THUMBTRACK:
		case SB_THUMBPOSITION:
			nPos = HIWORD(wParam);
			break;
		default:
			break;
		}
		//スクロールバーの移動
		ScrollBar_SetPos(hScroll, nPos, TRUE);

		wsprintf(szBuf, TEXT("%03d"), nPos);
		Edit_SetText(hPhone_num,szBuf);
		wsprintf(szText[5], TEXT("スクロールバーの位置 = %03d", nPos));
		InvalidateRect(hMain, NULL, TRUE);
		return TRUE;
	case WM_INITDIALOG:
		//親ウィンドウのハンドル取得
		hMain = GetParent(hDlg);
		//各コントロールのハンドル取得
		hMale = GetDlgItem(hDlg, male);
		hFemale = GetDlgItem(hDlg, female);
		hJapan = GetDlgItem(hDlg, japan);
		hNot_japan = GetDlgItem(hDlg, not_japan);
		hTwenty_over = GetDlgItem(hDlg, twenty_over);
		hAnimal = GetDlgItem(hDlg, animal);
		hPrefecture = GetDlgItem(hDlg, prefectures);
		hPhone_num = GetDlgItem(hDlg, phone_num);
		hScroll = GetDlgItem(hDlg, scroll1);
		//スクロールバーの初期設定
		ScrollBar_SetRange(hScroll, 0, 100, TRUE);
		ScrollBar_SetPos(hScroll, nPos, TRUE);
		//ラジオグループの初期設定
		if (nSex == 0)
			Button_SetCheck(hMale,BST_CHECKED);
		else
			Button_SetCheck(hFemale, BST_CHECKED);
		if (nNation == 0)
			Button_SetCheck(hJapan, BST_CHECKED);
		else
			Button_SetCheck(hNot_japan, BST_CHECKED);
		//チェックボックスの設定
		if (n20 == 1)
			Button_SetCheck(hTwenty_over, BST_CHECKED);
		else
			Button_SetCheck(hTwenty_over, BST_CHECKED);
		//コンボボックスへ文字列を与える
		for (n = 0; n < 6; n++)
			ComboBox_AddString(hPrefecture, szPrefecture[n]);
		//リストボックスへ文字列を与える
		for (n = 0; n < 5; n++)
			ListBox_AddString(hAnimal, szAnimal[n]);
		//コンボボックスの初期値
		ComboBox_SetCurSel(hPrefecture, nAddress);
		ListBox_SetCurSel(hAnimal, nPet);
		wsprintf(szBuf, TEXT(" % 03d"), nPet);
		Edit_SetText(hPhone_num, szBuf);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDCANCEL:
			EndDialog(hDlg, IDCANCEL);
			return TRUE;
		case IDOK:
			if (nSex == 0)
				lstrcpy(szText[0], TEXT("男が選択されました"));
			else
				lstrcpy(szText[0], TEXT("女が選択されました"));
			if (nNation == 0)
				lstrcpy(szText[1], TEXT("日本が選択されました"));
			else
				lstrcpy(szText[1], TEXT("日本以外が選択されました"));
			if (n20 == 0)
				lstrcpy(szText[2], TEXT("20歳未満です"));
			else
				lstrcpy(szText[2], TEXT("20歳以上です"));
			wsprintf(szText[3], TEXT("%sが選択されました"), szPrefecture[nAddress]);
			wsprintf(szText[4], TEXT("%sが選択されました"), szAnimal[nPet]);
			wsprintf(szText[5], TEXT("スクロールバーの位置 = %03d"), nPos);
			InvalidateRect(hMain, NULL, TRUE);
			EndDialog(hDlg, IDOK);
			return TRUE;
		case male:
			nSex = 0;
			lstrcpy(szText[0], TEXT("男が選択されました"));
			InvalidateRect(hMain, NULL, TRUE);
			return TRUE;
		case female:
			nSex = 1;
			lstrcpy(szText[0], TEXT("女が選択されました"));
			InvalidateRect(hMain, NULL, TRUE);
			return TRUE;
		case japan:
			nNation = 0;
			lstrcpy(szText[1], TEXT("日本が選択されました"));
			InvalidateRect(hMain, NULL, TRUE);
			return TRUE;
		case not_japan:
			nNation = 1;
			lstrcpy(szText[1], TEXT("日本以外が選択されました"));
			InvalidateRect(hMain, NULL, TRUE);
			return TRUE;
		case twenty_over:
			if (Button_GetCheck(hTwenty_over) == BST_CHECKED) {
				n20 = 1;
				lstrcpy(szText[2], TEXT("20歳以上です"));		
			}
			else {
				n20 = 0;
				lstrcpy(szText[2], TEXT("20歳未満です"));		
			}
			InvalidateRect(hMain, NULL, TRUE);
			return TRUE;
		case prefectures:
			id = ComboBox_GetCurSel(hPrefecture);
			wsprintf(szText[3], TEXT("%sが選択されました"), szPrefecture[id]);
			InvalidateRect(hDlg, NULL, TRUE);
			nAddress = id;
			return TRUE;
		case animal:
			id = ListBox_GetCurSel(hAnimal);
			wsprintf(szText[4], TEXT("%sが選択されました"), szAnimal[id]);
			InvalidateRect(hDlg, NULL, TRUE);
			nPet = id;
			return TRUE;
		case phone_num:
			nPos = GetDlgItemInt(hDlg, phone_num, &bSuccess, FALSE);
			if (nPos > 100)
				nPos = 100;
			if (nPos < 0)
				nPos = 0;
			ScrollBar_SetPos(hScroll, nPos, TRUE);
			wsprintf(szText[5], TEXT("スクロールバーの位置 = %03d"), nPos);
			InvalidateRect(hDlg, NULL, TRUE);
			return TRUE;
		default:
			break;
		}
	default:
		break;
	return (INT_PTR)FALSE;
	}

    return (INT_PTR)FALSE;
}
