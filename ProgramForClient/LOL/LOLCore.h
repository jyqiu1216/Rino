#pragma once
//电信
#define AiOuNiYa			0x101		//艾欧尼亚
#define ZhuAng				0x201		//祖安
#define NuoKeShaShi			0x301		//诺克萨斯
#define BanDeErCheng		0x401		//班德尔城
#define PiErTeWoFu			0x501		//皮尔特沃夫
#define ZhangZhengXueYuan	0x601		//战争学院
#define JuShengFeng			0x701		//巨神峰
#define LeiSheShouBei		0x801		//雷瑟守备
#define GangTieLieYang		0xC01		//钢铁烈阳
#define ChaiJueZhiDi		0x901		//裁决之地
#define HeiSheMeiZhui		0xA01		//黑色玫瑰
#define AngYingDao			0xB01		//暗影岛
#define JunHengJiaoPai		0xE01		//均衡教派
#define ShuiJinZhiHeng		0xD01		//水晶之痕
#define YinLiu				0xF01		//影流
#define ShouWanZhiHai		0x1001		//守望之海
#define ZhengFuZhiHai		0x1101		//征服之海
#define KaLaMangDa			0x1201		//卡拉曼达
#define PiChengJinBei		0x1301		//皮城警备
#define Nanjuelingyu		0x105		//男爵领域
//网通
#define BiErJiWoTe			0x102		//比尔吉沃特
#define DeMaXiYa			0x202		//德玛西亚
#define FeiLeiErZhuoDe		0x302		//弗雷尔卓德
#define WuWeiXianFeng		0x402		//无畏先锋
#define ShuRuiMa			0x502		//恕瑞玛
#define NiuQuChongLin		0x602		//扭曲丛林
#define JuLongZhiChao		0x702		//巨龙之巢
//其他
#define JiaoYuWangZhuanQu	0x10003		//教育网专区


#define PROCESSNAME L"CrossProxy.exe"
#include <windows.h>

#define MSG_LOLINFO_HALL					(0x1001)	//大厅
#define MSG_LOLINFO_CLIENT					(0x1002)	//League of Legends.exe

struct TGameInfo
{
	DWORD dwUin;			//QQ号
	wchar_t area[100];		//区服	
	wchar_t	userName[100];	//用户名
	TGameInfo()
	{
		memset(this, 0, sizeof(TGameInfo));
	}
};