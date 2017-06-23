#include "../Def.h"//"StdAfx.h"
#include "Thread.h"
#include <process.h>

namespace SimpleUtil
{
	CThread::CThread()
	{
		m_hThread = NULL;
		m_bIsRun = false;
		m_dwWaitTickMS = 0;
		m_hQuitEvent = CreateEvent( NULL , TRUE , FALSE , NULL );
	}

	CThread::~CThread()
	{
		Stop();
		DestroyThread();

		CloseHandle( m_hQuitEvent );
		if( m_hThread )
			CloseHandle( m_hThread );
	}

	unsigned int WINAPI CallTickThread(LPVOID p)
	{
		CThread* pTickThread = (CThread*) p;
		pTickThread->TickThread();

		return 1;
	}

	bool CThread::CreateThread( DWORD dwTimeMS )
	{
		unsigned int uiThreadId = 0;
		m_hThread = (HANDLE)_beginthreadex(NULL, 0, &CallTickThread, this, CREATE_SUSPENDED , &uiThreadId);
		if(m_hThread == NULL)	{
			return false;
		}
		m_dwWaitTickMS = dwTimeMS;
		return true;

	}
	void CThread::Run()
	{
		if( false == m_bIsRun )	{
			m_bIsRun = true;
			ResumeThread( m_hThread );
		}
	}

	void CThread::Stop()
	{
		if( true == m_bIsRun )	{
			m_bIsRun = false;
			SuspendThread( m_hThread );
		}
	}

	void CThread::TickThread()
	{
		while( true )	{
			DWORD dwRet = WaitForSingleObject( m_hQuitEvent, m_dwWaitTickMS );
			if( WAIT_OBJECT_0 == dwRet )
					break;
			else if( WAIT_TIMEOUT == dwRet )		{
				OnProcess();
					
			}
		}
	}

	void CThread::DestroyThread()
	{
		Run();
		SetEvent( m_hQuitEvent );
		WaitForSingleObject( m_hThread , INFINITE );
	}
}