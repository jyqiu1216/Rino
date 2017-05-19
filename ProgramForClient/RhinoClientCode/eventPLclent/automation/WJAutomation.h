// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the AUTORUN_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// DLL_EXPORT functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifndef _WJ_AUTORUN_H_
#define _WJ_AUTORUN_H_


#ifndef DLL_EXPORT
#define DLL_EXPORT __declspec(dllexport)
#endif

#define STR_LEN_64 64
#define STR_LEN_1024 1024


///*登录结果回调函数*/
//typedef void(*PFLOGINRST)(const char* account, int resultcode);
///*创建房间结果回调函数*/
//typedef void(*PFCREATEROOMRST)(const char* roomname, const char* account, int resultcode);
///*加入房间结果回调函数*/
//typedef void(*PFJOINROOMRST)(const char* roomname, const char* account, int resultcode);



extern const char* WINDOW_TITLE_LOL_LOGIN;
extern const char* WINDOW_TITLE_LOL_CLIENT;
extern const char* WINDOW_TITLE_LOL_GAME;
extern const char* WINDOW_TITLE_HB_LOGIN;
extern const char* WINDOW_TITLE_HB_CLIENT;
extern const char* WINDOW_TITLE_HB_BATTLENET;
extern const char* WINDOW_TITLE_HB_FRIENDS;


#define WJAM_RES_OK  0							/* 执行成功 */

#define WJAM_RES_D3D_ERROE		1				/* 执行成功 */
#define WJAM_RES_HOOK_TIMEOUT	2


#define WJAM_RES_TIMEOUT  100					/* 操作超时 */
#define WJAM_RES_TIMEOUT_LOL_LOGIN  101

#define WJAM_RES_UNKNOWN_GAME  110				/* 未知的游戏 */
#define WJAM_RES_WINDOW_NOT_EXIST  111			/* 窗口不存在 */
#define WJAM_RES_CLOSE_PROCESS_FAILED  112		/* 关闭游戏失败 */

#define WJAM_RES_LUA_NOT_INIT  501				/* LUA 未初始化 */
#define WJAM_RES_LUA_PARAM_ERROR  502			/* LUA参数错误 */
#define WJAM_RES_LUA_EXCE_ERROR  2				/* LUA 执行异常 */

#define WJAM_RES_LOL_EXIST_IN_GAME  	1001	/* LOL 正在游戏中 */
#define WJAM_RES_LOL_EXIST_IN_SERVER  	1002	/* LOL 已经登陆选择服务区，但未进入游戏 */
#define WJAM_RES_LOL_SELECT_SERVER_FAILED 1003	/* LOL 选择服务器失败 */
#define WJAM_RES_LOL_ROOM_EXIST			1004	/* 房间已经存在 */
#define WJAM_RES_LOL_LOGIN_FROM_DISCONNECTION	1005	/* 游戏处于断线重连状态 */

#define WJAM_RES_NEW_PLAYER				1006	/* 该用户在当前服务器是新手 */



#define WJAM_RES_HS_OPERATE_ERROR		2001

#include <string>

/**
* \date:	2016/08/22
* \brief:	自动化回调函数
* \param:	无
* \return:	[int _res] 0:成功;  _res!=0: 异常
* \usage:
*/
typedef int(*PFuncCallBack)(int _res);
//typedef void(*PFuncCallbackStatusChanged)();

//#include "GameMonitorHS.h"
enum EnGameStatusHS;
class Callback_KnowHeroLS;

DLL_EXPORT class AMCallback
{
public:
	virtual void callbackStatusChandedHS(const EnGameStatusHS& _status) 
	{
		printf(" =====> 炉石状态改变：%d", _status);
	};
};

namespace WJAM
{
	/*游戏*/
	enum GAME_ID
	{
		GAME_ID_LOL = 1,    /*英雄联盟*/
		GAME_ID_HB,			/*炉石传说*/
	};

	/*窗口ID*/
	enum WIND_ID
	{
		WIND_ID_LOL_LOGIN = 1,		/*英雄联盟登陆窗口*/
		WIND_ID_LOL_CLIENT,			/*英雄联盟大厅窗口*/
		WIND_ID_HB_LOGIN,			/*炉石传说登陆窗口*/
		WIND_ID_HB_CLIENT,			/*炉石传说客户端窗口*/
		WIND_ID_HB_BATTLENET,		/*炉石传说战网窗口*/
		WIND_ID_HB_FRIENDS,			/*炉石传说好友窗口*/
	};

	/*游戏模式*/
	enum GAME_MODE
	{
		GAME_MODE_CLASSIC = 1,		/*经典模式*/
		GAME_MODE_BATTLE,			/*枢纽攻防战*/
	};

