#pragma once
#include <vector>
#include <objbase.h>

/**
기본 서비스 파일 경로
*/
#define SERVICE_PATH		_T("c:\\Control\\Service")


/**
서비스 프로그램 INI파일
*/
#define CLIENT_CONFIG_INI_NAME		_T("ServiceInfo.ini")


/**
서비스 프로그램 이름
*/
#define SERVICE_NAME	_T("ControlServiceProgram")


/**
버전관리 : int
*/
#define DEF_Version			1003


#define DEF_PROGRAM_PATH 128
#define DEF_PROGRAM_EXE 64
#define DEF_PROGRAM_ID 128

// 컨트롤 프로그램 실행 관련 정보
struct ControlProgramExeInfo
{
	/**
	파일에서 읽어오는 정보들
	*/
	TCHAR szPathDir[DEF_PROGRAM_PATH];		// 순수 경로
	TCHAR szPath[DEF_PROGRAM_PATH];			// FullPath (경로+실행파일)
	TCHAR szExe[DEF_PROGRAM_EXE];			// 실행파일
	TCHAR szParam[DEF_PROGRAM_EXE];			// 파라미터
	TCHAR szID[DEF_PROGRAM_ID];				// 프로그램 구분 ID(이름)
	bool bWakeOption;						// WakeOn 적용 여부
	bool bWakeOne;							// WakeOn 적용일때 서비스 실행 이후 최초 한번만 깨우기 옵션
	bool bSystemOption;						// System권한(UAC) 실행옵션(WakeOn모드인경우 해당함)


	/**
	속성값
	*/
	DWORD dwProcess;						// 프로세스 고유 ID
	bool bCurrentProcess;					// 프로세스가 현재 실행중인지


	ControlProgramExeInfo() { DeActiveProcess(); bCurrentProcess = false; bWakeOption = false; bWakeOne = false; bSystemOption = false; }

	bool IsProcess() const			{ return dwProcess != 0 ? true : false; }	// 현재 실행중인 프로세스인가?
	void ActiveProcess(DWORD dwID)	{ dwProcess = dwID; }						// 프로세스 입력
	void DeActiveProcess()			{ dwProcess = 0; }							// 비활성화 상태
};
typedef std::vector<ControlProgramExeInfo> arrControlExe;


// 컨트롤 프로그램 데이터 정보
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
		_stprintf_s( szTemp , TEXT("[%s]Config File에 [%s]-[%s]항목은 존재하지 않습니다."), szFileName , szAppName ,szKey );
		return -1;
	}
	return 0;
}

inline int	GetINIInt( const TCHAR* szAppName , const TCHAR* szKey , const TCHAR* szFileName, int *pNum )
{
	int ret = GetPrivateProfileInt( szAppName , szKey  , -1 ,  szFileName );
	if( ret < 0 ) {
		TCHAR szTemp[ 300 ];
		_stprintf_s( szTemp , TEXT("[%s]Config File에 [%s]-[%s]항목은 존재하지 않습니다."),
			szFileName , szAppName ,szKey );
	}
	*pNum = ret;
	return ret;
}



