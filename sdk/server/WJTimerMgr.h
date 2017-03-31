// 简单计时器，不适合成千上万的计时器场景. 100 个计时器（或更少）的场景很适合

#pragma once

#include <functional>
#include <thread>
#include <list>
#include <unordered_map>
#include "common/WJSpinlock.h"


namespace WJ
{
    typedef unsigned int TimerID;

	class CallBack_Time
	{
	public:
		///定时器消息 （必须重载）
		virtual bool OnTimerMessage(TimerID timerId) = 0;
	};

    struct CWJTimer
    {
        TimerID m_timerID;  // input   千万保证tiemerID唯一, 不重复
        int     m_interval; // input	时间间隔
		CallBack_Time* m_pCallBack_Time;  //input 回调对象
        time_t  m_deadLine; // interval use
    };
    
    class CWJTimerMgr
    {
    public:
        CWJTimerMgr()
        {
            m_timerIdSeed = 0;
            m_bRunning = true;
            m_timerThread = std::thread(&CWJTimerMgr::ThreadTimerProc, this);
        }
        ~CWJTimerMgr()
        {
            m_bRunning = false;
            m_timerThread.join();
        }
        bool AddTimer(CWJTimer* pTimer);
        bool DelTimer(CWJTimer * pTimer);
		bool DelTimer(TimerID m_timerID);

        int  ProcessTimerEvents();  //返回处理超时事件个数
    private:
        void ThreadTimerProc();

	private:
        std::thread m_timerThread;
        bool m_bRunning;

        TimerID               m_timerIdSeed;

		std::unordered_map<TimerID, CWJTimer>  m_timerid2timers;  //所有已添加的计时器
        std::list<CWJTimer>   m_timeoutTimers;  //已到期的计时器
        CSpinlock             m_timersLock;
        
    };
}