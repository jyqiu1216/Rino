#ifndef WJ_RoomManage_H
#define WJ_RoomManage_H

#include <list>
#include "common/WJBaseType.h"
#include "server/comstruct.h"
#include "GameRoom.h"
#include "Redis/WJRedis.h"

namespace WJ
{
	class CRoomManage
	{
	private:
		std::unordered_map<UINT, CGameRoom*> m_roomid2room;
	public:
		CRoomManage();
		~CRoomManage();
		bool LoadRoomInfoFromDb();
		void UpdateRoomInfoToLoginServer();
		//通过房间id获取房间实例
		CGameRoom * GetRoom(UINT uRoomId);
		//通过比赛id，获取房间实例，并且该实例必须有可用空桌子
		CGameRoom * GetRoom_ForDeskid(UINT uContestid, ISysDeskInterface**pDesk);
	};
}

#endif//WJ_RoomManage_H