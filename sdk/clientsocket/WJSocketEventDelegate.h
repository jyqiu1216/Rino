#ifndef __WJ_WJSocketMessageDelegate_H__
#define __WJ_WJSocketMessageDelegate_H__

#include "common/WJBaseType.h"

namespace WJ 
{
	/*********************************************************
	*
	*						类说明
	*
	*********************************************************/
	class ISocketEventDelegate
	{
	public:
		virtual ~ISocketEventDelegate() {}
		//virtual void onReadComplete() = 0;
	};

}

#endif	//__WJ_WJSocketMessageDelegate_H__
