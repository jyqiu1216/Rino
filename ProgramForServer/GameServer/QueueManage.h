#ifndef WJ_QueueManage_H
#define WJ_QueueManage_H

#include <list>
#include <map>
#include <mutex>
#include <unordered_map>
#include "common/WJBaseType.h"
#include "UserInfo.h"
#include "server/comstruct.h"

namespace WJ
{
	enum E_R_TeamStruct_VS_station
	{
		E_R_TeamStruct_VS_station_wait=0,//等待被处理状态
		E_R_TeamStruct_VS_station_playing,//已经开始分配房间玩了
		E_R_TeamStruct_VS_station_end,//已经结束了
	};
	struct R_TeamStruct_VS//房间内团队VS数据
	{
		TeamStruct_VS _data;//数据
		E_R_TeamStruct_VS_station _E_R_TeamStruct_VS_station;//状态

		R_TeamStruct_VS()
		{
			_E_R_TeamStruct_VS_station = E_R_TeamStruct_VS_station_wait;
		}
	};

	//该结构体记录匹配玩家记录
	struct RememberVS_struct
	{
		UINT useridA;//solo赛玩家
		UINT useridB;//与之匹配过的玩家

		long int ltime;//
	};

	//玩家列表随机排队
	class QueueInfo_t
	{
	public:
		QueueInfo_t()
		{
			m_dwUserID = 0;
			m_EnterTime = 0;
		}

		bool operator < (const QueueInfo_t& ti) const
		{//按加入队列时间长短排序
			return m_EnterTime < ti.m_EnterTime;
		}


		UINT m_dwUserID;//ID
		UINT m_uContestid;//比赛id
		__time64_t m_EnterTime;//加入队列的时间点
	};

	class CQueueManage
	{
	public:
		UINT m_ucontestid;//该队列属于某比赛id下
		ENUM_ContestRule m_uContestRule;//比赛模式
	private:
		//存在多个比赛就应该存在多个相应的用户排队队列
		std::list<QueueInfo_t>	m_QueueUserList;	//等待搓桌，正在排队的玩家队列
        std::unordered_map<UINT, std::list<QueueInfo_t>::iterator >          m_userid2iter;     //等待搓桌的玩家ID到队列中的存放位置的映射
		//匹配记忆
		std::map<UINT, RememberVS_struct> m_RememberVS_map;
	public:
		CQueueManage();
		~CQueueManage();

		void run();
		//添加玩家信息到队列中
		bool Add(CGameUserInfo* pGameUserInfo);
		//删除队列中指定玩家信息
		void Delete(CGameUserInfo* pGameUserInfo);
		//删除队列中所有玩家信息
		void DeleteAll();

	private:
		//个人solo模式运行程序
		void run_RoomRule_solo();
		//组队模式模式运行程序
		void run_RoomRule_team();
	};
}

#endif//WJ_QueueManage_H