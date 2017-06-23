#include "../Def.h"//"StdAfx.h"
#include "String.h"

namespace SimpleUtil
{
	tstring replace_all( const tstring& source, const tstring& pattern, const tstring& replace )
	{
		tstring result = source;
		tstring::size_type pos = 0;
		tstring::size_type offset = 0;
		tstring::size_type pattern_len = pattern.size();
		tstring::size_type replace_len = replace.size();

		while ( ( pos = result.find( pattern, offset ) ) != tstring::npos )
		{
			result.replace( result.begin() + pos, result.begin() + pos + pattern_len, replace );
			offset = pos + replace_len;
		}
		return result;
	}

	int compare( const tstring& source1, const tstring& source2 )
	{
		return source1.compare( source2 );
	}
}
