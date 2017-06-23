#pragma once


/**
서비스 관련 인터페이스 클래스
*/
class IService
{
public:
	virtual ~IService() {}

	/**
	서비스 실행 메인 루프
	*/
	virtual void OnIdle() = 0;


	/**
	시작 직전 필요한 데이터 로딩(또는 초기화)
	*/
	virtual void OnStart() = 0;


	/**
	일시정지
	*/
	virtual void OnPause() {}


	/**
	서비스 일시정지후 재개
	*/
	virtual void OnResume() {}


	/**
	정지시작
	*/
	virtual void OnClose() {}
};

class CService
{
private:
	static SERVICE_STATUS_HANDLE m_hSrv;
	static DWORD m_NowState;						// 현재 상태를 알게 해주는 변수
	static bool m_bPause;							// 서비스 시작 중지인지 확인 하는 변수
	static bool m_bClosing;
	static HANDLE m_Exit_Event;
	static IService* m_Interface;
	static TCHAR m_szServiceName[64];

	static void MyServiceMain( DWORD argc, LPTSTR *argv );
	static void MySetStatus( DWORD dwState, DWORD dwAccept = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE );
	static void MyServiceHandler( DWORD fdwControl );

public:

	/**
	서비스 시작
	*/
	static void Start(LPTSTR servicename, IService* pInterface);
};


class CRegService
{
public:
	enum ErrService
	{
		ERR_OK = 0,
		ERR_AUTH,		// 권한없음
		ERR_PATH,		// 경로오류
		ERR_FAIL,		// 설치/제거실패
		ERR_NOTINST,	// 서비스미설치
	};

	/**
	서비스 설치
	@param name : 서비스 고유 이름
	@param exefile : 서비스 파일 경로 "D:\\TestControlServer.exe" 이런식으로
	@param desc : 서비스 부가 설명
	*/
	static ErrService InstallService(LPTSTR name, LPTSTR exefile, LPTSTR desc);

	/**
	서비스 해지
	*/
	static ErrService UnInstallService(LPTSTR name);
};