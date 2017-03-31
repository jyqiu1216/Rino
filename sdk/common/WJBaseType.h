#ifndef __WJ_BaseType_H__
#define __WJ_BaseType_H__

#include <string>

typedef		void*				HANDLE;
//typedef		void*				HINSTANCE;

typedef		char				CHAR;
typedef		unsigned char       BYTE;

typedef		unsigned short      WORD;

typedef		short               SHORT;

typedef		int					INT;
typedef		unsigned int		UINT;

typedef		long long			LLONG;
typedef		unsigned __int64	ULONGLONG;
typedef		unsigned long		ULONG;

typedef		double              DOUBLE;
typedef		float               FLOAT;

#define		INVALID_VALUE			UINT(-1)

#define		INVALID_DESKNO			BYTE(-1)
#define		INVALID_DESKSTATION		BYTE(-1)
#define		INVALID_USER_ID         INT(-1)

// 常量
#define NAME_LENGTH				33	// 呢称、用户名、真实姓名 实际保存的数组长度
#define PASSWORD_LENGTH			21	//密码未加密长度
#define PASSWORD_MD5_LENGTH		65	//MD5 加密过后的用户密码长度
#define GAMENAME_LENGTH				33	//游戏名称长度
#define EREANAME_LENGTH				33	//区服名称长度
#define CONTESTNAME_LENGTH			33	//比赛名称长度
#define ROOMNAME_LENGTH				33	//房间名称长度
#define PHONE_LENGTH				12	//电话号码长度
#define IDCARD_LENGTH				19	//身份证号码长度
#define TEAM_PASS_LENGTH			7	//团队暗号长度
#define TEAM_USER_NUM				10  //队伍最大人数
#define USER_ICON_LENGTH			300	//人物头像数据长度

#define CHAT_LENDTH		600	//聊天字符长度
#define ERROR_SOCKETID	999999		//错误的套接字id
#define ERROR_DESKID	999999		//错误的桌子号
#define ERROR_DESKSTATION	999999	//错误的座位号
#define ERROR_TEAMID		999999	//错误的团队id
#define ERROR_ROOMID        999999  //错误的房间id
#define ERROR_CONTESTID     0		//错误的比赛id

#define AI_NAME_TAG		"AI"		//机器人username开头的关键字
#define AI_PASS_TAG		"ai123456"	//机器人password
#define AI_NUM			2000		//机器人数目
//服务端底层dll宏
#ifdef WJSERVERMANAGELIB_EXPORTS
#define WJSERVERMANAGELIB_API //__declspec(dllexport)
#else
#define WJSERVERMANAGELIB_API //__declspec(dllimport)
#endif

//服务端游戏dll宏
#ifdef WJSERVERGAMELIB_EXPORTS
#define WJSERVERGAMELIB_API //__declspec(dllexport)
#else
#define WJSERVERGAMELIB_API //__declspec(dllimport)
#endif

//客户端通信底层dll宏
#ifdef WJCLIENTSOCKETLOGIC_LIB
# define WJCLIENTSOCKETLOGIC_EXPORT __declspec(dllexport)
#else
# define WJCLIENTSOCKETLOGIC_EXPORT __declspec(dllimport)
#endif

#endif	//__WJ_BaseType_H__
