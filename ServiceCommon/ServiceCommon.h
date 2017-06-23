#pragma once
#include <vector>
#include <objbase.h>

/**
�⺻ ���� ���� ���
*/
#define SERVICE_PATH		_T("c:\\Control\\Service")


/**
���� ���α׷� INI����
*/
#define CLIENT_CONFIG_INI_NAME		_T("ServiceInfo.ini")


/**
���� ���α׷� �̸�
*/
#define SERVICE_NAME	_T("ControlServiceProgram")


/**
�������� : int
*/
#define DEF_Version			1003


#define DEF_PROGRAM_PATH 128
#define DEF_PROGRAM_EXE 64
#define DEF_PROGRAM_ID 128

// ��Ʈ�� ���α׷� ���� ���� ����
struct ControlProgramExeInfo
{
	/**
	���Ͽ��� �о���� ������
	*/
	TCHAR szPathDir[DEF_PROGRAM_PATH];		// ���� ���
	TCHAR szPath[DEF_PROGRAM_PATH];			// FullPath (���+��������)
	TCHAR szExe[DEF_PROGRAM_EXE];			// ��������
	TCHAR szParam[DEF_PROGRAM_EXE];			// �Ķ����
	TCHAR szID[DEF_PROGRAM_ID];				// ���α׷� ���� ID(�̸�)
	bool bWakeOption;						// WakeOn ���� ����
	bool bWakeOne;							// WakeOn �����϶� ���� ���� ���� ���� �ѹ��� ����� �ɼ�
	bool bSystemOption;						// System����(UAC) ����ɼ�(WakeOn����ΰ�� �ش���)


	/**
	�Ӽ���
	*/
	DWORD dwProcess;						// ���μ��� ���� ID
	bool bCurrentProcess;					// ���μ����� ���� ����������


	ControlProgramExeInfo() { DeActiveProcess(); bCurrentProcess = false; bWakeOption = false; bWakeOne = false; bSystemOption = false; }

	bool IsProcess() const			{ return dwProcess != 0 ? true : false; }	// ���� �������� ���μ����ΰ�?
	void ActiveProcess(DWORD dwID)	{ dwProcess = dwID; }						// ���μ��� �Է�
	void DeActiveProcess()			{ dwProcess = 0; }							// ��Ȱ��ȭ ����
};
typedef std::vector<ControlProgramExeInfo> arrControlExe;


// ��Ʈ�� ���α׷� ������ ����
struct ControlProgramData
{
	GUID myGUID;	// guid
	DWORD dwID;		// process ID
	TCHAR szPath[DEF_PROGRAM_PATH];

	ControlProgramData() { myGUID = GUID_NULL; }
	ControlProgramData(GUID id, DWORD processid)
	{
		myGUID = id;
		dwID = processid;
	}
};


inline int GetINIStr( const TCHAR* szAppName, const TCHAR* szKey, const TCHAR* szFileName, TCHAR* szOutStr, int nSize )
{
	int ret = GetPrivateProfileString( szAppName  , szKey  , TEXT("") , szOutStr , nSize , szFileName);
	if( 0 == ret ) {
		TCHAR szTemp[ 300 ];
		_stprintf_s( szTemp , TEXT("[%s]Config File�� [%s]-[%s]�׸��� �������� �ʽ��ϴ�."), szFileName , szAppName ,szKey );
		return -1;
	}
	return 0;
}

inline int	GetINIInt( const TCHAR* szAppName , const TCHAR* szKey , const TCHAR* szFileName, int *pNum )
{
	int ret = GetPrivateProfileInt( szAppName , szKey  , -1 ,  szFileName );
	if( ret < 0 ) {
		TCHAR szTemp[ 300 ];
		_stprintf_s( szTemp , TEXT("[%s]Config File�� [%s]-[%s]�׸��� �������� �ʽ��ϴ�."),
			szFileName , szAppName ,szKey );
	}
	*pNum = ret;
	return ret;
}



