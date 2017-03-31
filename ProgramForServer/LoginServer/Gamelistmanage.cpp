
#include "Gamelistmanage.h"

#include <string>
#include "common/commonfunction.h"
#include "common\WJCommonMarco.h"
#include "common\QkLogger.h"

using namespace std;

namespace WJ
{
	CGameList::CGameList()
	{
		clear();
	}

	CGameList::~CGameList()
	{
		clear();
	}

	void CGameList::InitLoadFromDB(CWJRedis *pRedis)
	{
#if 0 //test 生成ai
		char name_key[64] = { 0 };
		char uid_key[64] = { 0 };
		char chip_key[64] = { 0 };
		char coin_key[64] = { 0 };
		char value[1024] = { 0 };
		for (int i = 1; i <= 60000; ++i)
		{
			if (i % 1000 == 0)
			{
				QK_WarnLog("insert %d user", i);
			}
			sprintf_s(name_key, sizeof(name_key), "username:AI%d", i);
			sprintf_s(value, sizeof(value), "%d", i);
			pRedis->sync_set(name_key, value);
			sprintf_s(uid_key, sizeof(uid_key), "user:%d", i);
			sprintf_s(value, sizeof(value), "passwd 35aaafa878438f9f85e6db7ed42e0511 isBind 0 nickname AI%d telephone 13656245214 id %d inType 3 createDate 2016-11-30T20:48:00 username AI%d", 
				i,
				i,
				i);
			pRedis->sync_hmset(uid_key, value);
			sprintf_s(chip_key, sizeof(uid_key), "redis_user_chip%d", i);
			pRedis->sync_set(chip_key, "99999999");
			sprintf_s(coin_key, sizeof(uid_key), "redis_user_coin%d", i);
			pRedis->sync_set(coin_key, "99999999");
		}
		exit(0);
#endif
		if (pRedis == nullptr)  return;

		std::vector<string> vector_gameidStr;
		pRedis->sync_zrange("gamelist", vector_gameidStr);
		auto iter_gameid = vector_gameidStr.begin();
		while (iter_gameid != vector_gameidStr.end())
		{//收集所有的游戏名节点			
			ComGameinfo _ComGameinfo;
			_ComGameinfo.gameid = atoi(iter_gameid->c_str());

			char key[MAX_PATH] = { 0 };
			sprintf_s(key, "game:%d gamename", _ComGameinfo.gameid);
			string gamename;
			pRedis->sync_hget(key, gamename);
			if (IsTextUTF8(gamename.c_str(), gamename.length()))
			{
				char* res = nullptr;
				res = U2G(gamename.c_str());
				std::memcpy(_ComGameinfo.gamename, res, sizeof(_ComGameinfo.gamename));
				WJ_SAFE_DELETE(res);
			}
			else
			{
				std::memcpy(_ComGameinfo.gamename, gamename.c_str(), sizeof(_ComGameinfo.gamename));
			}

			//_ComGameinfo_list.push_back(_ComGameinfo);
			m_game_node[_ComGameinfo.gameid] = _ComGameinfo;

			std::vector<string> vector_contestidStr;//找该游戏区服下的比赛

			sprintf_s(key, "game:%d haserea", _ComGameinfo.gameid);
			string haserea;
			pRedis->sync_hget(key, haserea);
			//int haserea = hget_int(key);
			if (haserea.compare("1") == 0)
			{//该游戏下的区服
				char key_erea[MAX_PATH] = { 0 };
				sprintf_s(key_erea, "erealist:%d", _ComGameinfo.gameid);
				//std::vector<int> vector_ereaid = command_ZRANGE_int(key_erea);
				std::vector<string> vector_ereaidStr;
				pRedis->sync_zrange(key_erea, vector_ereaidStr);
				auto iter_ereaid = vector_ereaidStr.begin();
				while (iter_ereaid != vector_ereaidStr.end())
				{
					//收集该游戏节点下的所有区服节点
					ComEreainfo _ComEreainfo;
					_ComEreainfo.gameid = _ComGameinfo.gameid;
					_ComEreainfo.ereaid = atoi(iter_ereaid->c_str());
					sprintf_s(key_erea, "erea:%d ereaname", _ComEreainfo.ereaid);
					//std::string ereaname = hget_str(key_erea);
					string ereaname;
					pRedis->sync_hget(key_erea, ereaname);
					if (IsTextUTF8(ereaname.c_str(), ereaname.length()))
					{
						char* res = nullptr;
						res = U2G(ereaname.c_str());
						std::memcpy(_ComEreainfo.ereaname, res, sizeof(_ComEreainfo.ereaname));
						WJ_SAFE_DELETE(res);
					}
					else
					{
						std::memcpy(_ComEreainfo.ereaname, ereaname.c_str(), sizeof(_ComEreainfo.ereaname));
					}

					//_ComEreainfo_list.push_back(_ComEreainfo);
					m_erea_node[_ComEreainfo.ereaid] = _ComEreainfo;

					sprintf_s(key, "fightlist:%d_%d", _ComEreainfo.gameid, _ComEreainfo.ereaid);
					std::vector<string> vector_contestid_midd_str;//找该游戏区服下的比赛
					//vector_contestid_midd = command_ZRANGE_int(key);
					pRedis->sync_zrange(key, vector_contestid_midd_str);

					vector_contestidStr.insert(vector_contestidStr.end(), vector_contestid_midd_str.begin(), vector_contestid_midd_str.end());
					++iter_ereaid;
				}
			}
			else
			{//无区服的情况
				sprintf_s(key, "fightlist:%d", _ComGameinfo.gameid);
				//vector_contestid = command_ZRANGE_int(key);
				pRedis->sync_zrange(key, vector_contestidStr);
			}

			auto iter_contestid = vector_contestidStr.begin();
			while (iter_contestid != vector_contestidStr.end())
			{
				char key_contest[MAX_PATH] = { 0 };
				//收集该游戏节点下的所有比赛节点
				ComContestInfo _ComContestInfo;
				_ComContestInfo.gameid = _ComGameinfo.gameid;
				_ComContestInfo.contestid = atoi(iter_contestid->c_str());
				vector<string> contestInfoStrs;
				sprintf_s(key_contest, 
					"contest:%d ereaid team contestname cContestDescribe uContestRule "
					"uContestKind _i64cost awardType award", _ComContestInfo.contestid);
				pRedis->sync_hmget(key_contest, contestInfoStrs);
				
				_ComContestInfo.ereaid = atoi(contestInfoStrs[0].c_str());
				
				//房间规则
				_ComContestInfo.uRoomRule = (ENUM_RoomRule)atoi(contestInfoStrs[1].c_str());
				string contestname = contestInfoStrs[2];
				if (IsTextUTF8(contestname.c_str(), contestname.length()))
				{
					char* res = nullptr;
					res = U2G(contestname.c_str());
					std::memcpy(_ComContestInfo.contestname, res, sizeof(_ComContestInfo.contestname));
					WJ_SAFE_DELETE(res);
				}
				else
				{
					std::memcpy(_ComContestInfo.contestname, contestname.c_str(), sizeof(_ComContestInfo.contestname));
				}
				string cContestDescribe = contestInfoStrs[3];
				if (IsTextUTF8(cContestDescribe.c_str(), cContestDescribe.length()))
				{
					char* res = nullptr;
					res = U2G(cContestDescribe.c_str());
					std::memcpy(_ComContestInfo.cContestDescribe, res, sizeof(_ComContestInfo.cContestDescribe));
					WJ_SAFE_DELETE(res);
				}
				else
				{
					std::memcpy(_ComContestInfo.cContestDescribe, cContestDescribe.c_str(), sizeof(_ComContestInfo.cContestDescribe));
				}

				_ComContestInfo.uContestRule = (ENUM_ContestRule)atoi(contestInfoStrs[4].c_str());
				_ComContestInfo.uContestKind = (ENUM_ContestKind)atoi(contestInfoStrs[5].c_str());
				_ComContestInfo._i64cost = atoll(contestInfoStrs[6].c_str());
				_ComContestInfo.awardType = (ENUM_AwardType)atoi(contestInfoStrs[7].c_str());
				_ComContestInfo.award = atoi(contestInfoStrs[8].c_str());

				//发送比赛节点
				//_ComContestInfo_list.push_back(_ComContestInfo._data);
				m_contest_node[_ComContestInfo.contestid] = _ComContestInfo;

				//获取比赛下的所有房间
				sprintf_s(key, "roomlist:%d", _ComContestInfo.contestid);
				//std::vector<int> vector_roomid = command_ZRANGE_int(key);
				vector<string> vector_roomidStr;
				pRedis->sync_zrange(key, vector_roomidStr);
				auto iter_roomid = vector_roomidStr.begin();
				while (iter_roomid != vector_roomidStr.end())
				{
					char key_room[MAX_PATH] = { 0 };
					ComRoomInfo _ComRoomInfo;
					_ComRoomInfo.gameid = _ComGameinfo.gameid;
					_ComRoomInfo.roomid = atoi(iter_roomid->c_str());
					_ComRoomInfo.contestid = _ComContestInfo.contestid;
					sprintf_s(key_room, "room:%d ereaid roomname strIP", _ComRoomInfo.roomid);
					vector<string> roomInfoStrs;
					pRedis->sync_hmget(key_room, roomInfoStrs);
					_ComRoomInfo.ereaid = atoi(roomInfoStrs[0].c_str());
					string roomname = roomInfoStrs[1];
					if (IsTextUTF8(roomname.c_str(), roomname.length()))
					{
						char* res = nullptr;
						res = U2G(roomname.c_str());
						std::memcpy(_ComRoomInfo.roomname, res, sizeof(_ComRoomInfo.roomname));
						WJ_SAFE_DELETE(res);
					}
					else
					{
						std::memcpy(_ComRoomInfo.roomname, roomname.c_str(), sizeof(_ComRoomInfo.roomname));
					}
					string strIP = roomInfoStrs[2];

					if (IsTextUTF8(strIP.c_str(), strIP.length()))
					{
						char* res = nullptr;
						res = U2G(strIP.c_str());
						std::memcpy(_ComRoomInfo.strIP, res, sizeof(_ComRoomInfo.strIP));
						WJ_SAFE_DELETE(res);
					}
					else
					{
						std::memcpy(_ComRoomInfo.strIP, strIP.c_str(), sizeof(_ComRoomInfo.strIP));
					}
					_ComRoomInfo.uOlineCount = 0;
					_ComRoomInfo.uport = 0;
					//发送房间节点
					ComRoomInfo_node roomInfo_node;
					roomInfo_node.room_node = _ComRoomInfo;
					m_room_node[_ComRoomInfo.roomid] = roomInfo_node;

					++iter_roomid;
				}

				++iter_contestid;
			}

			++iter_gameid;
		}
		int a;
		a = 2;
	}

