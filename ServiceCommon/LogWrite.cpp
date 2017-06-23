#include "StdAfx.h"
#include <iostream>
#include "LogWrite.h"
#include <tchar.h>

#include <atltime.h>

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

namespace SimpleSocket
{
	CLogWrite g_LogWrite;

	CLogWrite::CLogWrite() : dwFileDeleteTime(0)
	{
		_stprintf_s( szLogPath, _T("c:\\LogTemp") );
	}

	CLogWrite::~CLogWrite()
	{
	}

	const TCHAR* CLogWrite::GetLogPath()
	{
		return szLogPath;
	}

	void CLogWrite::SetLogFileName(TCHAR* name)
	{
		_stprintf_s( szLogPath, name );
	}


	// 오래된 파일 자동삭제
	bool CLogWrite::DeleteOldFile(const TCHAR* fullpath, const int nDay, const DWORD dwMs)
	{
		DWORD dwCurrent = timeGetTime();
		if( dwCurrent > dwFileDeleteTime + dwMs )
		{
			dwFileDeleteTime = dwCurrent;
			DeleteAllFilesOldDay(fullpath, nDay);
			return true;
		}
		return false;
	}


	/**
	특정 폴더내의 파일 삭제

	- 하위폴더들 내에 있는 파일들은 삭제되지 않음

	@param folderPath : 폴더경로
	@param OldDay : 몇일 지난 파일을 삭제할것인지 (0을넣으면 전부 삭제)
	
	@code
		DeleteAllFilesOldDay( "C:\\abc", 2 );	// "C:\\abc" 폴더에 있는 2일 이상 지난 모든 파일들을 삭제
	@end
	*/
	void CLogWrite::DeleteAllFilesOldDay(const TCHAR* folderPath, const int OldDay)
	{
		TCHAR fileFound[256];
		WIN32_FIND_DATA info;
		HANDLE hp = NULL;

		_stprintf_s(fileFound, _T("%s\\*.*"), folderPath);
		hp = FindFirstFile(fileFound, &info); //디렉토리에 파일이 있는지 첫번째 파일만.

		SYSTEMTIME tFileTime;

		CTime CurrTime = CTime::GetCurrentTime();

		do
		{
			_stprintf_s(fileFound, _T("%s\\%s"), folderPath, info.cFileName);

			FileTimeToSystemTime( &info.ftLastWriteTime, &tFileTime );


			CTime File_Time(tFileTime);
			CTimeSpan SpanTime = CurrTime - File_Time;

			LONGLONG SpanDay = SpanTime.GetDays();

			// ?일 이상 지난 파일들을 삭제
			if( SpanDay >= OldDay )
			{
				DeleteFile(fileFound);
			}
		}while(FindNextFile(hp, &info));  //다른 파일이 있을때 까지 

		FindClose(hp);
	}

	BOOL CLogWrite::SaveLogFile(TCHAR *name,TCHAR log[MAX_PATH])
	{
		HANDLE		hFile;
		DWORD		nBytesRead,sizzz;
		int			len;

		hFile=CreateFile(name,GENERIC_WRITE,FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
		if(INVALID_HANDLE_VALUE==hFile)
		{
			CloseHandle(hFile);
			return FALSE;
		}
		sizzz=GetFileSize(hFile,NULL);
		SetFilePointer(hFile,sizzz,NULL,FILE_BEGIN);

#ifdef _UNICODE
		char szTemp[MAX_PATH];
		Wide2CharSS( log, szTemp, 256 );
		len=strlen(szTemp);
		if(WriteFile(hFile,szTemp, len , &nBytesRead, NULL)==FALSE)
#else
		len=strlen(log);
		if(WriteFile(hFile,log, len , &nBytesRead, NULL)==FALSE)
#endif
		{
			CloseHandle(hFile);
			return FALSE;
		}

		CloseHandle(hFile);
		return TRUE;
	}

	BOOL CLogWrite::FileSave(TCHAR *filepath,TCHAR *msg, ... )
	{
		TCHAR buf[256];
		va_list va; 
		va_start(va, msg); 
		_vstprintf_s(buf, msg, (va_list)va); 
		va_end(va);

		TCHAR tempbuf[256];
		SYSTEMTIME stime;
		GetSystemTime(&stime);

		_stprintf_s(tempbuf, _T("[%2.d:%2.d:%2.d.%3.d]  %s%c%c"),
			(stime.wHour+8)%24+1,stime.wMinute,stime.wSecond,stime.wMilliseconds,
			buf,
			0x0d,0x0a
		);

		TCHAR temppath[256];
		_stprintf_s(temppath, _T("%s\\%4.d_%2.d_%2.d.txt"),
			filepath,
			stime.wYear, stime.wMonth, stime.wDay
		);
		CreateDirectory( filepath, NULL );

		SaveLogFile(temppath, tempbuf);
		return TRUE;
	}

	BOOL CLogWrite::LogSave(TCHAR *msg, ...)
	{
		TCHAR buf[256];
		va_list va; 
		va_start(va, msg); 
		_vstprintf_s(buf, msg, (va_list)va); 
		va_end(va);

		return FileSave(szLogPath, buf);
	}
}