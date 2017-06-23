#pragma once
#include <tlhelp32.h> 
#include "Psapi.h" 

/**
Process UTIL

- ���� ��忡�� Ư�� process�� ���������� �˻��Ͽ� ���� ����� �������� �����ų�� ������ Ȱ�� ����

*/
namespace ProcessUtil
{
	/**
	process���������� �˾Ƴ���
	*/
	bool IsRunProcess( const TCHAR* szExeName, DWORD& dwID );

	/**
	���μ��� ����
	*/
	bool KillProcess(HANDLE hProcess);

	/**
	prcess���� �˾Ƴ���(FullPath)
	- bKillProcess �ɼ� : ������ ���̱�
	*/
	inline bool IsRunProcess( const TCHAR* szExeName, const TCHAR* szFullPath, DWORD& dwID, bool bKillProcess=false )
	{
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0); 
		PROCESSENTRY32 pe32; 
		pe32.dwSize = sizeof(PROCESSENTRY32); 
		Process32First(hSnapshot,&pe32); 
		do 
		{ 
			if( _tcsicmp(szExeName, pe32.szExeFile) == 0 ) 
			{
				HANDLE hProcess;

				if( bKillProcess )
					hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, pe32.th32ProcessID);
				else
					hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, 0, pe32.th32ProcessID); 

				if( hProcess )
				{
					TCHAR szProcessPath[MAX_PATH];

					// GetModuleFileNameEx�� 64��Ʈ ���μ����� �о���� ����, ���� QueryFullProcessImageName�� ����ϵ��� ������
					//DWORD dwReturn = GetModuleFileNameEx( hProcess, NULL, szProcessPath, MAX_PATH );

					DWORD _size = MAX_PATH;
					PDWORD size_exe = &_size;
					//BOOL bRet = QueryFullProcessImageName(hProcess, PROCESS_QUERY_LIMITED_INFORMATION, szProcessPath, size_exe);
					BOOL bRet = QueryFullProcessImageName(hProcess, 0, szProcessPath, size_exe);

					DWORD dwLastErr = GetLastError();

					if( _tcsicmp(szFullPath, szProcessPath) == 0 ) 
					{
						if( bKillProcess )
						{
							bool bExit = KillProcess( hProcess );
							int gggg=0;
						}

						CloseHandle(hProcess);
						CloseHandle(hSnapshot);
						dwID = pe32.th32ProcessID;

						return true;
					}
					CloseHandle(hProcess);
				}
			} 
		} 
		while(Process32Next(hSnapshot,&pe32)); 
		CloseHandle(hSnapshot); 
		return false;
	}


	/**
	���� ����� ������ process token���ϱ� : �⺻������ ����� �������� �׻� �������� explorer.exe�� �̿�
	@param process �̸��������� (����Ʈ = explorer.exe)
	*/
	HANDLE GetProcessUserToken(TCHAR* pProcess = NULL);


	/**
	����� process token�� �̿��� ���ο� process ����
	*/
	inline bool CreateProcessUser(const HANDLE hToken, TCHAR* szFullPathExeName, TCHAR* szPathDir, DWORD& dwProcessID)
	{
		if( hToken == NULL ) return false;

		bool bReturn = false;
		STARTUPINFO si;
		PROCESS_INFORMATION pi;
		::ZeroMemory(&si, sizeof(STARTUPINFO));
		::ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
		si.cb = sizeof(STARTUPINFO);
		si.lpDesktop = _T("winsta0\\default");
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_SHOW;
		if(
			CreateProcessAsUser(
				hToken,
				NULL,
				szFullPathExeName,
				NULL,
				NULL,
				FALSE,//TRUE,	// �ڽ����μ����� �ڵ��ӿɼ� (�̰��� TRUE�� �ϸ� �ڽ��� ����ִ� ���·� �θ� ���� ������ �ٽý����Ұ�� �����߻���)
				CREATE_NEW_CONSOLE | CREATE_NEW_PROCESS_GROUP,
				NULL,
				szPathDir,
			#ifdef UNICODE
				reinterpret_cast<LPSTARTUPINFOW>(&si),
			#else
				(LPSTARTUPINFO)&si,
			#endif
				&pi)
			)
		{
			bReturn = true;
			dwProcessID = pi.dwProcessId;
		}

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		return bReturn;
	}


	/**
	������ �ڵ�� ���μ��� ID ���ϱ�
	*/
	DWORD GetPIDFromHWND(HWND hwnd);
}



