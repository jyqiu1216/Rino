#ifndef __WJ_UserInfoBaseStruct_H__
#define __WJ_UserInfoBaseStruct_H__

#include "WJBaseType.h"
namespace WJ
{
	enum ENUM_UserStation
	{//用户状态
		US_P_Wait = 0,	//大厅空闲状态
		US_P_Applying,	//报名中状态
		US_P_unApplying,	//取消报名中状态
		US_P_CreateTeaming,//创建团队中状态
		US_P_AddTeaming,//加入团队中状态
		US_P_DissolveTeaming,//解散团队中状态
		US_P_LeaveTeaming,//离开团队中状态
		US_P_TeamVSing,//团队VS中状态


		US_R_Wait,	//房间空闲状态
		US_R_GetStation,//获得座位状态
		US_R_Agress,  //就绪状态
		US_R_Playing, //游戏中状态
		US_R_NetCut,  //断线状态
	};

	///用户信息结构
	struct UserInfoBaseStruct
	{
	public:
		char				szName[NAME_LENGTH];			//登录名
		char				nickName[NAME_LENGTH];			//用户昵称
		char				password[PASSWORD_MD5_LENGTH];	//密码
		UINT				dwUserID;						//ID
		UINT				dwPoint;						//积分
		LLONG				i64Money;						//娱币
		ENUM_UserStation	bUserState;						//用户状态
		UINT				teamid;							//团队id
		bool				bRobot;							//机器人
		char				icon[USER_ICON_LENGTH];			//人物头像信息
		UserInfoBaseStruct()
		{
			reset();
		}

		void reset()
		{
			std::memset(this, 0, sizeof(UserInfoBaseStruct));
			teamid = ERROR_TEAMID;
		}
	};
	//大厅用户节点
	struct UserInfoInHallStruct
	{
		UserInfoBaseStruct basedata;//基础数据
		char				telephone[PHONE_LENGTH];		//电话号码
		char				IDCard[IDCARD_LENGTH];			//身份证号码
		
		UserInfoInHallStruct()
		{
			reset();
		}

		void reset()
		{
			basedata.reset();
			basedata.bUserState = US_P_Wait;
		}
	};
	//房间用户节点
	struct UserInfoInRoomStruct
	{
		UserInfoBaseStruct basedata;//基础数据
		UINT                uRoomid;                        //房间id
		UINT				uDeskid;						//桌子id
		UINT				uDeskStation;					//座位id
		UINT				uContestid;						//比赛id

		UserInfoInRoomStruct()
		{
			reset();
		}

		void reset()
		{
			basedata.reset();
			uRoomid = ERROR_ROOMID;
			uDeskid = ERROR_DESKID;
			uDeskStation = ERROR_DESKSTATION;
			basedata.bUserState = US_R_Wait;
			uContestid = ERROR_CONTESTID;
		}
	};
}


#endif	//__WJ_UserInfoBaseStruct_H__