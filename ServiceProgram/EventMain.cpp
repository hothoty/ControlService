#include "stdafx.h"
//#include <shellapi.h>
#include "EventMain.h"
#include "../ServiceCommon/Util/Process.h"
#include "../ServiceCommon/Util/String.h"

#include <functional>

//#pragma comment(lib,"shell32.lib")
#pragma comment(lib,"psapi.lib")

#define CLIENT_LOG_FOLDER		_T("Log_Service")

int CEventMain::DeletePath(CString strPath, bool bAllDelete) const
{
	// 경로안의 모든 파일(서브폴더포함) 삭제
	std::function<bool(CString)> DelPath = [&DelPath](CString strPath)
	{
		bool bReturn = true;

		CFileFind finder;
		BOOL bContinue = TRUE;

		if(strPath.Right(1) != _T("\\"))
			strPath += _T("\\");

		strPath += _T("*.*");
		bContinue = finder.FindFile(strPath);
		while(bContinue)
		{
			bContinue = finder.FindNextFile();
			if(finder.IsDots()) // Ignore this item.
			{
				continue;
			}
			else if(finder.IsDirectory()) // Delete all sub item.
			{
				if( DelPath(finder.GetFilePath()) == false )
				{
					//bReturn = false;
				}

				//if( ::RemoveDirectory((LPCTSTR)finder.GetFilePath()) == false )
				//{
				//	//bReturn = false;
				//}
			}
			else // Delete file.
			{
				if( ::DeleteFile((LPCTSTR)finder.GetFilePath()) == false )
					bReturn = false;
			}
		}
		finder.Close();

		strPath = strPath.Left(strPath.ReverseFind('\\'));
		/*if( ::RemoveDirectory((LPCTSTR)strPath) == false )
			bReturn = false;*/

		return bReturn;
	};

	int nDeleteProgram = 0;
	if( bAllDelete )
	{
		for( UINT i=0; i<m_ControlExeList.size(); i++ )
		{
			const ControlProgramExeInfo* pProgramExe = &m_ControlExeList[i];
			if( DelPath(pProgramExe->szPathDir) )
				nDeleteProgram++;
		}
	}
	else
	{
		if( DelPath(strPath) )
			nDeleteProgram++;
	}
	return nDeleteProgram;
}


CEventMain::CEventMain()
{
}

CEventMain::~CEventMain()
{
	Destroy();
}

void CEventMain::OnPause()
{
	SimpleSocket::g_LogWrite.LogSave( _T("OnPause") );
}
void CEventMain::OnResume()
{
	SimpleSocket::g_LogWrite.LogSave( _T("OnResume") );
}

void CEventMain::Destroy()
{
	m_ControlExeList.clear();
}

void CEventMain::OnClose()
{
	Destroy();
	SimpleSocket::g_LogWrite.LogSave( _T("OnClose") );
}

