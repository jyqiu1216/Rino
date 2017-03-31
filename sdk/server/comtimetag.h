#ifndef WJ_COMTIMETAG_H
#define WJ_COMTIMETAG_H

#define TIME_SPACE						50L								///游戏内部定时器最大不得超过的值
#define TIME_START_ID					100L							///游戏定时器开始 ID
///////登陆服务器///////////////////////////////////////////////////////////////////////////////


enum E_P_TIMER_ID
{
	E_P_TIME_ID_TEAM_VS=1,//为团队进行匹配
	E_P_TIME_ID_UPDATE_ONLINE_COUNT,//在线人数更新
	E_P_TIME_ID_HEART,//心跳检测
	E_P_TIME_ID_UPDATE_USER_INFO,//询问redis刷新用户数据
};

///////游戏服务器///////////////////////////////////////////////////////////////////////////////

enum E_R_TIMER_ID
{
	E_R_TIME_ID_ReconnectLoginServe = 1,
	E_R_TIME_ID_LoginServerKeepalive,
	E_R_TIME_ID_RoomQueue, // 排队机计时器id
	E_R_TIME_ID_HEART,//心跳检测
};

/*
游戏内桌子上的计时器ID构造格式如下
高位                         低位
|--roomid--|--deskid--|-游戏系统计时器id-|-游戏计时器id-|
其中：
  roomid			12位
  deskid			12位
  游戏系统计时器id  4位
  游戏计时器id		4位
*/

#define TIMER_ROOMID_MASK   0xfff00000         //房间ID掩码
#define TIMER_DESKID_MASK   0x000fff00         //桌子ID掩码
#define TIMER_GAME_TIMERID_MASK_SYS     0x000000f0         //游戏系统计时器ID掩码
#define TIMER_GAME_TIMERID_MASK			0x0000000f         //游戏计时器ID掩码



#endif//WJ_COMTIMETAG_H