// ServiceProgram.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
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