void CEventMain::LoadIni(bool bReload)
{
	TCHAR szTempIni[MAX_PATH];
	_stprintf_s( szTempIni, _T("%s\\%s"), SERVICE_PATH, CLIENT_CONFIG_INI_NAME );

	m_tWakeUpCheck.Reset();
	m_tProcessCheck.Reset();


	/**
	관리해야하는 프로그램 정보
	*/
	const TCHAR* defaultName = _T("CONTROLPROGRAM");
	int nCount = 1;
	TCHAR ControlProgramINI[256];
	m_ControlExeList.clear();
	while(1)
	{
		// 자동 넘버링 처리
		_stprintf_s( ControlProgramINI, _T("%s%d"), defaultName, nCount++ );

		// PATH 읽어오기
		TCHAR szPath[128];
		if( -1 == ( GetINIStr( ControlProgramINI, _T("PATH"), szTempIni, szPath, 128 ) ) )
		{
			SimpleSocket::g_LogWrite.LogSave( _T("OnStart : ControlProgram ini error [%s,%s]"), ControlProgramINI, _T("PATH"));
			break;
		}

		// EXE 읽어오기
		TCHAR szEXE[64];
		if( -1 == ( GetINIStr( ControlProgramINI, _T("EXE"), szTempIni, szEXE, 64 ) ) )
		{
			SimpleSocket::g_LogWrite.LogSave( _T("OnStart : ControlProgram ini error [%s,%s]"), ControlProgramINI, _T("EXE"));
			break;
		}

		// 파라미터 읽어오기
		TCHAR szParam[64];
		if (-1 == (GetINIStr(ControlProgramINI, _T("PARAM"), szTempIni, szParam, 64)))
		{
			SimpleSocket::g_LogWrite.LogSave(_T("OnStart : ControlProgram ini error [%s,%s]"), ControlProgramINI, _T("Param"));
			break;
		}

		// WAKEOn옵션 적용여부
		int nWakeOnMode = 0;
		if( -1 == ( GetINIInt( ControlProgramINI, _T("WAKEOP"), szTempIni, &nWakeOnMode ) ) )
		{
			SimpleSocket::g_LogWrite.LogSave( _T("OnStart : ControlProgram ini error wakeonop [%s]"), nWakeOnMode);
			// 이건 나중에 추가된거니까 없어도 break하지말고 그냥 0으로 간주하고 넘어간다
			nWakeOnMode = 0;
		}

		// UAC SYSTEM권한 적용여부
		int nSystemUAC = 0;
		if( -1 == ( GetINIInt( ControlProgramINI, _T("SYSTEM"), szTempIni, &nSystemUAC ) ) )
		{
			SimpleSocket::g_LogWrite.LogSave( _T("OnStart : ControlProgram ini error system [%s]"), nSystemUAC);
			// 이건 나중에 추가된거니까 없어도 break하지말고 그냥 0으로 간주하고 넘어간다
			nSystemUAC = 0;
		}

		// 식별ID 읽어오기
		TCHAR szID[64];
		if( -1 == ( GetINIStr( ControlProgramINI, _T("PROGRAMID"), szTempIni, szID, 64 ) ) )
		{
			SimpleSocket::g_LogWrite.LogSave( _T("OnStart : ControlProgram ini error [%s,%s]"), ControlProgramINI, _T("PROGRAMID"));
			break;
		}


		ControlProgramExeInfo FullPath;
		_tcscpy_s( FullPath.szPathDir, szPath );
		_stprintf_s( FullPath.szPath, _T("%s\\%s"), szPath, szEXE );
		_stprintf_s( FullPath.szExe, _T("%s"), szEXE );
		_stprintf_s(FullPath.szParam, _T("%s\\%s"), szPath, szParam);
		_tcscpy_s( FullPath.szID, szID );

		FullPath.bWakeOne = false;
		if (nWakeOnMode != 0)
		{
			FullPath.bWakeOption = true;

			if (nWakeOnMode == 2)
				FullPath.bWakeOne = true;
		}
		if( nSystemUAC )
			FullPath.bSystemOption = true;


		SimpleSocket::g_LogWrite.LogSave( _T("OnStart : ControlProgram ini OK :  [%s] [%s] [%s] [%s]"), FullPath.szPath, FullPath.szExe, FullPath.szParam, FullPath.szID);

		m_ControlExeList.push_back(FullPath);

		// 무한루프 방지
		if( nCount >= 10 ) break;
	}

	if( bReload )
	{
		SimpleSocket::g_LogWrite.LogSave( _T("Reloaded Ini File") );
	}
	else
	{
		// 최초 시작
		SimpleSocket::g_LogWrite.LogSave( _T("OnStart ") );
		//Connect();
	}
}

void CEventMain::OnStart()
{
	TCHAR szTempLog[MAX_PATH];
	_stprintf_s( szTempLog, _T("%s\\%s"), SERVICE_PATH, CLIENT_LOG_FOLDER );

	SimpleSocket::g_LogWrite.SetLogFileName(szTempLog);

	SimpleSocket::g_LogWrite.LogSave( _T("OnStart : Starting...") );

	LoadIni();	

	SimpleSocket::g_LogWrite.LogSave( _T("OnStart : END OK") );
}

