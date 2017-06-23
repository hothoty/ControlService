#pragma once

namespace SimpleSocket
{
	class CLogWrite
	{
	public:
		CLogWrite();
		~CLogWrite();

		/**
		�α����� ���(FULLPATH) ����
		*/
		void SetLogFileName(TCHAR* name);


		/**
		������ �α����� ��� ���ϱ�
		*/
		const TCHAR* GetLogPath();

		/**
		�αױ��
		*/
		BOOL LogSave(TCHAR *msg, ...);


		/**
		������ ���� �ڵ�����
		- �ش����������� ����ó���ȴ�(nDay��ŭ ���� ���ϸ�)

		@param return ����ó��üũ����(true), ����ó��üũ�������(false)
		*/
		bool DeleteOldFile(const TCHAR* fullpath, const int nDay, const DWORD dwMs = (1000 * 60 * 60)/*����ȣ���ص� 1�ð��� 1ȸ�� ó��*/);


	private:
		TCHAR szLogPath[256];

		DWORD dwFileDeleteTime;

		BOOL SaveLogFile(TCHAR *name,TCHAR log[MAX_PATH]);
		BOOL FileSave(TCHAR *filepath,TCHAR *msg, ...);
		void DeleteAllFilesOldDay(const TCHAR* folderPath, const int OldDay);
	};

	extern CLogWrite g_LogWrite;
}