	//清除所有记录
	void CGameList::clear()
	{
		m_game_node.clear();
		m_erea_node.clear();
		m_contest_node.clear();
		m_room_node.clear();
	}

	//添加游戏节点
	void CGameList::addgame_node(ComGameinfo &node)
	{
		auto iter = m_game_node.find(node.gameid);
		if (iter != m_game_node.end())
		{
			std::memcpy(&((*iter).second), &(node), sizeof(ComGameinfo));
			return;
		}
		m_game_node.insert(std::map<long, ComGameinfo>::value_type(node.gameid, node));
	}

	//添加区服节点
	void CGameList::adderea_node(ComEreainfo &node)
	{
		auto iter = m_erea_node.find(node.ereaid);
		if (iter != m_erea_node.end())
		{
			std::memcpy(&((*iter).second), &(node), sizeof(ComEreainfo));
			return;
		}
		m_erea_node.insert(std::map<long, ComEreainfo>::value_type(node.ereaid, node));
	}

	//添加比赛节点
	void CGameList::addcontest_node(ComContestInfo &node)
	{
		auto iter = m_contest_node.find(node.contestid);
		if (iter != m_contest_node.end())
		{
			std::memcpy(&((*iter).second), &(node), sizeof(ComContestInfo));
			return;
		}
		m_contest_node.insert(std::map<long, ComContestInfo>::value_type(node.contestid, node));
	}

