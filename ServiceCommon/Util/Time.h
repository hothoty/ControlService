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
	
		bool Get( DWORD num );	// �ð����	 ( ���� �ð����� ����Ѵ�, ��������ð����� ��Ȯ���� ���Ҷ� �̿� )
		bool Get2( DWORD num );	// �ð����2 ( �̰��� �����ð��� �����ϰ� ������ �����ð��� ��� )
		bool Get3(DWORD num);
		void Reset( void );
	};
}