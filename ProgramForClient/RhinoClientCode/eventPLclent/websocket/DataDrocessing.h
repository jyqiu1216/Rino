#ifndef DATADROCESSING_H
#define DATADROCESSING_H
#include <QObject>
#include <QWebSocket>
#include "websocket\WSMsgHandler.h"

//这个类是c++接收web客户端发来的消息，并做相应的处理,处理代码被投递到
class DataDrocessing :public WSMsgHandler
{
	Q_OBJECT

public:
	DataDrocessing();
	~DataDrocessing();
	static DataDrocessing* inobgect();
	
	void web_Processing_1001(QJsonObject TYPE);//接受到web登陆消息
	void web_Processing_1004(QJsonObject TYPE);//接受到web请求大厅消息
	void web_Processing_1005(QJsonObject TYPE);//接受到web请求LOL任务赛报名消息
	void web_Processing_1006(QJsonObject TYPE);//接受到web请求任务赛详情数据

	//点击比赛节点获取房间列表交付
	void web_Processing_1013(QJsonObject TYPE);
	//离开比赛详情页面交付
	void web_Processing_1014(QJsonObject TYPE);
	//请求玩家是否处于游戏状态标志
	void web_Processing_1031(QJsonObject TYPE);
	//微信登陆二维码扫描窗口打开交付
	void web_Processing_9001();
	//微信登陆二维码扫描窗口关闭交付
	void web_Processing_9002(QJsonObject TYPE);
	//微信登陆二维码扫描拿到用户信息进行转发交付
	void web_Processing_9003(QJsonObject TYPE);
	//微信登陆二维码扫描通知大厅使用账户登陆交付
	void web_Processing_9004();
	//微信登陆二维码扫描通知大厅使用key+userid登陆业务服务器交付
	void web_Processing_9005(QJsonObject TYPE);
	//设置中修改密码
	void web_Processing_9006(QJsonObject TYPE);
	//告知当前是什么游戏id
	void web_Processing_1027(QJsonObject TYPE);
	//H5索要注册需要的推广id
	void web_Processing_7000();
	//告知当前客户端版本
	void web_Processing_1028();

	//锁住比赛详情页按钮
	void OnContestButLock();
	//解锁比赛详情页按钮
	void OnContestButUnlock();
	//通知h5，路径不对，请求再次设置游戏路径
	void HTTPMessageProcessing_1012();
	//获得游戏路径交付
	void HTTPMessageProcessing_1010(QString &data);
	//检测路径正确性
	bool CheckRoute(QString &str_result, QString &str_name);
	//设置路径
	void SetRoute(QString &str_route, QString &str_name);
	//取web服务器地址
	void OnGetWebAddr();
	//lol solo 赛断线重连
	void web_Processing_3012(QJsonObject TYPE);
	//普通登录转微信登录
	void web_Processing_9011();
	//普通登录窗口关闭交付
	void web_Processing_9012();
	//玩家是否查看对阵图
	void web_Processing_9010(QJsonObject TYPE);
	//炉石联赛玩家pick英雄
	void web_Processing_9016(QJsonObject TYPE);
	//联赛房间内玩家发送聊天消息
	void web_Processing_9017(QJsonObject TYPE);
	//炉石联赛玩家ban英雄
	void web_Processing_9021(QJsonObject TYPE);
	//通知玩家开始联赛小组游戏是否成功
	//UINT resid
	//0 成功
	//1 路径错误
	//2 其他错误
	void tellEventGroupStartRes(int resid);
private:

	virtual void onRecvMsg(const QString &_msg, Cweb_c_type _type)//处理收到的websocket消息
	{
		QWebSocket *pSocket = qobject_cast<QWebSocket *>(sender());
		qDebug() << QStringLiteral("收到websocket消息：")<< _msg;
		Processing(_msg,_type);//数据处理
	}

	virtual void callbackConn(QWebSocket *_socket, bool _isSrv);

	void Processing(const QString &_msg, Cweb_c_type _type);//数据处理
public:
	enum HS_HERO_NAME
	{
		hs_hero_name_Rogue = 1,  //盗贼
		hs_hero_name_Druid,      //德鲁伊
		hs_hero_name_Mage,	     //法师
		hs_hero_name_Hunter, 	 //猎人
		hs_hero_name_pastor,	 //牧师
		hs_hero_name_Shaman,	 //萨满祭司
		hs_hero_name_Paladin,	 //圣骑士
		hs_hero_name_Warlock,	 //术士
		hs_hero_name_Warrior,	 //战士
	};
};

#endif // DATADROCESSING_H
