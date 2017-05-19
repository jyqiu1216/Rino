#ifndef CWEBCHANNELIMPL_H
#define CWEBCHANNELIMPL_H

#include <QObject>
#include <QDebug>
//#include "zswebview.h"
#include <qnetworkreply.h>
#include "WJPlatformLogic.h"
#include "Qthread/RoomLogic.h"
#include "common/Log.h"
#include "message\WJNetPlatformMessage.h"
#include "common\WJUserInfoBaseStruct.h"
#include "message\WJNetPM_gamelist.h"
#include "clientsocket\WJSocketMessage.h"
class WJWebView;
//与JS进行通信
class WebChannelExt :public QObject
{
	Q_OBJECT

public:
	WebChannelExt(WJWebView* _webView);
	~WebChannelExt();
	void setbageweb();
public slots:
	void setHeadPortrait(QString userId, QString img);//设置用户头像
	void getHeadPortrait(QString userId);//获取头像信息
	void requestuser(QString userid);//请求用户数据
	void link_web(int gameid, QString matchpoint, QString matchname, QString matchreward);
	void replyFinished(QNetworkReply* reply);
	void replyFinished_field(QNetworkReply* reply);
	void on_readyRead();
	void sing_string(QString str);
	void getFlushState();
	void setFlushState(bool state) { m_isNeedFlush = state; }
	void registeruser();//注册
	void ModifyPassword(QString userId, QString originalPassword, QString newpassword);//修改密码
	void ModifyInformation(QString userId, QString nickname, QString sex);//修改信息
	void Send_phone_binding_verification_code(QString userId, QString type, QString mobile);//发送手机绑定验证码
	void Send_Bound_phone(QString userId, QString mobile, QString checkCode);//绑定手机
	void Send_Remove_binding(QString userId, QString type);//发送手机解除绑定验证码
	void Remove_bound_phone(QString userId, QString checkCode);//解除绑定手机
	void Passwordrecovery();//密码找回
	void slots_Recharge();//充值
	void helpCenter(int iuser_token, int userid);//帮助中心网页跳转
	//@brief:	加载H5页面@param:	[_url] H5页面的URL
	void loadUrl(const QString& _url);
	
	void Jumpweb(QString web);//跳转到网页
	
	void ShearPlate(QString plate);//剪切板

	void query_Free_field_integral();//查询免费场最多积分

	void getallHeadportrait();//获取所有头像图片
	//@brief:	最小化
	void minimized();

	//@brief:	显示窗口
	void showWindow();

	//@brief:	关闭窗口
	void closeWindow();

	// @brief:	打印日志
	void qtLog(const QString& logstr);

// 	/ @brief 关闭page
	void closeWebPage();


	void webeject(const QString& url);
	void setwebsize(int x, int y);

	void getVersion() { emit signal_sendResult(m_version); }


	// $BEGIN$	自定义信号	====================================================================
signals:
	// @brief:	用来连接JS的函数，前台执行函数需要返回结果时，回调时使用
	void signal_sendResult(const QString &text);

	//$END$		自定义信号	====================================================================


	//$BEGIN$	槽	====================================================================

	public slots:
	//$END$	槽	====================================================================

protected:

	WJWebView* m_webView;

	QString m_version = "1.0.0.2";

	bool m_isNeedFlush = true;
};

#endif // CWEBCHANNELIMPL_H
