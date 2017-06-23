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
	컨트롤 프로그램 실행 리스트
	*/
	arrControlExe m_ControlExeList;


	/**
	프로세스 깨우기 체크 타이머
	*/
	SimpleUtil::CTIME m_tWakeUpCheck;


	// 이건 무응답 체크를 위한 타이머
	SimpleUtil::CTIME m_tProcessCheck;


	// 연속 몇회 무응답인가 체크 카운터
	int m_Counter = 0;

	
	/**
	ini 컨피그 파일 로딩
	*/
	void LoadIni(bool bReload=false);


	/**
	메모리 정리
	*/
	void Destroy();


	/**
	프로그램의 프로세스가 상태를 체크하여 깨우거나 제거한다
	*/
	void Idle_ProcessWakeUp();

	/**
	삭제처리
	*/
	int DeletePath(CString strPath, bool bAllDelete) const;
};
