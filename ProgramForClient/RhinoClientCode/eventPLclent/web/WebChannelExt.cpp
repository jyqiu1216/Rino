#include "WebChannelExt.h"
#include "WJWebView.h"
#include "message\WJNetPlatformMessage.h"
#include "message\WJNetRM_user_action.h"
#include "message\WJNetRoomMessage.h"
#include "common\WJUserInfoBaseStruct.h"
#include "message\WJNetPM_apply.h"
#include "WJMainWindow.h"
#include "common/WJGameInfo.h"
#include "Qthread/RoomLogic.h"
//app主目录
extern QString g_appPath;
extern QString g_appResourcePath;

WebChannelExt::WebChannelExt(WJWebView* webwindowptr):m_webView(webwindowptr)
{
	CommonData()->bExecutionAutomationDoing = false;
}
void WebChannelExt::getFlushState()
{
    if(m_isNeedFlush)
        emit signal_sendResult("true");
    else
        emit signal_sendResult("false");
}
void WebChannelExt::getallHeadportrait()//获取所有头像图片
{
	QJsonObject jsonpart;
	for (int i = 0; i < 28; i++)
	{
		jsonpart.insert(QString("%1").arg(i), QStringLiteral("http://cdn.xiniugame.com/portrait/") + QString("%1").arg(i) + QStringLiteral(".jpg"));
	}
	QJsonDocument documentpack;
	documentpack.setObject(jsonpart);
	QString json_json(documentpack.toJson(QJsonDocument::Compact));
	emit signal_sendResult(json_json);
	qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("所有头像数据:") << json_json;
}
void WebChannelExt::setHeadPortrait(QString userId,QString img)//设置用户头像
{
	//创建一个管理器
	QNetworkAccessManager *manager = new QNetworkAccessManager(this);
	//连接请求结束信号
	connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
	//创建一个请求
	QNetworkRequest request;
	QString str = Q_WEB_ROOT_ADDRESS + QStringLiteral("api/uploadImage?userId=") + userId + QStringLiteral("&img=") + img;
	request.setUrl(QUrl(str));
	qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("发送手机解除绑定验证码:") << str;
	//发送GET请求
	QNetworkReply *reply = manager->get(request);
	//连接响应时返回数据信号
	connect(reply, SIGNAL(readyRead()), this, SLOT(on_readyRead()));
}
void WebChannelExt::getHeadPortrait(QString userId)//获取头像信息
{
	//创建一个管理器
	QNetworkAccessManager *manager = new QNetworkAccessManager(this);
	//连接请求结束信号
	connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
	//创建一个请求
	QNetworkRequest request;
	QString str = Q_WEB_ROOT_ADDRESS + QStringLiteral("api/userInfo?id=") + userId;
	request.setUrl(QUrl(str));
	qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("发送手机解除绑定验证码:") << str;
	//发送GET请求
	QNetworkReply *reply = manager->get(request);
	//连接响应时返回数据信号
	connect(reply, SIGNAL(readyRead()), this, SLOT(on_readyRead()));
}


void WebChannelExt::loadUrl(const QString &_url)
{
    qDebug()<<_url<<endl;
    m_isNeedFlush = true;
    //m_webView->load(QUrl(_url));
}
void WebChannelExt::setbageweb()
{
	qDebug() << CommonData()->Resultspagecontent;
}

void WebChannelExt::minimized()
{
    ((QWidget*)(m_webView->parent()))->showMinimized();
}

void WebChannelExt::showWindow()
{
    m_webView->show();
}
void WebChannelExt::closeWindow()
{
	WJMainWindow()->hide();
	WJMainWindow()->close();
}
WebChannelExt::~WebChannelExt()
{	
	qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("~WebChannelExt()");
	WJMainWindow()->close();
	ClientLink()->close();
}
void WebChannelExt::qtLog(const QString& logstr)
{
	qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << logstr << endl;
}

