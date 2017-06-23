#pragma once
#include <string>

namespace SimpleUtil
{
	typedef std::basic_string<TCHAR> tstring;

	// 스트링 바꿔치기
	tstring replace_all( const tstring& source, const tstring& pattern, const tstring& replace );

	// 스트링 비교
	int compare( const tstring& source1, const tstring& source2 );
}
