// RegService.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "RegService.h"
#include <io.h>
#define MAX_LOADSTRING 100

#include "../ServiceCommon/ServiceCommon.h"



#define	DEF_PROGRAM_NAME		_T("ServiceProgram.exe")



// ���� �����Դϴ�.
HINSTANCE hInst;								// ���� �ν��Ͻ��Դϴ�.
TCHAR szTitle[MAX_LOADSTRING];					// ���� ǥ���� �ؽ�Ʈ�Դϴ�.
TCHAR szWindowClass[MAX_LOADSTRING];			// �⺻ â Ŭ���� �̸��Դϴ�.

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{


 	// TODO: ���⿡ �ڵ带 �Է��մϴ�.
	MSG msg;
	HACCEL hAccelTable;

	// ���� ���ڿ��� �ʱ�ȭ�մϴ�.
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_REGSERVICE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_REGSERVICE);

	// �⺻ �޽��� �����Դϴ�.
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
// �Լ�: MyRegisterClass()
//
// ����: â Ŭ������ ����մϴ�.
//
// ����:
//
// Windows 95���� �߰��� 'RegisterClassEx' �Լ����� ����
// �ش� �ڵ尡 Win32 �ý��۰� ȣȯ�ǵ���
// �Ϸ��� ��쿡�� �� �Լ��� ����մϴ�. �� �Լ��� ȣ���ؾ�
// �ش� ���� ���α׷��� �����
// '�ùٸ� ������' ���� �������� ������ �� �ֽ��ϴ�.
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
// �Լ�: InitInstance(HANDLE, int)
//
// ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
// ����:
//
// �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
// �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

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
// �Լ�: WndProc(HWND, unsigned, WORD, LONG)
//
// ����: �� â�� �޽����� ó���մϴ�.
//
// WM_COMMAND	- ���� ���α׷� �޴��� ó���մϴ�.
// WM_PAINT	- �� â�� �׸��ϴ�.
// WM_DESTROY	- ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
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

			font = CreateFont( 12, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, _T("����") );
			SendMessage( g_hBstart, WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0) );
			SendMessage( g_hBend  , WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0) );

		}
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam); 
		wmEvent = HIWORD(wParam); 
		// �޴��� ���� ������ ���� �м��մϴ�.
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


				SERVICE_DESCRIPTION lpDes;  //���� ��� ����   
				TCHAR Desc[1024];        //���� ����   

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
					SERVICE_NAME, //���� ���� �̸�   
					SERVICE_NAME,//"������ �ܺ� ǥ�� �̸�",   
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
					//GetDlgItem(IDC_STATIC)->SetWindowText("���� ���� : ��ġ�Ǿ� �����ʽ��ϴ�");   
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
		// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
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

// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
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
