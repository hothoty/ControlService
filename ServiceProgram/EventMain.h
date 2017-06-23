#pragma once
#include "RegService.h"
#include "../ServiceCommon/Util/Time.h"

class CEventMain : public IService
{
public:
	CEventMain();
	~CEventMain();

	virtual void OnIdle() override;
	virtual void OnStart() override;
	virtual void OnPause() override;
	virtual void OnResume() override;
	virtual void OnClose() override;

private:

	/**
	��Ʈ�� ���α׷� ���� ����Ʈ
	*/
	arrControlExe m_ControlExeList;


	/**
	���μ��� ����� üũ Ÿ�̸�
	*/
	SimpleUtil::CTIME m_tWakeUpCheck;


	// �̰� ������ üũ�� ���� Ÿ�̸�
	SimpleUtil::CTIME m_tProcessCheck;


	// ���� ��ȸ �������ΰ� üũ ī����
	int m_Counter = 0;

	
	/**
	ini ���Ǳ� ���� �ε�
	*/
	void LoadIni(bool bReload=false);


	/**
	�޸� ����
	*/
	void Destroy();


	/**
	���α׷��� ���μ����� ���¸� üũ�Ͽ� ����ų� �����Ѵ�
	*/
	void Idle_ProcessWakeUp();

	/**
	����ó��
	*/
	int DeletePath(CString strPath, bool bAllDelete) const;
};