	//添加房间节点
	void CGameList::addroom_node(ComRoomInfo_node &node)
	{
		auto iter = m_room_node.find(node.room_node.roomid);
		if (iter != m_room_node.end())
		{
			std::memcpy(&((*iter).second), &(node), sizeof(ComRoomInfo_node));
			return;
		}
		m_room_node.insert(std::map<long, ComRoomInfo_node>::value_type(node.room_node.roomid, node));

	
	}

	//获取游戏节点数目
	UINT CGameList::Getgame_nodeCount()
	{
		return (UINT)m_game_node.size(); 
	}

	//获取区服节点数目
	UINT CGameList::Geterea_nodeCount()
	{
		return (UINT)m_erea_node.size();
	}

	//获取比赛节点数目
	UINT CGameList::Getcontest_nodeCount()
	{
		return (UINT)m_contest_node.size();
	}

	//获取房间节点数目
	UINT CGameList::Getroom_nodeCount()
	{
		return (UINT)m_room_node.size();
	}
	

	
	//获取游戏类型
	UINT CGameList::FillGameName(char * pBuffer, UINT uBufferSize, UINT uBeginPos, bool & bFinish, UINT &uCopyCount)
	{
		//拷贝数据
		uCopyCount = 0;//拷贝了几个
		UINT i = uBeginPos;//移动的节点
		if (uBeginPos < m_game_node.size())
		{
			for (auto iter = m_game_node.begin(); iter != m_game_node.end(); ++iter, ++i)
			{
				if (((uCopyCount + 1)*sizeof(ComGameinfo)) >= uBufferSize)
				{
					break;
				}

				std::memcpy(pBuffer + uCopyCount*sizeof(ComGameinfo), &(iter->second), sizeof(ComGameinfo));
				++uCopyCount;
			}
		}

		//拷贝完成
		bFinish = (i >= m_game_node.size());

		return i;
	}

