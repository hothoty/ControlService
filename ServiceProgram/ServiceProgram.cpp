// ServiceProgram.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "RegService.h"
#include "EventMain.h"


int _tmain(int argc, _TCHAR* argv[])
{
	CEventMain eventMain;
	CService::Start(SERVICE_NAME, &eventMain);
	return 0;
}

