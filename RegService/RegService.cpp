// RegService.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "RegService.h"
#include <io.h>
#define MAX_LOADSTRING 100

#include "../ServiceCommon/ServiceCommon.h"



#define	DEF_PROGRAM_NAME		_T("ServiceProgram.exe")



// 전역 변수입니다.
HINSTANCE hInst;								// 현재 인스턴스입니다.
TCHAR szTitle[MAX_LOADSTRING];					// 제목 표시줄 텍스트입니다.
TCHAR szWindowClass[MAX_LOADSTRING];			// 기본 창 클래스 이름입니다.

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{


 	// TODO: 여기에 코드를 입력합니다.
	MSG msg;
	HACCEL hAccelTable;

	// 전역 문자열을 초기화합니다.
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_REGSERVICE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 응용 프로그램 초기화를 수행합니다.
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_REGSERVICE);

	// 기본 메시지 루프입니다.
	while (GetMessage(&msg, NULL, 0, 0)) 
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
// 함수: MyRegisterClass()
//
// 목적: 창 클래스를 등록합니다.
//
// 설명:
//
// Windows 95에서 추가된 'RegisterClassEx' 함수보다 먼저
// 해당 코드가 Win32 시스템과 호환되도록
// 하려는 경우에만 이 함수를 사용합니다. 이 함수를 호출해야
// 해당 응용 프로그램에 연결된
// '올바른 형식의' 작은 아이콘을 가져올 수 있습니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_REGSERVICE);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCTSTR)IDC_REGSERVICE;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
// 함수: InitInstance(HANDLE, int)
//
// 목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
// 설명:
//
// 이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
// 주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   hWnd = CreateWindow(szWindowClass, szTitle, WS_CAPTION | WS_CLIPCHILDREN | WS_THICKFRAME ,
      CW_USEDEFAULT, 0, 300, 200, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
// 함수: WndProc(HWND, unsigned, WORD, LONG)
//
// 목적: 주 창의 메시지를 처리합니다.
//
// WM_COMMAND	- 응용 프로그램 메뉴를 처리합니다.
// WM_PAINT	- 주 창을 그립니다.
// WM_DESTROY	- 종료 메시지를 게시하고 반환합니다.
//
//
HWND hWndEdit;
HWND g_hBstart;
HWND g_hBend;
const int WIND_HEIGHT = 140;
#define IDC_BTN_START 0x00ef
#define IDC_BTN_END   0x00ed
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	static HFONT font;

	switch (message) 
	{
	case WM_CREATE:
		{

			g_hBstart	= CreateWindow( _T("button"), _T("Install"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				15, ( WIND_HEIGHT - 60 ), 80, 25, 
				hWnd, (HMENU)IDC_BTN_START, hInst, NULL );

			g_hBend	= CreateWindow( _T("button"), _T("UnInstall"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				180, ( WIND_HEIGHT - 60 ), 80, 25, 
				hWnd, (HMENU)IDC_BTN_END, hInst, NULL );

			hWndEdit = CreateWindow( _T("edit"), _T(""), WS_VISIBLE | WS_CHILD | ES_READONLY | ES_AUTOVSCROLL | ES_MULTILINE   ,
				0, 20, 500, 400 , hWnd, NULL, hInst, NULL);

			font = CreateFont( 12, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, _T("굴림") );
			SendMessage( g_hBstart, WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0) );
			SendMessage( g_hBend  , WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0) );

		}
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam); 
		wmEvent = HIWORD(wParam); 
		// 메뉴의 선택 영역을 구문 분석합니다.
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDC_BTN_START:
			{
				TCHAR SrvPath[MAX_PATH];
				_stprintf_s( SrvPath, _T("%s\\%s"), SERVICE_PATH, DEF_PROGRAM_NAME );


				SERVICE_DESCRIPTION lpDes;  //설명 등록 변수   
				TCHAR Desc[1024];        //설명 저장   

				SC_HANDLE hScm = OpenSCManager ( NULL, NULL, SC_MANAGER_CREATE_SERVICE);   


				if ( hScm == NULL)   
				{   
					MessageBox(hWnd, _T("Need Access Permission"), _T("MESSAGE"), MB_OK);   
					return 0;   
				}   

				//GetCurrentDirectory( MAX_PATH, SrvPath);   

				//strcat(SrvPath, "\\");   
				//strcat(SrvPath, "Test.EXE");   

				#ifdef _UNICODE
					{
						char szTemp[256];
						Wide2Char( SrvPath, szTemp, 256 );
						if( _access(szTemp, 0) != 0 )   
						{   
							CloseServiceHandle(hScm);   
							MessageBox(hWnd, _T("No service file in folder"), _T("MESSAGE"), MB_OK);   
							return 0;   

						}
					}
				#else
					if( _access(SrvPath, 0) != 0 )   
					{   
						CloseServiceHandle(hScm);   
						MessageBox(hWnd,"No service file in folder","MESSAGE",MB_OK);   
						return 0;   

					}
				#endif
				

				SC_HANDLE hSrv = CreateService(   
					hScm,   
					SERVICE_NAME, //서비스 고유 이름   
					SERVICE_NAME,//"서비스의 외부 표시 이름",   
					SERVICE_PAUSE_CONTINUE | SERVICE_CHANGE_CONFIG,   
					SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS,
					SERVICE_AUTO_START,   
					SERVICE_ERROR_IGNORE,   
					SrvPath,   
					NULL, NULL, NULL, NULL, NULL);    

				if(hSrv== NULL )   
					MessageBox(hWnd, _T("Fail Install Service"), _T("MESSAGE"), MB_OK);   

				else  
				{
					_stprintf_s( Desc, _T("Service Test") );
					lpDes.lpDescription = Desc;
					ChangeServiceConfig2(hSrv, SERVICE_CONFIG_DESCRIPTION, &lpDes);   
					MessageBox(hWnd, _T("Install Service Complete."), _T("MESSAGE"), MB_OK);   

					CloseServiceHandle(hSrv);   
				}   

				CloseServiceHandle(hScm);   

			}
			break;

		case IDC_BTN_END:
			{

				SC_HANDLE hScm = OpenSCManager ( NULL, NULL, SC_MANAGER_CREATE_SERVICE);   


				if( hScm == NULL)   
				{   
					MessageBox( hWnd, _T("Need Permission"), _T("MESSAGE"), MB_OK);   
					return 0;   
				}   

				SC_HANDLE hSrv = OpenService( hScm, SERVICE_NAME, SERVICE_ALL_ACCESS);   

				if( hSrv == NULL ){   
					CloseServiceHandle(hScm);   
					MessageBox( hWnd, _T("Need Install service first"), _T("MESSAGE"), MB_OK);
					return 0;   

				}   

				SERVICE_STATUS ss;
				QueryServiceStatus(hSrv, &ss);   
				if (ss.dwCurrentState != SERVICE_STOPPED){   
					ControlService(hSrv, SERVICE_CONTROL_STOP, &ss);   
					Sleep(2000);   
				}   

				if ( DeleteService ( hSrv ))   
				{   

					MessageBox( hWnd, _T("Removed Service."), _T("MESSAGE"), MB_OK);   
					//GetDlgItem(IDC_STATIC)->SetWindowText("현재 상태 : 설치되어 있지않습니다");   
				}   

				else  
				{   

					MessageBox( hWnd, _T("Fail remove service."), _T("MESSAGE"), MB_OK);   
				}   

				CloseServiceHandle(hSrv);   
				CloseServiceHandle(hScm);   

			}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 여기에 그리기 코드를 추가합니다.
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

// 정보 대화 상자의 메시지 처리기입니다.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}
