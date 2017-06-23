#pragma once

namespace SimpleUtil
{
	class CTIME
	{
	private:
		DWORD m_dwBacksettime;
		DWORD m_dwNowsettime;
	public:
		CTIME();
	
		bool Get( DWORD num );	// 시간계산	 ( 지난 시간까지 계산한다, 게임진행시간같은 정확도록 요할때 이용 )
		bool Get2( DWORD num );	// 시간계산2 ( 이것은 지난시간을 무시하고 무조건 일정시간을 계산 )
		bool Get3(DWORD num);
		void Reset( void );
	};
}