void WebChannelExt::closeWebPage()
{
    m_webView->showMinimized();
}
void WebChannelExt::webeject(const QString& url)
{
    qDebug()<<"string "<<url<<endl;
    m_webView->load(QUrl(url));
}
 void WebChannelExt::setwebsize(int x,int y)
 {

 }

 void WebChannelExt::requestuser(QString userid)//请求用户数据
 {
	 //创建一个管理器
	 QNetworkAccessManager *manager = new QNetworkAccessManager(this);
	 //连接请求结束信号
	 connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
	 //SystemLog::getInstance()->writeLog(SystemLog::E_LOG_DEBUG, "WJTestLOG.log", "1", 1, "请求用户数据");

	 //创建一个请求
	 QNetworkRequest request;
	 QString str = Q_WEB_ROOT_ADDRESS + QStringLiteral("api/fight/collection?userId=") + userid;
		request.setUrl(QUrl(str));
		qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("请求用户数据:") << str;
	 //发送GET请求
	 QNetworkReply *reply = manager->get(request);
	 //连接响应时返回数据信号
	 connect(reply, SIGNAL(readyRead()), this, SLOT(on_readyRead()));
 }

 void WebChannelExt::ModifyPassword(QString userId, QString originalPassword, QString newpassword)//修改密码
 {
	 //创建一个管理器
	 QNetworkAccessManager *manager = new QNetworkAccessManager(this);
	 //连接请求结束信号
	 connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
	 //SystemLog::getInstance()->writeLog(SystemLog::E_LOG_DEBUG, "WJTestLOG.log", "1", 1, "发送修改密码");
	 //创建一个请求
	 QNetworkRequest request;
	 QString str = Q_WEB_ROOT_ADDRESS + QStringLiteral("api/changePassword?userId=") + userId + QStringLiteral("&originalPassword=") + originalPassword + QStringLiteral("&newpassword=") + newpassword;
	 request.setUrl(QUrl(str));
	 qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("修改密码:") << str;
	 //发送GET请求
	 QNetworkReply *reply = manager->get(request);
	 //连接响应时返回数据信号
	 connect(reply, SIGNAL(readyRead()), this, SLOT(on_readyRead()));
 }
 void WebChannelExt::ModifyInformation(QString userId, QString nickname, QString sex)//修改信息
 {
	 //创建一个管理器
	 QNetworkAccessManager *manager = new QNetworkAccessManager(this);
	 //连接请求结束信号
	 connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
	 //SystemLog::getInstance()->writeLog(SystemLog::E_LOG_DEBUG, "WJTestLOG.log", "1", 1, "发送修改用户信息");
	 //创建一个请求
	 QNetworkRequest request;
	 QString str = Q_WEB_ROOT_ADDRESS + QStringLiteral("api/updateUserInfo?userId=") + userId + QStringLiteral("&nickname=") + nickname + QStringLiteral("&sex=") + sex;
	 request.setUrl(QUrl(str));
	 qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("修改信息:") << str;
	 //发送GET请求
	 QNetworkReply *reply = manager->get(request);
	 //连接响应时返回数据信号
	 connect(reply, SIGNAL(readyRead()), this, SLOT(on_readyRead()));
 }
 void WebChannelExt::Send_phone_binding_verification_code(QString userId, QString type, QString mobile)//发送手机绑定验证码
 {
	 //创建一个管理器
	 QNetworkAccessManager *manager = new QNetworkAccessManager(this);
	 //连接请求结束信号
	 connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
	 //SystemLog::getInstance()->writeLog(SystemLog::E_LOG_DEBUG, "WJTestLOG.log", "1", 1, "发送手机绑定验证码");
	 //创建一个请求
	 QNetworkRequest request;
	 QString str = Q_WEB_ROOT_ADDRESS + QStringLiteral("api/sendSMSCode?userId=") + userId + QStringLiteral("&type=") + type + QStringLiteral("&mobile=") + mobile;
		 request.setUrl(QUrl(str));
	 qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("修改信息:") << str;
	 //发送GET请求
	 QNetworkReply *reply = manager->get(request);
	 //连接响应时返回数据信号
	 connect(reply, SIGNAL(readyRead()), this, SLOT(on_readyRead()));
 }
 void WebChannelExt::Send_Bound_phone(QString userId, QString mobile, QString checkCode)//绑定手机
 {
	 //创建一个管理器
	 QNetworkAccessManager *manager = new QNetworkAccessManager(this);
	 //连接请求结束信号
	 connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
	 //SystemLog::getInstance()->writeLog(SystemLog::E_LOG_DEBUG, "WJTestLOG.log", "1", 1, "绑定手机");
	 //创建一个请求
	 QNetworkRequest request;
	 QString str = Q_WEB_ROOT_ADDRESS + QStringLiteral("api/bindTelephone?userId=") + userId + QStringLiteral("&mobile=") + mobile + QStringLiteral("&checkCode=") + checkCode;
	 request.setUrl(QUrl(str));
	 qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("绑定手机:") << str;
	 //发送GET请求
	 QNetworkReply *reply = manager->get(request);
	 //连接响应时返回数据信号
	 connect(reply, SIGNAL(readyRead()), this, SLOT(on_readyRead()));
 }
 void WebChannelExt::Send_Remove_binding(QString userId, QString type)//发送手机解除绑定验证码
 {
	 //创建一个管理器
	 QNetworkAccessManager *manager = new QNetworkAccessManager(this);
	 //连接请求结束信号
	 connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
	 //SystemLog::getInstance()->writeLog(SystemLog::E_LOG_DEBUG, "WJTestLOG.log", "1", 1, "发送手机解除绑定验证码");
	 //创建一个请求
	 QNetworkRequest request;
	 QString str = Q_WEB_ROOT_ADDRESS + QStringLiteral("api/sendSMSCode?userId=") + userId + QStringLiteral("&type=") + type;
	 request.setUrl(QUrl(str));
	 qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("发送手机解除绑定验证码:") << str;
	 //发送GET请求
	 QNetworkReply *reply = manager->get(request);
	 //连接响应时返回数据信号
	 connect(reply, SIGNAL(readyRead()), this, SLOT(on_readyRead()));
 }
 void WebChannelExt::Remove_bound_phone(QString userId, QString checkCode)//解除绑定手机
 {
	 //创建一个管理器
	 QNetworkAccessManager *manager = new QNetworkAccessManager(this);
	 //连接请求结束信号
	 connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
	 ///SystemLog::getInstance()->writeLog(SystemLog::E_LOG_DEBUG, "WJTestLOG.log", "1", 1, "解除绑定手机");
	 //创建一个请求
	 QNetworkRequest request;
	 QString str = Q_WEB_ROOT_ADDRESS + QStringLiteral("api/bindTelephone?userId=") + userId + QStringLiteral("&checkCode=") + checkCode;
	 request.setUrl(QUrl(str));
	 qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("解除绑定手机:") << str;
	 //发送GET请求
	 QNetworkReply *reply = manager->get(request);
	 //连接响应时返回数据信号
	 connect(reply, SIGNAL(readyRead()), this, SLOT(on_readyRead()));
 }


 void WebChannelExt::registeruser()//注册
{
	qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("注册.");
	WJMainWindow()->showRegistrationPage(true);
}
void WebChannelExt::slots_Recharge()//充值
{
	qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("充值");
	QDesktopServices::openUrl(QUrl(Q_WEB_ROOT_ADDRESS + QStringLiteral("recharge/page")));
}
 void WebChannelExt::Passwordrecovery()//密码找回
{
	qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("密码找回.");
	QDesktopServices::openUrl(QUrl(QStringLiteral("http://www.xiniugame.com/web/loginweb")));
}
 void WebChannelExt::Jumpweb(QString web)//跳转到网页
 {
	qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("跳转到网页：") << web;
	QDesktopServices::openUrl(QUrl(web));
 }
 void WebChannelExt::ShearPlate(QString plate)//剪切板
 {
 	qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("剪切板:") << plate;
	QClipboard *board = QApplication::clipboard();
	board->setText(plate);
 }
