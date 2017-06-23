#pragma once
#include <tlhelp32.h> 
#include "Psapi.h" 

/**
Process UTIL

- 서비스 모드에서 특정 process가 실행중인지 검사하여 현재 사용자 계정으로 실행시킬때 등으로 활용 가능

*/
namespace ProcessUtil
{
	/**
	process실행중인지 알아내기
	*/
	bool IsRunProcess( const TCHAR* szExeName, DWORD& dwID );

	/**
	프로세스 종료
	*/
	bool KillProcess(HANDLE hProcess);

	/**
	prcess실행 알아내기(FullPath)
	- bKillProcess 옵션 : 강제로 죽이기
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

					// GetModuleFileNameEx는 64비트 프로세스를 읽어오지 못함, 따라서 QueryFullProcessImageName를 사용하도록 수정함
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
	현재 사용자 계정의 process token구하기 : 기본적으로 사용자 계정으로 항상 실행중인 explorer.exe를 이용
	@param process 이름강제지정 (디폴트 = explorer.exe)
	*/
	HANDLE GetProcessUserToken(TCHAR* pProcess = NULL);


	/**
	사용자 process token을 이용해 새로운 process 실행
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
				FALSE,//TRUE,	// 자식프로세스에 핸들상속옵션 (이것을 TRUE로 하면 자식이 살아있는 상태로 부모 먼저 종료후 다시시작할경우 오류발생함)
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
	윈도우 핸들로 프로세스 ID 구하기
	*/
	DWORD GetPIDFromHWND(HWND hwnd);
}



