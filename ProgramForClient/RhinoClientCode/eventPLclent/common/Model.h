#ifndef MODEL_H
#define MODEL_H

#define STR_LEN_64	64//字符长度
#define PLAYER_COUNT	10//游戏人数

#include <QMap>
#include <QList>
#include <vector>
#include <set>
using namespace std;


typedef struct UserData//用户数据
{
	QString szName;//登录名
	QString	nickName;//用户昵称
	QString	password;//密码
	int dwUserID;//ID
	int dwPoint;//积分
	long i64Money;//娱币
	QString	bUserState;//用户状态
	QString	landmassage = "";//登陆消息
	QString icon;//头像
	QString rewardTotal;//用户总的奖励
}USERDATA, *PUSERDATA;
typedef std::vector<USERDATA> QMAPUSERDATA;

//押注数据
typedef struct _BetInfo {

	//显示位置
	int				_sinIndex;
	//游戏ID
	int				_sinGameId;
	//金额
	int				_sinMoney;
	//倍率
	float			_sfRate;

	bool operator < (const _BetInfo& Value) const {

		return Value._sinIndex < _sinIndex;
	}

	_BetInfo() {

		_sinIndex = 0;
		_sinGameId = 0;
		_sinMoney = 0;
		_sfRate = 0;
	}

} BetInfo, *pBetInfo;
typedef set<BetInfo> TVBETINFO;

typedef struct LOLautomation//lol自动化
{
	char roomname[128];		/*房间名称*/
	char password[50];		/*房间密码*/
	int mode;				/*游戏模式*/
	int map;				/*地图*/
	int playernum;			/*队伍人数*/
	int observe;			/*观战模式*/
	int gametype;			/*游戏类型*/
	bool bCreate;			//创建第三方房间
	int ServiceArea;		//区服ID
	int side;				//玩家阵营
	LOLautomation()
	{
		std::memset(this, 0, sizeof(LOLautomation));
	}
}LOLAUTOMATION, *PLOLAUTOMATION;



typedef struct GameInformation//游戏信息
{
	int GameState;
}GAMEINFORMATION, *PGAMEINFORMATION;



typedef struct HallData//大厅数据
{
	int id;//比赛ID
	int sort_no;//` int(10) DEFAULT NULL COMMENT '赛事排序',
	QString name;//` varchar(20) COLLATE utf8_unicode_ci DEFAULT NULL COMMENT '赛事名称',
	int manage_id;//` bigint(20) DEFAULT NULL COMMENT '归属管理模块',
	int game_id;//` bigint(20) DEFAULT NULL COMMENT '游戏ID',
	int server_id;//` bigint(20) DEFAULT NULL COMMENT '区服ID',
	int type;//` tinyint(4) DEFAULT NULL COMMENT '赛事类别 0 免费 1 积分 2 娱币 ',
	int point_type;//` tinyint(4) DEFAULT NULL COMMENT '赛点形式 \r\n个人SOLO\r\n率先推掉敌方一塔的个人获胜\r\n补刀先达到100刀的个人获胜\r\n10分钟以内获得金钱最多的个人获胜\r\n\r\n组队\r\n率先获得第一条大龙的队伍获胜\r\n率先推掉对方基地的队伍获胜\r\n率先推掉敌方4座防御塔的队伍获胜\r\n队伍中击杀英雄总数率先达到15个的队伍获胜',
	QString pointName;
	int theme;//` tinyint(4) DEFAULT NULL COMMENT '赛事主题   1 初级场 2进阶场 3 高级场 4 大师场',
	QString icon;//` varchar(128) COLLATE utf8_unicode_ci DEFAULT NULL,
	int criteria;//` int(11) DEFAULT NULL COMMENT '参赛条件 0 免费 其他值对应消耗积分或娱币金额',
	int mode;//` tinyint(4) DEFAULT NULL COMMENT '赛事模式 1 个人solo 2 团队',
	int begin_time;//` datetime DEFAULT NULL COMMENT '比赛开始时间',
	int end_time;//` datetime DEFAULT NULL COMMENT '比赛结束时间',
	QString specialty;//` varchar(256) COLLATE utf8_unicode_ci DEFAULT NULL COMMENT '赛事特点 如：solo一血',
	int max_number;//` int(11) DEFAULT NULL COMMENT '最大参数人数',
	int team_number;//` tinyint(4) DEFAULT NULL COMMENT '团队人数',
	QString prize;//` varchar(20) COLLATE utf8_unicode_ci DEFAULT NULL COMMENT '最高奖励',
	QString comment;//` varchar(512) COLLATE utf8_unicode_ci DEFAULT NULL COMMENT '赛事说明',
	QString prize_info;//` varchar(512) COLLATE utf8_unicode_ci DEFAULT NULL COMMENT '奖励设置',
	QString mode_info;//` varchar(512) COLLATE utf8_unicode_ci DEFAULT NULL COMMENT '赛点说明',
	int is_valid;//` tinyint(4) DEFAULT NULL,
	int exec_user;//` bigint(20) DEFAULT NULL COMMENT '赛事联络人',
	QString create_date;//` datetime DEFAULT NULL COMMENT '录入日期',
	QString update_date;//` datetime DEFAULT NULL COMMENT '最新修改日期',
	QString open_status;//` char(10) COLLATE utf8_unicode_ci DEFAULT NULL COMMENT ' 启动状态:0-编辑,1-进行中,2-已结束',
	int profit;//` int(8) DEFAULT NULL COMMENT '比赛收益',
	int profitType;
	//QString prize;
}HALLDATA, *PHALLDATA;
typedef QMap<int, HALLDATA> QMAHALLDATA;


#endif