	//获取区服类型
	UINT CGameList::FillErea(char * pBuffer, UINT uBufferSize, UINT uGameID, UINT uBeginPos, bool & bFinish, UINT &uCopyCount)
	{
		//拷贝数据
		uCopyCount = 0;//拷贝了几个
		UINT i = uBeginPos;//移动的节点
		if (uBeginPos < m_erea_node.size())
		{
			for (auto iter = m_erea_node.begin(); iter != m_erea_node.end(); ++iter, ++i)
			{
				if (((uCopyCount + 1)*sizeof(ComEreainfo)) >= uBufferSize)
				{
					break;
				}

				if (uGameID == iter->second.gameid)
				{
					std::memcpy(pBuffer + uCopyCount*sizeof(ComEreainfo), &(iter->second), sizeof(ComEreainfo));
					uCopyCount++;
				}
			}
		}

		//拷贝完成
		bFinish = (i >= m_erea_node.size());

		return i;
	}

	//获取游戏名字
	UINT CGameList::FillGameContest(char * pBuffer, UINT uBufferSize, UINT uEreaID, UINT uBeginPos, bool & bFinish, UINT &uCopyCount)
	{
		//拷贝数据
		uCopyCount = 0;//拷贝了几个
		UINT i = uBeginPos;//移动的节点
		if (uBeginPos < m_contest_node.size())
		{
			for (auto iter = m_contest_node.begin(); iter != m_contest_node.end(); ++iter, ++i)
			{
				if (((uCopyCount + 1)*sizeof(ComContestInfo)) >= uBufferSize)
				{
					break;
				}

				if (uEreaID == iter->second.ereaid)
				{
					std::memcpy(pBuffer + uCopyCount*sizeof(ComContestInfo), &(iter->second), sizeof(ComContestInfo));
					uCopyCount++;
				}
			}
		}

		//拷贝完成
		bFinish = (i >= m_contest_node.size());

		return i;
	}

