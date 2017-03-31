#ifndef __WJ_WJSocketSelector_H__
#define __WJ_WJSocketSelector_H__

#include <unordered_map>
#include <assert.h>
#include "common/WJCommonMarco.h"
#include "WJSocketMessage.h"

namespace WJ
{
	/*********************************************************
	*
	*						类说明
	*
	*********************************************************/
	template <class KEY>
	class WJSocketSelector
	{
		typedef typename std::unordered_map<KEY, WJSocketSelectorItem> SOCKET_OBSERVERS;
		typedef typename std::unordered_map<KEY, WJSocketSelectorItem>::iterator ITERATOR;

	public:
		WJSocketSelector()
		{
		}

		~WJSocketSelector()
		{
			clear();
		}

	public:
		void addSelector(KEY key, SEL_SocketMessage selector)
		{
			assert((nullptr != selector) && "selector is nullptr");
			if (nullptr == selector) return;

			auto tmp = _observers.find(key);
			if (tmp != _observers.end())
			{
				tmp->second = selector;
			}
			else
			{
				_observers.insert(std::make_pair(key, WJSocketSelectorItem(selector)));
			}
		}

		void removeSelector(KEY key)
		{
			auto selector = _observers.find(key);
			if (selector != _observers.end())
			{
				_observers.erase(key);
			}
		}

		/*
		*执行选择器
		*/
		bool executeSelector(KEY key, WJSocketMessage* socketMessage)
		{
			auto selector = _observers.find(key);
			if (selector != _observers.end())
			{
				selector->second.doCallSelector(socketMessage);
				return true;
			}
			return false;
		}

		ITERATOR begin()
		{
			return _observers.begin();
		}

		ITERATOR end()
		{
			return _observers.end();
		}

		bool empty()
		{
			return _observers.empty();
		}

		void clear() 
		{
			_observers.clear();
		}
	private:
		SOCKET_OBSERVERS _observers;
	};
}

#endif	//__WJ_WJSocketSelector_H__

