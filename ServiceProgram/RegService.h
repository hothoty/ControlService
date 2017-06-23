#pragma once


/**
���� ���� �������̽� Ŭ����
*/
class IService
{
public:
	virtual ~IService() {}

	/**
	���� ���� ���� ����
	*/
	virtual void OnIdle() = 0;


	/**
	���� ���� �ʿ��� ������ �ε�(�Ǵ� �ʱ�ȭ)
	*/
	virtual void OnStart() = 0;


	/**
	�Ͻ�����
	*/
	virtual void OnPause() {}


	/**
	���� �Ͻ������� �簳
	*/
	virtual void OnResume() {}


	/**
	��������
	*/
	virtual void OnClose() {}
};

class CService
{
private:
	static SERVICE_STATUS_HANDLE m_hSrv;
	static DWORD m_NowState;						// ���� ���¸� �˰� ���ִ� ����
	static bool m_bPause;							// ���� ���� �������� Ȯ�� �ϴ� ����
	static bool m_bClosing;
	static HANDLE m_Exit_Event;
	static IService* m_Interface;
	static TCHAR m_szServiceName[64];

	static void MyServiceMain( DWORD argc, LPTSTR *argv );
	static void MySetStatus( DWORD dwState, DWORD dwAccept = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE );
	static void MyServiceHandler( DWORD fdwControl );

public:

	/**
	���� ����
	*/
	static void Start(LPTSTR servicename, IService* pInterface);
};


class CRegService
{
public:
	enum ErrService
	{
		ERR_OK = 0,
		ERR_AUTH,		// ���Ѿ���
		ERR_PATH,		// ��ο���
		ERR_FAIL,		// ��ġ/���Ž���
		ERR_NOTINST,	// ���񽺹̼�ġ
	};

	/**
	���� ��ġ
	@param name : ���� ���� �̸�
	@param exefile : ���� ���� ��� "D:\\TestControlServer.exe" �̷�������
	@param desc : ���� �ΰ� ����
	*/
	static ErrService InstallService(LPTSTR name, LPTSTR exefile, LPTSTR desc);

	/**
	���� ����
	*/
	static ErrService UnInstallService(LPTSTR name);
};