#include "../Def.h"//"StdAfx.h"
#include <tlhelp32.h> 
#include "Process.h"
#include "Psapi.h" 
#include <tchar.h>
#pragma comment(lib,"psapi.lib")

namespace ProcessUtil
{
	bool IsRunProcess( const TCHAR* szExeName, DWORD& dwID ) 
	{
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0); 
		PROCESSENTRY32 pe32; 
		pe32.dwSize = sizeof(PROCESSENTRY32); 
		Process32First(hSnapshot,&pe32); 
		do 
		{ 
			if( _tcsicmp(szExeName, pe32.szExeFile) == 0 ) 
			{
				HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, 0, pe32.th32ProcessID); 
				if( hProcess )
				{
					CloseHandle(hProcess);
					CloseHandle(hSnapshot);
					dwID = pe32.th32ProcessID;
					return true;
				}
			} 
		} 
		while(Process32Next(hSnapshot,&pe32)); 
		CloseHandle(hSnapshot); 
		return false; 
	}

	bool KillProcess(HANDLE hProcess)
	{
		DWORD uExitCode = 0;
		return (TerminateProcess(hProcess, uExitCode) ? true : false);
	}

	HANDLE GetProcessUserToken(TCHAR* pProcess)
	{ 
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0); 
		PROCESSENTRY32 pe32; 
		pe32.dwSize = sizeof(PROCESSENTRY32); 
		Process32First(hSnapshot,&pe32); 
		do
		{ 
			TCHAR* sProcessExe = pe32.szExeFile;

			bool bTRUE = false;
			if( pProcess )
			{
				if( _tcsicmp(pProcess, sProcessExe) == 0 )
					bTRUE = true;
			}
			else
			{
				if( _tcsicmp(_T("explorer.exe"), sProcessExe) == 0 )
					bTRUE = true;
			}

			if( bTRUE ) 
			{
				HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, 0, pe32.th32ProcessID); 
				if( hProcess )
				{
					HANDLE hToken = NULL;
					BOOL bResult = ::OpenProcessToken(hProcess, TOKEN_ALL_ACCESS, &hToken);
					CloseHandle(hSnapshot); 
					return hToken;
				}
			} 
		} 
		while(Process32Next(hSnapshot,&pe32)); 
		CloseHandle(hSnapshot); 
		return NULL; 
	}

	DWORD GetPIDFromHWND(HWND hwnd)
	{
		DWORD processID;
		GetWindowThreadProcessId(hwnd, &processID);
		return processID;
	}

}
