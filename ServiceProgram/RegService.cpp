#include "StdAfx.h"
#include <io.h>
#include "RegService.h"

SERVICE_STATUS_HANDLE CService::m_hSrv = NULL;
DWORD CService::m_NowState = SERVICE_START_PENDING;
bool CService::m_bPause = false;
bool CService::m_bClosing = false;
HANDLE CService::m_Exit_Event = NULL;
IService* CService::m_Interface = NULL;
TCHAR CService::m_szServiceName[64];



  
// 서비스의 현재 상태를 변경하는 함수
void CService::MySetStatus( DWORD dwState, DWORD dwAccept )
{
	SERVICE_STATUS ss;
	ss.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	ss.dwCurrentState = dwState;
	ss.dwControlsAccepted = dwAccept;
	ss.dwWin32ExitCode = 0;
	ss.dwServiceSpecificExitCode = 0;
	ss.dwCheckPoint = 0;
	ss.dwWaitHint = 0;

	// 현재 상태를 보관해 둔다.
	m_NowState = dwState;
	SetServiceStatus( m_hSrv,&ss );
}


void CService::MyServiceHandler( DWORD fdwControl )
{
	// 현재 상태와 같은 제어 코드일 경우는 처리할 필요 없다.
	if(fdwControl == m_NowState)
		return;

	switch( fdwControl ) 
	{
	case SERVICE_CONTROL_PAUSE:
		MySetStatus( SERVICE_PAUSE_PENDING, 0 );
		m_bPause = true;
		MySetStatus( SERVICE_PAUSED );
		m_Interface->OnPause();
		break;

	case SERVICE_CONTROL_CONTINUE:
		MySetStatus( SERVICE_CONTINUE_PENDING, 0 );
		m_bPause = false;
		MySetStatus( SERVICE_RUNNING );
		m_Interface->OnResume();
		break;

	case SERVICE_CONTROL_STOP:
		MySetStatus( SERVICE_STOP_PENDING, 0 );
		SetEvent(m_Exit_Event);
		//SetEvent(Exit_Event[1]);
		m_bClosing = true;
		break;

	case SERVICE_CONTROL_INTERROGATE:
	default:
		MySetStatus( m_NowState );
		break;
	}

}

void CService::MyServiceMain( DWORD argc, LPTSTR *argv )
{
	// 서비스 핸들러를 등록한다.
	m_hSrv = RegisterServiceCtrlHandler( m_szServiceName, (LPHANDLER_FUNCTION)MyServiceHandler );
	if( 0 == m_hSrv )
		return;

	MySetStatus( SERVICE_START_PENDING );
	m_bPause = false;


	// 필요한 초기화 처리
	m_Interface->OnStart();


	// 서비스가 시작되었음
	MySetStatus( SERVICE_RUNNING );

	while(1)
	{
		if( !CService::m_bPause )
		{
			//실제 처리 부분
			m_Interface->OnIdle();
		}

		// 종료처리시작된상황
		if( CService::m_bClosing )
		{
			m_Interface->OnClose();
			// 실제 종료관련 모든 처리 완료됬다면 break처리
			break;
		}
	}
	MySetStatus( SERVICE_STOPPED );
}

void CService::Start(LPTSTR servicename, IService* pInterface)
{
	m_Interface = pInterface;
	_tcscpy_s( m_szServiceName, 64, servicename );

	SERVICE_TABLE_ENTRY ste[] =
	{
		{ servicename, (LPSERVICE_MAIN_FUNCTION)MyServiceMain },
		{ NULL, NULL }
	};
	StartServiceCtrlDispatcher( ste );
}


CRegService::ErrService CRegService::InstallService(LPTSTR name, LPTSTR exefile, LPTSTR desc)
{
	SERVICE_DESCRIPTION lpDes;  //설명 등록 변수   

	SC_HANDLE hScm = OpenSCManager ( NULL, NULL, SC_MANAGER_CREATE_SERVICE);   

	if( hScm == NULL )
		return ERR_AUTH;

	//GetCurrentDirectory( MAX_PATH, SrvPath);   

	//strcat(SrvPath, "\\");   
	//strcat(SrvPath, "Test.EXE");   

	#ifdef UNICODE
		{
			char szTemp[256];
			Wide2Char( exefile, szTemp, 256 );
			if( _access(szTemp, 0) != 0 )   
			{   
				CloseServiceHandle(hScm);   
				return ERR_PATH;
			}
		}
	#else
		if( _access(exefile, 0) != 0 )   
		{   
			CloseServiceHandle(hScm);   
			return ERR_PATH;
		}
	#endif

	SC_HANDLE hSrv = CreateService(
		hScm,
		name,	// 서비스고유이름
		name,	// 서비스외부표시이름
		SERVICE_PAUSE_CONTINUE | SERVICE_CHANGE_CONFIG,
		SERVICE_WIN32_OWN_PROCESS,
		SERVICE_AUTO_START,
		SERVICE_ERROR_IGNORE,
		exefile,
		NULL, NULL, NULL, NULL, NULL);

	if( hSrv == NULL )
	{
		CloseServiceHandle(hScm);   
		return ERR_FAIL;
	}

	lpDes.lpDescription = desc;
	ChangeServiceConfig2(hSrv, SERVICE_CONFIG_DESCRIPTION, &lpDes);

	CloseServiceHandle(hScm);
	CloseServiceHandle(hSrv);
	return ERR_OK;
}

CRegService::ErrService CRegService::UnInstallService(LPTSTR name)
{
	SC_HANDLE hScm = OpenSCManager ( NULL, NULL, SC_MANAGER_CREATE_SERVICE);

	if( hScm == NULL)
		return ERR_AUTH;

	SC_HANDLE hSrv = OpenService( hScm, name, SERVICE_ALL_ACCESS);

	if( hSrv == NULL )
	{
		CloseServiceHandle(hScm);
		return ERR_NOTINST;
	}

	SERVICE_STATUS ss;
	QueryServiceStatus(hSrv, &ss);
	if (ss.dwCurrentState != SERVICE_STOPPED)
	{
		ControlService(hSrv, SERVICE_CONTROL_STOP, &ss);
		Sleep(2000);
	}

	ErrService err = ERR_OK;
	if( DeleteService( hSrv ) )
	{
		//GetDlgItem(IDC_STATIC)->SetWindowText("현재 상태 : 설치되어 있지않습니다");
	}
	else
	{
		err = ERR_FAIL;
	}

	CloseServiceHandle(hSrv);
	CloseServiceHandle(hScm);
	return err;
}