void WebChannelExt::replyFinished(QNetworkReply* reply)
{
	QByteArray data = reply->readAll();//网页内容输出
	qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("网页数据数据:")<<QString::fromUtf8(data).toStdString().c_str();
	emit signal_sendResult(QString::fromUtf8(data).toStdString().c_str());
}
void  WebChannelExt::helpCenter(int iuser_token, int userid)//帮助中心网页跳转
{
	qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("帮助中心网页跳转.")
		<< iuser_token << userid;
	if (0 == iuser_token)
	{
		QDesktopServices::openUrl(QUrl(QStringLiteral("https://xiniugame.kf5.com/hc/")));
	}
	else
	{
		QDateTime current_date_time = QDateTime::currentDateTime();
		QString current_date = current_date_time.toString("MMdd");
		QString x;
		int icount;
		QString y = current_date.left(1);
		if (0 == atoi(y.toStdString().c_str()))
		{
			x = current_date.mid(2, 3);
			icount = userid*atoi(x.toStdString().c_str()) + 100;
		}
		icount = userid*atoi(current_date.toStdString().c_str()) + 100;
		QString str = QStringLiteral("http://xiniugame.com/sso/kf5Login?token=");
		str += QStringLiteral("abcde");
		str += QString::number(icount, 10);
		QDesktopServices::openUrl(QUrl(str));
	}
	
}
void WebChannelExt::sing_string(QString str)
{
	emit signal_sendResult(str);
}

void WebChannelExt::query_Free_field_integral()//查询免费场最多积分
{
	//创建一个管理器
	QNetworkAccessManager *manager = new QNetworkAccessManager(this);
	//连接请求结束信号
	connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished_field(QNetworkReply*)));
	//创建一个请求
	QNetworkRequest request;
	QString str = Q_WEB_ROOT_ADDRESS + QStringLiteral("api/fight/getNowfundcoin?userId=") + CommonData()->userinfo.dwUserID;
	request.setUrl(QUrl(str));
	qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("查询免费场最多积分:") << str;
	//发送GET请求
	QNetworkReply *reply = manager->get(request);
	//连接响应时返回数据信号
	connect(reply, SIGNAL(readyRead()), this, SLOT(on_readyRead()));
}
void WebChannelExt::replyFinished_field(QNetworkReply* reply)
{
	QByteArray data = reply->readAll();//查询免费场最多积分返回
	qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("查询免费场最多积分返回:") << QString::fromUtf8(data).toStdString().c_str();
	CommonData()->userinfo.rewardTotal = QString::fromUtf8(data).toStdString().c_str();
}
void WebChannelExt::link_web(int gameid, QString matchpoint, QString matchname, QString matchreward)//一键开赛接口
{

}
void WebChannelExt::on_readyRead()
{
	//....
}
