#ifndef __WJ_WJSocketProtocolData_H__
#define __WJ_WJSocketProtocolData_H__
#include <stdio.h>
#include < assert.h>
#include "common/WJBaseType.h"
#include "common/WJCommonMarco.h"
#include "message/WJNetMessageHead.h"

namespace WJ {
	/*********************************************************
	*
	*						类说明
	*套接字协议数据类(用于把需要发送给服务端的数据用约定的包头
	*加数据包装起来)
	*
	*********************************************************/
	class WJSocketProtocolData
	{
	public:
		WJSocketProtocolData() : _objectSize(0)
		{
			::memset(_object, 0x0, sizeof(_object));
			::memset(&_messageHead, 0x0, sizeof(NetMessageHead));
		}

		/*
		* 创建网络数据包
		*/
		bool createPackage(UINT MainID, UINT AssistantID, UINT uHandleCode,UINT CheckCode, void* object = nullptr, INT objectSize = 0)
		{
			//assert(INVALID_VALUE != CheckCode);
			if (INVALID_VALUE == CheckCode) return false;
			
			UINT messageHeadSize = sizeof(NetMessageHead);

			_objectSize = messageHeadSize + objectSize; 

			assert((_objectSize < CACHE_BUFFER_SIZE) && "data size > cache buffer.");

			bool ret = false;
			do
			{
				if (_objectSize <= CACHE_BUFFER_SIZE)
				{
					_messageHead.uMessageSize = _objectSize;
					_messageHead.bMainID = MainID;
					_messageHead.bAssistantID = AssistantID;
					_messageHead.bHandleCode = uHandleCode;
					_messageHead.bReserve = CheckCode;
					::memcpy(_object, &_messageHead, messageHeadSize);
					if (nullptr != object)
					{
						::memcpy(_object + messageHeadSize, (void*)object, _objectSize);
					}
					//Description();
					ret = true;
				}
			} while (0);

			return ret;
		}

		/*
		* 获得数据内容
		*/
		CHAR* getPackage()
		{
			return _object;
		}

		/*
		* 获得数据内容大小
		*/
		INT getPackageSize() const
		{
			return _objectSize;
		}

		/*
		* 打印日志
		*/
		void Description() 
		{
# if defined(_DEBUG) || defined(DEBUG)
			CHAR buf[512] = {0};
			sprintf(buf, "NetMessageHead = {uMessageSize = %d bMainID = %d bAssistantID = %d bHandleCode = %d bReserve = %d object = %s objectSize = %d}",
				_messageHead.uMessageSize, 
				_messageHead.bMainID,
				_messageHead.bAssistantID,
				_messageHead.bHandleCode,
				_messageHead.bReserve, 
				_object,
				_objectSize);
			printf("%s", buf);
#endif	//_DEBUG || DEBUG
		}

	public:
		const static UINT CACHE_BUFFER_SIZE = 1024*10;

	private:
		NetMessageHead	_messageHead;//包头
		CHAR			_object[CACHE_BUFFER_SIZE];//数据内容
		INT				_objectSize;//数据内容大小
	};
}

#endif	//__WJ_WJSocketProtocolData_H__

