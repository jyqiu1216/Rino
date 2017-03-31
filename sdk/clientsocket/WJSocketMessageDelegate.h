#ifndef __WJ_WJGameMessageDelegate_H__
#define __WJ_WJGameMessageDelegate_H__

#include "common/WJBaseType.h"
#include "message/WJNetMessageHead.h"
namespace WJ {
	class WJSocketMessage;
	/*********************************************************
	*
	*						类说明
	*上层网络操作类接口父类
	*
	*********************************************************/
	class ISocketMessageDelegate
	{
	public:
		virtual ~ISocketMessageDelegate(void) {}
		virtual void onSocketMessage(WJSocketMessage* socketMessage) = 0;
		virtual void onConnected(bool connect, emSocketStatus status) = 0;
		virtual void onDisConnect() = 0;
	};

};

#endif	//__WJ_WJGameMessageDelegate_H__

