#ifndef WJ_GameRoom_H
#define WJ_GameRoom_H

#include <unordered_map>
#include "UserInfoManage.h"
#include "GameDeskManage.h"
#include "QueueManage.h"
#include "server/WJSSocket.h"

namespace WJ
{
	class CGameRoom
	{
	private:
		CGameDeskManage						m_GameDeskManage;	//桌子管理
		Room_ManageInfoStruct				m_initRoomData;		//房间数据

	public:
		CGameRoom();
		~CGameRoom();

		const Room_ManageInfoStruct* GetRoom_ManageInfoStruct();
		CGameDeskManage* GetCGameDeskManage();
		
		///初始化房间数据
		void Init_Room(Room_ManageInfoStruct &initRoomData);
		///初始化函数
		virtual bool Init_subclass();
		///取消初始化函数
		virtual bool UnInit_subclass();
		///启动函数
		virtual bool Start_subclass();
		///停止服务
		virtual bool Stop_subclass();

		void UpdateInfoToLS();

		//批量发送函数
		void SendDataBatch(void * pData, UINT uSize, UINT uMainID, UINT uAssistantID, UINT uHandleCode);
	public:
		//用户列表消息
		bool OnSocketRead(CWJSSocket *pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize);

		bool OnSocketClose(CWJSSocket *pSocket);
		bool OnTimerMessage(UINT uTimerID);
	private:
		//用户列表消息
		bool OnSocketRead_user_list(CWJSSocket *pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize);
		//用户动作消息
		bool OnSocketRead_user_action(CWJSSocket *pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize);
		//房间信息
		bool OnSocketRead_room(CWJSSocket *pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize);
		//排队消息
		bool OnSocketRead_queue(CWJSSocket *pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize);
		//游戏框架消息
		bool OnSocketRead_game_frame(CWJSSocket *pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize);
		//游戏普通消息
		bool OnSocketRead_game_notify(CWJSSocket *pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize);
	};
}

#endif//WJ_GameRoom_H