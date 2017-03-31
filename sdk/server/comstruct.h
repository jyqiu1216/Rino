
#ifndef _S_COM_STRUCT_HEAD_FILE
#define _S_COM_STRUCT_HEAD_FILE
#include "common/WJBaseType.h"
namespace WJ
{
	enum ENUM_chip_change_type//与redis约定娱币修改的原因
	{
		e_chip_change_type_apply=3,//报名消耗
		e_chip_change_type_apply_restitution=8,//报名退费
		e_chip_change_type_contest_finish_win = 7,//比赛获胜
	};

	enum ENUM_coin_change_type//与redis约定娱币修改的原因
	{
		e_coin_change_type_apply=2,//报名消耗
		e_coin_change_type_apply_restitution=10,//报名退费
		e_coin_change_type_contest_finish_win=9,//比赛获胜
	};

	enum ENUM_ContestKind//比赛场次类型（互斥）
	{
		US_ContestKind_free = 1,//免费场
		US_ContestRule_point,//积分场
		US_ContestRule_happy, //娱币场
	};

	static char g_char_ENUM_ContestKind[3][60] = {"免费场", "积分场", "娱币场"};

	enum ENUM_ContestRule//比赛模式（互斥）
	{
		US_ContestRule_mission = 0,//任务赛
		US_ContestRule_solo = 1,//个人挑战赛
		US_ContestRule_team,//团队挑战赛
	};

	enum ENUM_AwardType//奖励类型（互斥）
	{
		ENUM_AwardType_point = 1,//奖励积分
		ENUM_AwardType_happy,//奖励娱币
	};
	
	static char g_char_ENUM_ContestRule[2][60] = { "个人挑战赛", "团队挑战赛" };

	enum ENUM_RoomRule//房间规则（互斥）
	{
		US_RoomRule_alone = 0,//单机
		US_RoomRule_1v1,//1v1
		US_RoomRule_2v2,//2v2
		US_RoomRule_3v3,//3v3
		US_RoomRule_4v4,//4v4
		US_RoomRule_5v5,//5v5
		US_RoomRule_6v6,//6v6
		US_RoomRule_7v7,//7v7
		US_RoomRule_8v8,//8v8
		US_RoomRule_9v9,//9v9
		US_RoomRule_10v10,//10v10
	};

	enum ENUM_TeamPayType//团队付费类型
	{
		TeamPayType_Captain = 0,//队长支付
		TeamPayType_AA,//AA制
	};

	//团队节点
	struct TeamStruct
	{
		char m_Team_password[TEAM_PASS_LENGTH];//团队暗号
		UINT m_uCaptain_userid;//队长
		UINT m_uTemaPlayerNum;//队伍人数
		UINT m_Userid[TEAM_USER_NUM];//每个人的userid
		ENUM_TeamPayType m_TeamPayType;//支付方式
		UINT m_uContest;//比赛id
		bool bAllot;//是否被分配到房间去了
		UINT uroomid;//被分配的房间id
		bool bVS;//是否被匹配
		bool bButVS;//队长启动了匹配按钮
		TeamStruct()
		{
			std::memset(this, 0, sizeof(TeamStruct));
		}
	};

	//团队VS信息
	struct TeamStruct_VS
	{
		UINT teamid; //队伍id
		UINT uroomid;//被分配的房间id
		TeamStruct _TeamStruct[2];

		TeamStruct_VS()
		{
			teamid = 0;
			uroomid = 0;
		}
	};
	//大厅，房间服务端所用数据基类
	struct ManageInfoStruct
	{
	public:
		bool bRun_SocketLib;//是否启动套接字组件
		bool bRun_DataSQLLib;//是否启动数据库组件
		bool bRun_Redis;//Redis服务组件

		bool bAutoPort;//是否由系统自动分配端口号
		UINT uMaxCount;//最大连接数
		UINT uListenPort;//端口号

		char uRedis_ip[20];//redis服务器端ip
		UINT uRedis_port;//redis服务器端口号
		char uRedis_Pass[20];//登陆redis密码
	public:
		ManageInfoStruct()
		{
			std::memset(this, 0, sizeof(ManageInfoStruct));
		}
	};

	//房间类特有数据结构体
	struct Room_ManageInfoStruct
	{
		UINT uRoomID;//房间id
		char cRoomName[ROOMNAME_LENGTH];//房间名
		ENUM_RoomRule uRoomRule;//房间规则
		UINT uGameID;//游戏id
		UINT ereaid;//游戏区服id
		UINT uDeskCount;//游戏桌数目
		UINT uDeskPeople;//一桌最大游戏人数
		UINT uContestid;//比赛id
		char cContestName[CONTESTNAME_LENGTH];//比赛名
		ENUM_ContestRule uContestRule;//比赛模式
		ENUM_ContestKind uContestKind;//比赛场次类型
		LLONG _i64cost;//报名费用
		ENUM_AwardType awardType;//奖励类型
		UINT award;//奖励
		HANDLE hDllInstance;//组件句柄

		Room_ManageInfoStruct()
		{
			std::memset(this, 0, sizeof(Room_ManageInfoStruct));
		}
	};

	//大厅类特有数据结构体
	struct Hall_ManageInfoStruct
	{
		Hall_ManageInfoStruct()
		{
			std::memset(this, 0, sizeof(Hall_ManageInfoStruct));
		}
	};
}

#endif //_S_COM_STRUCT_HEAD_FILE


