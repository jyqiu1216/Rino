#include "ClientLink.h"
#include "common/WJUserInfoBaseStruct.h"
#include "message/WJNetPM_login.h"
#include "WJPlatformLogic.h"
#include "common/Common.h"
#include "RoomLogic.h"
CClientLink* g_pCClientLink = nullptr;

CClientLink::CClientLink(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	Qt::WindowFlags flag = (Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint);
	setWindowFlags(flag);
	setAttribute(Qt::WA_TranslucentBackground, true);

	connect(ui.pushButton_Reconnect, SIGNAL(clicked()), this, SLOT(onStart()));
}

CClientLink::~CClientLink()
{

}

CClientLink *CClientLink::getInstance()
{
	if (nullptr == g_pCClientLink)
		g_pCClientLink = new CClientLink();
	return g_pCClientLink;
}

void CClientLink::onStart()
{
	ui.pushButton_close->hide();
	ui.pushButton_Reconnect->hide();
	//LOG_DEBUG("正在启动请稍后");
	ui.label->setText(QStringLiteral("正在启动请稍后..........."));
	show();
	RoomLogic()->SocketClose();
	WJ::PlatformLogic().connect((char*)clientconfig().GetData()->hall_server_address.c_str(), clientconfig().GetData()->hall_server_port);
}
void CClientLink::slot_landlink(bool  isllink)
{
	if (isllink)
	{
		hide();
	}
	else
	{
		ui.pushButton_close->show();

		ui.pushButton_Reconnect->show();
		//LOG_DEBUG("连接不成功，请检查网络");
		ui.label->setText(QStringLiteral("连接不成功，请检查网络.............."));
		isllink = false;
	}
	//sdp!这里不要直接登陆，说过是收到ASS_NET_CONNECT_2消息后才能登陆
}



