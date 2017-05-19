#include "clientsocket/WJSocketMessage.h"
#include "common/WJMemoryPool.h"
#include "message/WJNetMessageHead.h"

namespace WJ
{
	//////////////////////////////////////////////////////////////////////////
	WJMemPool gMemPool(sizeof(WJSocketMessage), 32, 1024);

	WJSocketMessage* WJSocketMessage::getMessage()
	{
		return new WJSocketMessage();
	}

	void WJSocketMessage::releaseMessage(WJSocketMessage* message)
	{
		WJ_SAFE_DELETE(message);
	}

	WJSocketMessage::WJSocketMessage() : objectSize(0), socketStatus(SocketStatus_UNKNOW)
	{
		::memset(object, 0x0, sizeof(object));
		::memset(&messageHead, 0x0, sizeof(object));
	}

	WJSocketMessage::~WJSocketMessage()
	{

	}

	void *WJSocketMessage::operator new(std::size_t ObjectSize)
	{
		return gMemPool.get();
	}

	void WJSocketMessage::operator delete(void *ptrObject)
	{
		gMemPool.release(ptrObject);
	}

	void WJSocketMessage::setMessage(const NetMessageHead* head, CHAR* obj, INT Size)
	{
		messageHead = *head;
		objectSize = Size;
		memcpy(object, obj, objectSize);
		CHAR buf[16] = {0};
		sprintf(buf, "%u_%u", messageHead.bMainID, messageHead.bAssistantID);
		strKey.assign(buf);//消息主id与消息辅助id共同做成该消息信息的唯一标示
	}
}
