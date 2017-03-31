#include "RoomManage.h"
#include "gsconfig.h"
#include "common/commonfunction.h"
#include "common/WJGameInfo.h"
#include "gameserver.h"
#include "common/QkLogger.h"

using namespace std;

namespace WJ
{
	extern CGameServer * g_pGameServer;

	CRoomManage::CRoomManage()
	{
	}

	CRoomManage::~CRoomManage()
	{
		auto it = m_roomid2room.begin(), end = m_roomid2room.end();
		for (; it != end; ++it)
		{
			if (it->second)
			{
                WJ_SAFE_DELETE(it->second);
			}
		}
		m_roomid2room.clear();
	}

	//初始化所有服务器
	bool CRoomManage::LoadRoomInfoFromDb()
	{
		for (int i = 0; i < CONFIG_CONTEST_MAXNUM;++i)
		{
			if (0 == gsconfig()->contestid[i])
			{
				continue;
			}
			//获取比赛下的所有房间
			char key[MAX_PATH];
			sprintf_s(key, sizeof(key), "roomlist:%d", gsconfig()->contestid[i]);
            vector<string> vector_roomid;
			CGameServer::m_pRedis->sync_zrange(key, vector_roomid);
			for (UINT i = 0; i < vector_roomid.size(); ++i)
			{
				Room_ManageInfoStruct roominfo;
				CGameRoom* pGameRoom = new CGameRoom();
                assert(pGameRoom);

				//房间数据
				char key_room[MAX_PATH];
				//房间id
				roominfo.uRoomID = atoi(vector_roomid.at(i).c_str());
				//房间名
                sprintf_s(key_room, sizeof(key_room), 
                    "room:%d roomname gameid uDeskCount uDeskPeople contestid",
                    roominfo.uRoomID);
				//sprintf_s(key_room, sizeof(key_room), "room:%d roomname", roominfo.uRoomID);
                vector<string> roomDbInfo;
				CGameServer::m_pRedis->sync_hmget(key_room, roomDbInfo);
				std::memcpy(roominfo.cRoomName, roomDbInfo[0].c_str(), sizeof(roominfo.cRoomName));
                roominfo.cRoomName[sizeof(roominfo.cRoomName) - 1] = '\0';
                //游戏ID
				UINT uGameID = atoi(roomDbInfo[1].c_str());
				if (uGameID == 1)
				{
					roominfo.uGameID = e_gameid_10000001;
				}
				else if (uGameID == 2)
				{
					roominfo.uGameID = e_gameid_10000002;
				}
				else
				{
					roominfo.uGameID = uGameID;
				}
				//游戏桌数目
				roominfo.uDeskCount = atoi(roomDbInfo[2].c_str());
				//一桌最大游戏人数
				roominfo.uDeskPeople = atoi(roomDbInfo[3].c_str());;
				//比赛id
				roominfo.uContestid =atoi(roomDbInfo[4].c_str());
				//房间规则
                vector<string> contestDbInfo;
                sprintf_s(key_room, sizeof(key_room), 
                    "contest:%d team contestname uContestRule uContestKind _i64cost awardType award", 
                    roominfo.uContestid);
				CGameServer::m_pRedis->sync_hmget(key_room, contestDbInfo);
				roominfo.uRoomRule = (ENUM_RoomRule)atoi(contestDbInfo[0].c_str());
				//比赛名
			    std::memcpy(roominfo.cContestName, contestDbInfo[1].c_str(), sizeof(roominfo.cContestName));
                roominfo.cContestName[sizeof(roominfo.cContestName) - 1] = '\0';
				//比赛模式
				roominfo.uContestRule = (ENUM_ContestRule)atoi(contestDbInfo[2].c_str());
				//比赛场次类型
				roominfo.uContestKind = (ENUM_ContestKind)atoi(contestDbInfo[3].c_str());
				//报名费用
				roominfo._i64cost = atoi(contestDbInfo[4].c_str());
				//奖励
				sprintf_s(key_room, "contest:%d awardType", roominfo.uContestid);
				roominfo.awardType = (ENUM_AwardType)atoi(contestDbInfo[5].c_str());
				sprintf_s(key_room, "contest:%d award", roominfo.uContestid);
				roominfo.award = atoi(contestDbInfo[6].c_str());

				//比赛数据也收集下
				ContestInfo_locad cmd;
				//比赛名
				std::memcpy(cmd.cContestName, roominfo.cContestName, sizeof(cmd.cContestName));
				//比赛模式
				cmd.uContestRule = roominfo.uContestRule;
				//比赛场次类型
				cmd.uContestKind = roominfo.uContestKind;
				//报名费用
				cmd._i64cost = roominfo._i64cost;
				//奖励
				cmd.awardType = roominfo.awardType;
				cmd.award = roominfo.award;

				g_pGameServer->AddContInfo_locad(roominfo.uContestid, cmd);

				pGameRoom->Init_Room(roominfo);
                auto retPair = m_roomid2room.insert(make_pair(roominfo.uRoomID, pGameRoom));
                if (!retPair.second)
				{
                    WJ_SAFE_DELETE(pGameRoom);
                }
				pGameRoom->Init_subclass();
				pGameRoom->Start_subclass();


			}
		}

		QK_InfoLog("初始化所有服务器完成。");
		return true;
	}

	void CRoomManage::UpdateRoomInfoToLoginServer()
	{
		auto it = m_roomid2room.begin(), end = m_roomid2room.end();
		for (; it != end; ++it)
		{
			if (it->second)
			{
				it->second->UpdateInfoToLS();
			}
		}
	}

	CGameRoom * CRoomManage::GetRoom(UINT uRoomId)
	{
		auto it = m_roomid2room.find(uRoomId);
		if (it != m_roomid2room.end())
		{
			return it->second;
		}
		return nullptr;
	}

	//通过比赛id，获取房间实例，并且该实例必须有可用空桌子
	CGameRoom * CRoomManage::GetRoom_ForDeskid(UINT uContestid, ISysDeskInterface**pDesk)
	{
		auto it = m_roomid2room.begin();
		while (it != m_roomid2room.end())
		{
			if (it->second->GetRoom_ManageInfoStruct()->uContestid == uContestid)
			{
				//先为玩家找桌子
				UINT ucount = it->second->GetCGameDeskManage()->GetCount();//桌子总数
				ISysDeskInterface* p = nullptr;
				bool bOK = false;//人准备好了，可以搓一桌
				bool bGetDesk = false;//是否分配到桌子了
				for (UINT i = 0; i < ucount; ++i)
				{
					p = it->second->GetCGameDeskManage()->At(i);
					if (nullptr == p || p->IsPlayGame())
					{//在游戏中 或 不能使用，排除
						continue;
					}

					bOK = false;
					UINT uplayernum = p->GetDeskPlayerNum();
					if (0 == uplayernum)
					{//是空桌子才使用
						*pDesk = p;
						return it->second;
					}//if (0 == uplayernum)
				}//for (UINT i = 0; i < ucount; ++i)
			}
			++it;
		}

		return nullptr;
	}
}