	//获取房间名字
	UINT CGameList::FillGameRoom(char * pBuffer, UINT uBufferSize, UINT uContestID, UINT uBeginPos, bool & bFinish, UINT &uCopyCount)
	{
		//拷贝数据
		uCopyCount = 0;//拷贝了几个
		UINT i = uBeginPos;//移动的节点
		if (uBeginPos < m_room_node.size())
		{
			for (auto _iter = m_room_node.begin(); _iter != m_room_node.end(); ++_iter, ++i)
			{
				if (((uCopyCount + 1)*sizeof(ComRoomInfo)) >= uBufferSize)
				{
					break;
				}

				if (uContestID == _iter->second.room_node.contestid)
				{
					std::memcpy(pBuffer + uCopyCount*sizeof(ComRoomInfo), &(_iter->second.room_node), sizeof(ComRoomInfo));
					uCopyCount++;
				}
			}
		}

		//拷贝完成
		bFinish = (i >= m_room_node.size());

		return i;
	}

	//获得比赛数据
	const ComContestInfo* CGameList::GetComContestInfo(UINT uContestid)
	{
		auto iter = m_contest_node.find(uContestid);
		if( iter != m_contest_node.end())
		{
			
			return &((*iter).second);
		}

		return nullptr;
	}

	//获得房间数据
	const ComRoomInfo* CGameList::GetComRoomInfo(UINT uroomid)
	{
		auto iter = m_room_node.find(uroomid);
		if (iter != m_room_node.end())
		{

			return &((*iter).second.room_node);
		}


		return nullptr;
	}

	//获得房间数据
	const ComRoomInfo* CGameList::GetComRoomInfoEx(UINT uroomid)
	{
		for (auto iter = m_room_node.begin(); iter != m_room_node.end(); ++iter)
		{
			if ((*iter).second.room_node.roomid == uroomid)
			{
				return &((*iter).second.room_node);
			}
		}

		return nullptr;
	}

	//该比赛是否有房间
	bool CGameList::ExistRoomForContest(UINT uContestid)
	{
		for (auto iter = m_room_node.begin(); iter != m_room_node.end(); ++iter)
		{
			if ((*iter).second.room_node.contestid == uContestid)
			{
				return true;
			}
		}

		return false;
	}

	//获得房间的套接字id
	bool CGameList::GetRoomSocketID(UINT roomid, UINT &usocketindex)
	{
		usocketindex = 0;
		for (auto iter = m_room_node.begin(); iter != m_room_node.end(); ++iter)
		{
			if ((*iter).second.room_node.roomid == roomid)
			{
				usocketindex = (*iter).second.usocketindex;
				return true;
			}
		}

		return false;
	}

	//获得房间列表
	void CGameList::GetComRoomList(std::vector<ComRoomInfo_node> &room_node, UINT uContestid)
	{
		for (auto iter = m_room_node.begin(); iter != m_room_node.end(); ++iter)
		{
			if ((*iter).second.room_node.contestid == uContestid)
			{
				int nSize = (int)m_room_node.size();
				for (int i = 0; i < nSize; i++)
				{
					room_node.push_back(m_room_node[i]);
				}
			}
		}
	}

	//该房间当前分配有多少比赛队伍
	UINT CGameList::getNowTeamCountForRoom(UINT uroomid)
	{
		for (auto iter = m_room_node.begin(); iter != m_room_node.end(); ++iter)
		{
			if ((*iter).second.room_node.roomid == uroomid)
			{
				return (*iter).second.uTeamCount;
			}
		}

		return 0;
	}

	//增加分配队伍
	void CGameList::AddTeamCount(UINT uroomid, UINT AddCount)
	{
		auto iter = m_room_node.find(uroomid);
		if (iter != m_room_node.end())
		{
			(*iter).second.uTeamCount += AddCount;
		}
	}

	//减少分配队伍
	void CGameList::ReduceTeamCount(UINT uroomid, UINT ReduceCount)
	{
		auto iter = m_room_node.find(uroomid);
		if (iter != m_room_node.end())
		{
			(*iter).second.uTeamCount -= ReduceCount;
		}
	}
}
