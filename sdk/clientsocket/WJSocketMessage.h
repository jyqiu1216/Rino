#ifndef __WJ_WJSocketMessage_H__
#define __WJ_WJSocketMessage_H__

#include "common/WJBaseType.h"
#include "message/WJNetMessageHead.h"
#include <functional>
#include <string>

namespace WJ 
{

	#define SELECTER_KEY_FORMAT	"%u_%u"

	class WJSocketMessage;
	typedef std::function<bool(WJSocketMessage* socketMessage)> SEL_SocketMessage;

	#define WJ_SOCKET_CALLBACK(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, ##__VA_ARGS__)

	/*********************************************************
	*
	*						类说明
	*
	*********************************************************/
	class WJCLIENTSOCKETLOGIC_EXPORT WJSocketMessage
	{
		public:
			static WJSocketMessage* getMessage();
			static void releaseMessage(WJSocketMessage* message);

		public:
			void setMessage(const NetMessageHead* head, CHAR* obj, INT Size);

		private:
			WJSocketMessage();
			~WJSocketMessage();

 			void *operator new(std::size_t ObjectSize);
 			void operator delete(void *ptrObject);

		public:
			NetMessageHead messageHead;
			UINT objectSize;			// message size
			CHAR object[4096];
			emSocketStatus socketStatus;
			std::string strKey;
			
	};

	/*********************************************************
	*
	*						类说明
	*
	*********************************************************/
	class WJSocketSelectorItem
	{
	public:
		WJSocketSelectorItem(SEL_SocketMessage selector) : _selector(selector)
		{
		}

	public:
		bool doCallSelector(WJSocketMessage* socketMessage)
		{ 
			return (_selector)(socketMessage);
		}

	private:
		SEL_SocketMessage	_selector;
	};
};

#endif	//__WJ_WJSocketMessage_H__
