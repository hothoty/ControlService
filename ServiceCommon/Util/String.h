#pragma once
#include <string>

namespace SimpleUtil
{
	typedef std::basic_string<TCHAR> tstring;

	// ��Ʈ�� �ٲ�ġ��
	tstring replace_all( const tstring& source, const tstring& pattern, const tstring& replace );

	// ��Ʈ�� ��
	int compare( const tstring& source1, const tstring& source2 );
}
