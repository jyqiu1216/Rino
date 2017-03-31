
#include "common/WJSpinlock.h"


namespace WJ
{
	void CSpinlock::Lock()
	{
		int expected = 0;
		while (!m_busy.compare_exchange_strong(expected, 1))
			expected = 0;
	}
	void CSpinlock::UnLock()
	{
		m_busy.store(0);
	}
}