void CEventMain::OnIdle()
{
	// cpu 100% 방지
	Sleep(1000);

	// 30일이상 지난 로그파일을 제거
	if( SimpleSocket::g_LogWrite.DeleteOldFile( SimpleSocket::g_LogWrite.GetLogPath(), 30 ) )
	{
		int deleteCheckOk = 0;
	}

	Idle_ProcessWakeUp();
}

void CEventMain::Idle_ProcessWakeUp()
{
	if( !m_tWakeUpCheck.Get2(3000) ) return;

	//SimpleSocket::LogSave("idle_process");

	for( UINT i=0; i<m_ControlExeList.size(); i++ )
	{
		ControlProgramExeInfo* pProgramExe = &m_ControlExeList[i];

		DWORD dwCurrentProcessID = 0;

		// 만약 프로세스가 실행중인경우
		bool bRun = ProcessUtil::IsRunProcess( pProgramExe->szExe, pProgramExe->szPath, dwCurrentProcessID );
		if( bRun )
		{
			pProgramExe->bCurrentProcess = true;


			// 만약 프로세스를 실행한지 얼마 안됬다면 충분한 시간 뒤에 무응답 체크를 하기 위해 리턴한다 (파일기록될 충분한 시간)
			//if (!m_tProcessCheck.Get3(40000)) return;


			//// 프로세스는 실행중인데 응답이 없는가를 확인한다(파일기록을 통해...)
			//SYSTEMTIME curTime;
			//GetLocalTime(&curTime);

			//// 현시각 파일 체크
			//CFileFind finder;
			//CString fPath;
			//fPath.Format(_T("C:\\Temp\\Sv%d.%.2d.%.2d.%.2d.%.2d.%.2d.txt"),
			//	curTime.wYear, curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);
			//if (finder.FindFile(fPath) == FALSE)
			//{
			//	m_Counter++;
			//}
			//else
			//{
			//	m_Counter = 0;
			//}

			//// 연속 ?회 파일이 존재안하는 경우만 강제종료
			//if (m_Counter == 5)
			//{
			//	bool bKillProcess = true;
			//	DWORD dwCurrentProcessID = 0;
			//	if (ProcessUtil::IsRunProcess(pProgramExe->szExe, pProgramExe->szPath, dwCurrentProcessID, bKillProcess))
			//	{
			//		SimpleSocket::g_LogWrite.LogSave(_T("Killed process [%s], [%s]"), pProgramExe->szExe, fPath);
			//		DeletePath(_T("C:\\Temp\\"), false);
			//	}
			//}
		}
		else
		{
			pProgramExe->bCurrentProcess = false;

			if( pProgramExe->bWakeOption )	// Wake옵션해당인경우만 깨우기(모니터링모드는 절대 깨우지 않는다)
			{
				bool bWake = false;

				if (pProgramExe->bWakeOne)
				{
					// 이때는 한번만 깨우고 그다음부터 깨우지 않는다
					pProgramExe->bWakeOption = false;
					bWake = true;
				}
				else
				{
					// 이때는 항상 깨운다
					bWake = true;
				}

				if (bWake)
				{
					DWORD dwProcessID;

					// 유저 계정얻어올 프로세스 이름
					TCHAR szProcessNameUser[32];
					if (pProgramExe->bSystemOption)
						_tcscpy_s(szProcessNameUser, _T("ServiceProgram.exe"));
					else
						_tcscpy_s(szProcessNameUser, _T("explorer.exe"));

					HANDLE hToken = ProcessUtil::GetProcessUserToken(szProcessNameUser);
					//if( ProcessUtil::CreateProcessUser( hToken, pProgramExe->szPath, pProgramExe->szPathDir, dwProcessID ) )
					if (ProcessUtil::CreateProcessUser(hToken, pProgramExe->szParam, pProgramExe->szPathDir, dwProcessID))
					{
						SimpleSocket::g_LogWrite.LogSave(_T("Created new process id[%d]"), dwProcessID);
						m_tProcessCheck.Reset();
					}
				}
			}

			m_Counter = 0;
		}
	}
}

