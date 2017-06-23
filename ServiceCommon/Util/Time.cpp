#include "../Def.h"//"StdAfx.h"
#include "Time.h"

#include <Mmsystem.h>
#pragma comment(lib,"winmm.lib")

namespace SimpleUtil
{
	CTIME::CTIME()
	{
		m_dwBacksettime = 0;
		m_dwNowsettime = 0;
		Reset();
	}

	bool CTIME::Get( DWORD num )
	{
		m_dwNowsettime = timeGetTime();
		if(m_dwNowsettime < m_dwBacksettime + num ) return false;
		m_dwBacksettime += num;
		return true;
	}

	bool CTIME::Get2( DWORD num )
	{
		m_dwNowsettime = timeGetTime();
		if(m_dwNowsettime < m_dwBacksettime + num ) return false;
		m_dwBacksettime = timeGetTime();
		return true;
	}

	bool CTIME::Get3(DWORD num)
	{
		m_dwNowsettime = timeGetTime();
		if (m_dwNowsettime < m_dwBacksettime + num) return false;
		return true;
	}

	void CTIME::Reset( void )
	{
		m_dwBacksettime = timeGetTime();
	}
}