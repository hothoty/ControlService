// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // �Ϻ� CString �����ڴ� ��������� ����˴ϴ�.
#define _AFX_NO_MFC_CONTROLS_IN_DIALOGS         // ��ȭ ���ڿ��� MFC ��Ʈ�ѿ� ���� ������ �����մϴ�.

// MFC�� ���� �κа� ���� ������ ��� �޽����� ���� ����⸦ �����մϴ�.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC �ٽ� �� ǥ�� ���� ����Դϴ�.



// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.

// library
#include "../ServiceCommon/LogWrite.h"


// library common
#include "../ServiceCommon/ServiceCommon.h"
#ifdef _DEBUG
	#pragma comment(lib,"../debug/ServiceCommon.lib")
#else
	#pragma comment(lib,"../release/ServiceCommon.lib")
#endif