
#ifndef WJ_gamelistmanage_H
#define WJ_gamelistmanage_H
#include <vector>
#include <map>
#include <mutex>
#include "common/WJBaseType.h"
#include "message/WJNetPM_gamelist.h"
#include "Redis\WJRedis.h"

//游戏列表的模型是 游戏节点下面是比赛节点，比赛节点下面是房间节点

namespace WJ
{
	struct ComRoomInfo_node
	{
		ComRoomInfo room_node;//数据
		UINT usocketindex;//套接字id
		UINT uTeamCount;//该房间当前团队数

		ComRoomInfo_node()
		{
			usocketindex = ERROR_SOCKETID;
			uTeamCount = 0;
		}
	};
	class CGameList
	{
	
	public:
		CGameList();
		~CGameList();

		void InitLoadFromDB(CWJRedis *pRedis);
		//清除所有记录
		void clear();
		//添加游戏节点
		void addgame_node(ComGameinfo &node);
		//添加区服节点
		void adderea_node(ComEreainfo &node);
		//添加比赛节点
		void addcontest_node(ComContestInfo &node);
		//添加房间节点
		void addroom_node(ComRoomInfo_node &node);
		//获取游戏节点数目
		UINT Getgame_nodeCount();
		//获取区服节点数目
		UINT Geterea_nodeCount();
		//获取比赛节点数目
		UINT Getcontest_nodeCount();
		//获取房间节点数目
		UINT Getroom_nodeCount();
		//获取游戏
		UINT FillGameName(char * pBuffer, UINT uBufferSize, UINT uBeginPos, bool & bFinish, UINT &uCopyCount);	
		//获取区服
		UINT FillErea(char * pBuffer, UINT uBufferSize, UINT uGameID, UINT uBeginPos, bool & bFinish, UINT &uCopyCount);
		//获取比赛
		UINT FillGameContest(char * pBuffer, UINT uBufferSize, UINT uEreaID, UINT uBeginPos, bool & bFinish, UINT &uCopyCount);
		//获取房间
		UINT FillGameRoom(char * pBuffer, UINT uBufferSize, UINT uContestID, UINT uBeginPos, bool & bFinish, UINT &uCopyCount);

		//该比赛是否有房间
		bool ExistRoomForContest(UINT uContestid);
		//获得比赛数据
		const ComContestInfo* GetComContestInfo(UINT uContestid);
		//获得房间数据
		const ComRoomInfo* GetComRoomInfo(UINT uroomid);
		//获得房间数据
		const ComRoomInfo* GetComRoomInfoEx(UINT uroomid);

		//获得房间的套接字id
		bool GetRoomSocketID(UINT roomid, UINT &usocketindex);
		//获得房间列表
		void GetComRoomList(std::vector<ComRoomInfo_node> &room_node, UINT uContestid);

		//该房间当前分配有多少比赛队伍
		UINT getNowTeamCountForRoom(UINT uroomid);
		//增加分配队伍
		void AddTeamCount(UINT uroomid, UINT AddCount);
		//减少分配队伍
		void ReduceTeamCount(UINT uroomid, UINT ReduceCount);
	private:
		std::map<long, ComGameinfo> m_game_node;				//游戏节点
		std::map<long, ComEreainfo> m_erea_node;				//区服节点
		std::map<long, ComContestInfo> m_contest_node;			//比赛节点
		std::map<long, ComRoomInfo_node> m_room_node;			//房间节点
	};
}

#endif//WJ_gamelistmanage_H
