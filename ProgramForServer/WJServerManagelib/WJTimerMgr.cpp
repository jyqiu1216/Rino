#include "server/WJTimerMgr.h"
#include <utility>
#include <windows.h>

using namespace std;

namespace WJ
{

    bool CWJTimerMgr::AddTimer(CWJTimer * pTimer)
    {
		if (pTimer == NULL || pTimer->m_interval <= 0 || pTimer->m_pCallBack_Time == NULL)
        {
            return false;
        }
		time_t curTime = time(NULL);
		pTimer->m_deadLine = curTime + pTimer->m_interval;
		m_timersLock.Lock();
		auto it = m_timerid2timers.find(pTimer->m_timerID);
		if (it != m_timerid2timers.end())
		{
			it->second = *pTimer;
		}
		else
		{
			m_timerid2timers.insert(make_pair(pTimer->m_timerID, *pTimer));
		}
        m_timersLock.UnLock();
        return true;
    }

    bool CWJTimerMgr::DelTimer(CWJTimer * pTimer)
    {
        if (pTimer == NULL)
        {
            return false;
        }
		bool ret = DelTimer(pTimer->m_timerID);
		return ret;
    }

	bool CWJTimerMgr::DelTimer(TimerID m_timerID)
	{
		m_timersLock.Lock();
		m_timerid2timers.erase(m_timerID);
		auto it = m_timeoutTimers.begin(), end = m_timeoutTimers.end();
		for (; it != end; ++it)
		{
			if (it->m_timerID == m_timerID)
			{
				break;
			}
		}
		if (it != end)
		{
			m_timeoutTimers.erase(it);
		}
		m_timersLock.UnLock();
		return true;
	}

    int CWJTimerMgr::ProcessTimerEvents()
    {
        int count = 0;
		CWJTimer timer;
		while (true)
		{
			m_timersLock.Lock();
			if (m_timeoutTimers.empty())
			{
				m_timersLock.UnLock();
				return count;
			}
			else
			{
				timer = m_timeoutTimers.front();
				m_timeoutTimers.pop_front();
			}
			m_timersLock.UnLock();
			timer.m_pCallBack_Time->OnTimerMessage(timer.m_timerID);
			++count;
		}
        return count;
    }

    void CWJTimerMgr::ThreadTimerProc()
    {
        while (m_bRunning)
        {
            time_t curTime = time(NULL);
            m_timersLock.Lock();
			auto it = m_timerid2timers.begin(), end = m_timerid2timers.end();
            for (; it != end; ++it)
            {
                if (it->second.m_deadLine <= curTime)
                {
					it->second.m_deadLine += it->second.m_interval;
                    m_timeoutTimers.push_back(it->second);
                }
            }
            m_timersLock.UnLock();
            Sleep(1000);
        }
    }

}