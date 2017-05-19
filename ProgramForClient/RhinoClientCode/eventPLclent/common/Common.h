#ifndef _COMMON_H_
#define _COMMON_H_

#include <QtWebEngineWidgets\qwebengineview.h>
#include <QtWidgets\QMainWindow>
#include <QtWebEngineWidgets\qwebengineview.h>
#include <QtWebChannel\qwebchannel.h>
#include <QMessageBox>
#include <QWidget>
#include <QtGui>
#include <qpropertyanimation.h>
#include <qdir.h>
#include <QLatin1String> 
#include <QFile>
#include <QtCore>
#include <QtWebEngineWidgets/QtWebEngineWidgets>
#include <QtWebChannel/QtWebChannel>

#include <unordered_map>
#include <functional>
#include "webpopup.h"
#include "webpopuppag.h"
#include "common/Model.h"
#include "QMbox.h"
#include "bridge.h"
#include "ClientLink.h"
#include "common/clientconfig.h"
#include "ClientLink.h"
#include "message/WJNetPlatformMessage.h"
//#include "WJGameInfo.h"


using namespace std;

QString GBK2UTF8(const QString &inStr);
QString UTF82GBK(const QString &inStr);
std::string UnicodeToUTF8(const wchar_t* str);

DWORD FindProcess(char *strProcessName);
VOID KillProcess(char *strProcessName);
bool ProcessIsLOL(DWORD pid);
DWORD GetProcessIdByName(LPCTSTR pName, bool bCheck = false);
BOOL GetProcessModule(DWORD dwPID, wchar_t* dll);

class IBaseMyDataDrocessing;
class IBaseMyHallLogic;
class IBaseMyGameRoom;

enum E_GameID;
enum class MatchType;

class CCommon
{
public:
	//确认可用变量
	E_GameID m_NowGame;		//当前选中的游戏id
	UINT m_NowContest;		//当前选中的比赛id
	UINT m_NowRoom;			//当前选中的房间id
	UINT m_NowErea;			//当前选中的区服id

	std::vector<WJ::ComRoomInfo> m_GameRoomList;	//当前选中比赛内房间列表
	HALLDATA m_NowContestInfo;						//当前比赛节点数据（solo赛或者团队赛）
	QString lolroute;			//lol路径
	QString HearthStoneroute;	//炉石传说路径
	USERDATA userinfo;			//用户信息
	TVBETINFO m_BetInfo;		//LOL押注数据
	TVBETINFO m_BetHsInfo;		//炉石押注数据

	std::vector<WJ::ComGameinfo> m_GameNameList;		//游戏名列表
	std::vector<WJ::ComContestInfo> m_GameContestList;	//比赛列表
	std::vector<WJ::ComEreainfo> m_AreaList;			//游戏区服列表

	//待定变量
	
	GAMEINFORMATION GameInformation;	//桌子数据
	QMAHALLDATA HallData;				//大厅数据存储列表
	
	//GAMECONT Eventdetails;//赛事详细信息
	
	MatchType match;
	QString Resultspagecontent;		//发送结果页内容
	bool bExecutionAutomationDoing;	//是否在执行自动化
	QString GameContname;			//判断个人赛或者团队赛
	QString webstring;				//网页地址
	QString version;				//版本号
	QString GameName;
	QString strweb;					//判断打开的是什么页面
	QString currentpath;			//当前程序路径
	QString qquinNUME;				//qquinNUME
	QString qquinID;				//从java服务器取得类似 U5912975290486760802
	QList<int> presongamelist;		//个人赛list
	QList<int> teamgamelist;		//团队赛list
	QString userAccount;			//保存用户账户信息

	QString Logpath;				//log日志路径
	//结果数据缓存用于弹出右下角押注结果
	QJsonObject m_BetCompleter;

	//函数区域
public:
	//弹出框
	//title:弹窗内容；okButton：确认按钮内容；noButton：取消按钮内容；butNub是否有取消按钮2（有）1（没有）
	//返回：true:确认   false取消
	bool BoxMess(QWidget *widget, QString title, QString okButton = QStringLiteral("确认"), QString noButton = QStringLiteral("取消"), int butNub = 1);

	///保存大厅信息
	void SetHallData(QString str);
	void GetAllGameRoute();	//获取所有游戏文件路径
	bool GetLolRoute();		//判断lol路径是否已经存在
	void SetGameRoute();	//写入游戏文件地址

	////监测版本是否可以更新
	//bool VersionUpdate();
	////启动update.exe
	///*
	//std::string version 主版本
	//std::string smallversion 子版本
	//std::string _filename 下载的文件
	//std::string strDownLoadUpdatepADDR 下载的根地址
	//std::string startgamename 启动的程序名称
	//*/
	//bool updateOnline(std::string version, std::string smallversion, std::string _filename, std::string strDownLoadUpdatepADDR, std::string startgamename);
	//获得当前比赛详情
	void GetNowContestInfo();
public:
	static CCommon* getInstance();

	//注册游戏
	void RegisterGameList();

	IBaseMyDataDrocessing* GetMyDataDrocessing(E_GameID igameid);
	IBaseMyHallLogic* GetMyHallLogic(E_GameID igameid);
	IBaseMyGameRoom* GetMyGameRoom(E_GameID igameid);
	//获得
private:
	// 构造函数是私有的
	CCommon(void);

	// 析构函数
	virtual ~CCommon(void);

private:
	//类拷贝是私有的
	CCommon(const CCommon &);
	CCommon & operator = (const CCommon &);
};


#define  CommonData()	CCommon::getInstance()

//弹出框安全函数封装
#include <qwidget.h>
#define WJ_SAFE_QBoxMess(_p, _title) \
if (nullptr != _p)\
	CommonData()->BoxMess((QWidget*)_p, _title, QStringLiteral("确定"), "", 1);

#endif