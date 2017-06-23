#pragma once

namespace SimpleUtil
{
	class CThread
	{
	public:
		CThread();
		virtual ~CThread();
	
		bool	CreateThread( DWORD dwTimeMS );
		void	DestroyThread();

		void	Run();
		void	Stop();

		bool	IsRun() { return m_bIsRun; }

		/**
		외부사용 금지 (내부적으로 호출됨)
		*/
		void	TickThread();

	private:
		HANDLE	m_hThread;
		HANDLE	m_hQuitEvent;
		bool	m_bIsRun;
		DWORD	m_dwWaitTickMS;

		virtual void	OnProcess() = 0;
	};

	class CLock
	{
	public :
		  class CriticalSection
		  {
			 public:
				 explicit CriticalSection(CLock &crit) : m_csSyncObject(crit) { m_csSyncObject.Enter(); }
				 ~CriticalSection() { m_csSyncObject.Leave(); }
			 private :
				CLock &m_csSyncObject;
				CriticalSection(const CriticalSection &rhs);	// No copies do not implement
				CriticalSection &operator=(const CriticalSection &rhs);
		  };
		  CLock() { InitializeCriticalSection(&m_csSyncObject); }
		  ~CLock() { DeleteCriticalSection( &m_csSyncObject ); }

		  BOOL TryEnter()	{ return TryEnterCriticalSection(&m_csSyncObject); }
		  void Enter()		{ ::EnterCriticalSection(&m_csSyncObject); }
		  void Leave()		{ ::LeaveCriticalSection(&m_csSyncObject); }

	   private :
		  CRITICAL_SECTION m_csSyncObject;

		  CLock(const CLock &rhs);
		  CLock &operator=(const CLock &rhs);
	};
}