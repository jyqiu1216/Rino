#ifndef EVENTPLCLENT_H
#define EVENTPLCLENT_H
#include <QtWebEngineWidgets\QWebEngineView> 


#include "common\Common.h"
#include "ui_eventplclent.h"

#include "message\WJNetPlatformMessage.h"
#include "clientsocket/WJSocketMessage.h"
#include "common\WJBaseType.h"

class linkCtQ;
class LoginInter;
class webPopup;
class TeamChallengePage;
class SystemSettings;
class WJPlatformLogic;
class ClientLink;
class linkCtoQ;
class eventPLclent : public QMainWindow
{
	Q_OBJECT

public:
	eventPLclent(QWidget *parent = 0);
	~eventPLclent();
	bool iscolseclent = false;
	void initUI();
	void setuiIsempty();
	void setuiInterfacecontent();
	void send_ServiceArea();
	void setputBUttonqufu();
	
private:
	LoginInter* m_LoginInter;//登录界面
	webPopup* m_webPopup;//web
	TeamChallengePage *m_TeamChallengePage;
	SystemSettings *m_SystemSettings;
	WJPlatformLogic *m_WJPlatformLogic;
	ClientLink *m_ClientLink;
	linkCtoQ *m_linkCtoQ;
	int i;

signals:
	void singnal_setthedeable();

private slots:
	void slots_newpag(QString);
	void slots_Recharge();
	void slots_Platformmall();
	void slots_hall();
	void slots_Entryrecord();
	void slots_Officialnews();
	void slots_Invitefriends();
	void slots_Youmentionmechange();
	void slots_Ladderranking();
	void slots_Setup();
	void slots_Helpcenter();
	void slots_close();
	void slots_pushButton_one();
	void slots_pushButton_two();//第二场赛事
	void slots_pushButton_three();//第三场赛事
	void slots_pushButton_four();//第四场赛事
	void slots_pushButton_five();//第五场赛事
	void slots_pushButton_six();//第六场赛事
	void slots_pag();
	void slots_home();
	void slot_break_hall();
	void slots_personalsolo();
	void slots_teamchange();
	void slot_regame();
	void slot_Alreadyregistered();
	void slots_EntertainmentCurrency();
	void slots_integral();
	void slots_Hearthstone();
	void slots_HeroUnion();

	void slot_LoadUserinformation();
	void slot_pushButton_AIEcho();
	void slot_pushButton_juan();
	void slot_pushButton_Noxus();
	void slot_pushButton_Bandle();
	void slot_pushButton_APyrgi();
	void slot_pushButton_WarCollege();
	void slot_pushButton_GiantGodPeak();
	void slot_pushButton_ReetheGarrison();
	void slot_pushButton_LandOfJudgment();
	void slot_pushButton_BlackRose();
	void slot_pushButton_ShadowLsles();
	void slot_pushButton_SteelHeat();
	void slot_pushButton_BalancedDenomination();
	void slot_pushButton_CrystalSScar();
	void slot_pushButton_ShadowFlow();
	void slot_pushButton_WatchOfTheSea();
	void slot_pushButton_ConquestOfTheSea();
	void slot_pushButton_Kalamunda();
	void slot_pushButton_CityGuard();
	void slot_pushButton_BaoshBirgi();
	void slot_pushButton_Demacia();
	void slot_pushButton_FerrerJuud();
	void slot_pushButton_FearlessVanguard();
	void slot_pushButton_TwistedTreeline();
	void slot_pushButton_TheWrathofRama();
	void slot_pushButton_Dragon();
	void slot_pushButton_Education();


private:
	Ui::eventPLclentClass ui;
	bool eventFilter(QObject *obj, QEvent *e);
	QWebChannel *channel;

	QWebEngineView m_webView;

};

#endif // EVENTPLCLENT_H
