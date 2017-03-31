#ifndef __WJ_WJUIDelegateQueue_H__
#define __WJ_WJUIDelegateQueue_H__

#include "common/WJCommonMarco.h"
#include <unordered_map>
#include <assert.h>

namespace WJ
{
	template <class T>
	class WJUIDelegateQueue
	{
		typedef typename std::unordered_map<T, T> DelegateQueue;
		typedef typename std::unordered_map<T, T>::iterator ITERATOR;

	public:
		WJUIDelegateQueue()
		{
			_uiQueue = new DelegateQueue();
		}

		~WJUIDelegateQueue()
		{
			WJ_SAFE_DELETE(_uiQueue);
		}

	public:
		void addObserver(T delegate)
		{
			assert(nullptr != delegate);
			{
				auto Value = std::make_pair(delegate, delegate);
				_uiQueue->insert(Value);
			}
		}

		void removeObserver(T delegate)
		{
			assert(nullptr != delegate);
			{
				auto iter = _uiQueue->find(delegate);
				if (iter != _uiQueue->end())
				{
					_uiQueue->erase(iter);
				}
			}
		}

		ITERATOR begin()
		{
			return _uiQueue->begin();
		}

		ITERATOR end()
		{
			return _uiQueue->end();
		}

	private:
		DelegateQueue * _uiQueue;
	};
}

#endif	//__WJ_WJUIDelegateQueue_H__

