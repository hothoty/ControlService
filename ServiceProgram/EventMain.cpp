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
	// ��ξ��� ��� ����(������������) ����
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
	�����ؾ��ϴ� ���α׷� ����
	*/
	const TCHAR* defaultName = _T("CONTROLPROGRAM");
	int nCount = 1;
	TCHAR ControlProgramINI[256];
	m_ControlExeList.clear();
	while(1)
	{
		// �ڵ� �ѹ��� ó��
		_stprintf_s( ControlProgramINI, _T("%s%d"), defaultName, nCount++ );

		// PATH �о����
		TCHAR szPath[128];
		if( -1 == ( GetINIStr( ControlProgramINI, _T("PATH"), szTempIni, szPath, 128 ) ) )
		{
			SimpleSocket::g_LogWrite.LogSave( _T("OnStart : ControlProgram ini error [%s,%s]"), ControlProgramINI, _T("PATH"));
			break;
		}

		// EXE �о����
		TCHAR szEXE[64];
		if( -1 == ( GetINIStr( ControlProgramINI, _T("EXE"), szTempIni, szEXE, 64 ) ) )
		{
			SimpleSocket::g_LogWrite.LogSave( _T("OnStart : ControlProgram ini error [%s,%s]"), ControlProgramINI, _T("EXE"));
			break;
		}

		// �Ķ���� �о����
		TCHAR szParam[64];
		if (-1 == (GetINIStr(ControlProgramINI, _T("PARAM"), szTempIni, szParam, 64)))
		{
			SimpleSocket::g_LogWrite.LogSave(_T("OnStart : ControlProgram ini error [%s,%s]"), ControlProgramINI, _T("Param"));
			break;
		}

		// WAKEOn�ɼ� ���뿩��
		int nWakeOnMode = 0;
		if( -1 == ( GetINIInt( ControlProgramINI, _T("WAKEOP"), szTempIni, &nWakeOnMode ) ) )
		{
			SimpleSocket::g_LogWrite.LogSave( _T("OnStart : ControlProgram ini error wakeonop [%s]"), nWakeOnMode);
			// �̰� ���߿� �߰��ȰŴϱ� ��� break�������� �׳� 0���� �����ϰ� �Ѿ��
			nWakeOnMode = 0;
		}

		// UAC SYSTEM���� ���뿩��
		int nSystemUAC = 0;
		if( -1 == ( GetINIInt( ControlProgramINI, _T("SYSTEM"), szTempIni, &nSystemUAC ) ) )
		{
			SimpleSocket::g_LogWrite.LogSave( _T("OnStart : ControlProgram ini error system [%s]"), nSystemUAC);
			// �̰� ���߿� �߰��ȰŴϱ� ��� break�������� �׳� 0���� �����ϰ� �Ѿ��
			nSystemUAC = 0;
		}

		// �ĺ�ID �о����
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

		// ���ѷ��� ����
		if( nCount >= 10 ) break;
	}

	if( bReload )
	{
		SimpleSocket::g_LogWrite.LogSave( _T("Reloaded Ini File") );
	}
	else
	{
		// ���� ����
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
	// cpu 100% ����
	Sleep(1000);

	// 30���̻� ���� �α������� ����
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

		// ���� ���μ����� �������ΰ��
		bool bRun = ProcessUtil::IsRunProcess( pProgramExe->szExe, pProgramExe->szPath, dwCurrentProcessID );
		if( bRun )
		{
			pProgramExe->bCurrentProcess = true;


			// ���� ���μ����� �������� �� �ȉ�ٸ� ����� �ð� �ڿ� ������ üũ�� �ϱ� ���� �����Ѵ� (���ϱ�ϵ� ����� �ð�)
			//if (!m_tProcessCheck.Get3(40000)) return;


			//// ���μ����� �������ε� ������ ���°��� Ȯ���Ѵ�(���ϱ���� ����...)
			//SYSTEMTIME curTime;
			//GetLocalTime(&curTime);

			//// ���ð� ���� üũ
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

			//// ���� ?ȸ ������ ������ϴ� ��츸 ��������
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

			if( pProgramExe->bWakeOption )	// Wake�ɼ��ش��ΰ�츸 �����(����͸����� ���� ������ �ʴ´�)
			{
				bool bWake = false;

				if (pProgramExe->bWakeOne)
				{
					// �̶��� �ѹ��� ����� �״������� ������ �ʴ´�
					pProgramExe->bWakeOption = false;
					bWake = true;
				}
				else
				{
					// �̶��� �׻� �����
					bWake = true;
				}

				if (bWake)
				{
					DWORD dwProcessID;

					// ���� �������� ���μ��� �̸�
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

