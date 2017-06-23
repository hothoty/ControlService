#pragma once

namespace SimpleSocket
{
	class CLogWrite
	{
	public:
		CLogWrite();
		~CLogWrite();

		/**
		로그파일 경로(FULLPATH) 설정
		*/
		void SetLogFileName(TCHAR* name);


		/**
		설정된 로그파일 경로 구하기
		*/
		const TCHAR* GetLogPath();

		/**
		로그기록
		*/
		BOOL LogSave(TCHAR *msg, ...);


		/**
		오래된 파일 자동삭제
		- 해당폴더내용이 삭제처리된다(nDay만큼 지난 파일만)

		@param return 삭제처리체크실행(true), 삭제처리체크실행안함(false)
		*/
		bool DeleteOldFile(const TCHAR* fullpath, const int nDay, const DWORD dwMs = (1000 * 60 * 60)/*자주호출해도 1시간에 1회만 처리*/);


	private:
		TCHAR szLogPath[256];

		DWORD dwFileDeleteTime;

		BOOL SaveLogFile(TCHAR *name,TCHAR log[MAX_PATH]);
		BOOL FileSave(TCHAR *filepath,TCHAR *msg, ...);
		void DeleteAllFilesOldDay(const TCHAR* folderPath, const int OldDay);
	};

	extern CLogWrite g_LogWrite;
}
