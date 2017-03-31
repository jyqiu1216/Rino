#pragma once

#include <atomic>

/*
C++11实现自旋锁
自旋锁是一种用于保护多线程共享资源的锁，
与一般的互斥锁(mutex)不同之处在于当自旋锁尝试获取锁的所有权时会以忙等待(busy waiting)的形式不断的循环检查锁是否可用。
在多处理器环境中对持有锁时间较短的程序来说使用自旋锁代替一般的互斥锁往往能提高程序的性能。
*/


namespace WJ
{
	class CSpinlock
	{
	public:
		CSpinlock() 
		{
			m_busy.store(0);
		}
		void Lock();
		void UnLock();
	private:
		std::atomic<int>  m_busy;
	};
}