// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 일부 CString 생성자는 명시적으로 선언됩니다.
#define _AFX_NO_MFC_CONTROLS_IN_DIALOGS         // 대화 상자에서 MFC 컨트롤에 대한 지원을 제거합니다.

// MFC의 공통 부분과 무시 가능한 경고 메시지에 대한 숨기기를 해제합니다.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 핵심 및 표준 구성 요소입니다.



// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.

// library
#include "../ServiceCommon/LogWrite.h"


// library common
#include "../ServiceCommon/ServiceCommon.h"
#ifdef _DEBUG
	#pragma comment(lib,"../debug/ServiceCommon.lib")
#else
	#pragma comment(lib,"../release/ServiceCommon.lib")
#endif