	/*地图*/
	enum GAME_MAP
	{
		GAME_MAP_RIFT = 1,			/*召唤师峡谷*/
		GAME_MAP_FOREST,			/*扭曲丛林*/
		GAME_MAP_ABYSS,				/*嚎哭深渊*/
	};

	/*观战模式*/
	enum GAME_OBSERVE
	{
		GAME_OBSERVE_ALL = 1,		/*所有人*/
		GAME_OBSERVE_NONE,			/*不允许*/
		GAME_OBSERVE_HALL,			/*同大厅玩家*/
		GAME_OBSERVE_FRIENDS,		/*好友*/
	};

	/*游戏类型*/
	enum GAME_TYPE
	{
		GAME_TYPE_SELFCHOOSE = 1,   /*自选*/
		GAME_TYPE_DRAFT,			/*征召*/
		GAME_TYPE_RANDOM,			/*随机*/
	};

	/*玩家阵营*/
	enum GAME_SIDE
	{
		GAME_SIDE_RED = 1,			/*红方*/
		GAME_SIDE_BLUE,				/*蓝方*/
	};

	/*登录信息*/
	typedef struct LOGININFO
	{
		int gameid;					/*游戏*/
		char account[STR_LEN_64];   /*账号*/
		char password[STR_LEN_64];  /*密码*/
		int serverid;				/*服务器ID*/
		char path[STR_LEN_1024];    /*可执行文件路径*/
	};

	/*房间信息*/
	typedef struct ROOMINFO
	{
		char roomname[STR_LEN_64];  /*房间名称*/
		char password[STR_LEN_64];  /*房间密码*/
		char account[STR_LEN_64];	/*账号*/
		int mode;					/*游戏模式*/
		int map;					/*地图*/
		int playernum;				/*队伍人数*/
		int observe;				/*观战模式*/
		int gametype;				/*游戏类型*/
		int side;					/*玩家阵营*/
	};


	/* 初始化脚本 */
	DLL_EXPORT bool initLua(const char *_luaFile, AMCallback* _callback=nullptr);
	
	/* 登录接口 */
	DLL_EXPORT int login(LOGININFO* _loginInfo);
	
	/* 创建房间接口 */
	DLL_EXPORT int createRoom(ROOMINFO* _roomInfo);
	
	/* 加入房间接口 */
	DLL_EXPORT int joinRoom(ROOMINFO* _roomInfo);
	
	/**
	* \brief:	开始游戏
	* \param:	[_gameId]	游戏类型id
	* \return:	[_isRoomOwner]	true: 房主; false:非房主;
	*/
	DLL_EXPORT int startGame(WJAM::GAME_ID _gameId, bool _isRoomOwner);
	
	/**
	* \brief:	关闭游戏
	* \param:	[_path]	游戏路径
	* \return:	[int]	0: 关闭成功; !=0:
	*/
	DLL_EXPORT int closeGame(GAME_ID _gameId);
	
	/**
	* \brief:	隐藏或显示游戏的窗口
	* \param:	[_gameId]	游戏类型id
	* \param:	[_visible]	true: 显示;	false:	隐藏;
	* \return:	void
	*/
	DLL_EXPORT void setGameWindowVisible(WJAM::GAME_ID _gameId, bool _visible);
	
	/*
	* \brief:	炉石自动化操作初始化
	* \param:	[_wsPort]: webSocket 服务端口
	* \param:	[_url]: 内嵌H5的URL
	* \param:	[_userName]: 返回参数：玩家名称
	*/
	DLL_EXPORT int startupHS(int _wsPort, const char* _url, std::string& _playerName);
	
	
	//DLL_EXPORT int playHS(const char *_player);
	
	
	/**
	* \brief:	请求加好友
	* \param:	[_name]: 好友名称
	*/
	DLL_EXPORT int reqAddFriend(const char *_name);
	
	/**
	* \brief:	接受好友添加请求
	* \param:	[_name]: 好友名称
	*/
	DLL_EXPORT int acceptFriendReq(const char *_name);
	
	/**
	* \brief:	发起挑战请求
	* \param:	[_name]:	挑战的好友的名称
	*/
	DLL_EXPORT int reqChallenge(const char *_name);
	
	/**
	* \brief:	接受挑战
	*/
	DLL_EXPORT int acceptChallenge();
	
	/**
	* \brief:	获取炉石传说的游戏状态
	*/
	DLL_EXPORT int getGameStatusHS();
	DLL_EXPORT void setGameStatusHS(const EnGameStatusHS& _status);

	DLL_EXPORT int takeEvent();

	//初始化炉石识别英雄
	DLL_EXPORT int InitKnowHeroLS(std::string &modelFile, std::string &deployFile, std::string &meanFile, std::string &labelFile);

	//炉石识别英雄
	DLL_EXPORT void SetKnowHeroLSCallBack(Callback_KnowHeroLS* pCallback);
}

#endif // !_WJ_AUTORUN_H_