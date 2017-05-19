#include "clientconfig.h"
Ccconfig& Ccconfig::getInstance()
{
	static Ccconfig cconfig;
	return cconfig;
}

Ccconfig::Ccconfig()
{

}

Ccconfig::~Ccconfig()
{

}

//初始化配置文件，检测如果文件不存在就创建文件，并将默认参数写进文件
void Ccconfig::initconfig()
{
	QString val;
	QFile file;
	file.setFileName("clientconfig.json");
	bool bres = file.exists();
	if (bres)
	{
		file.open(QIODevice::ReadOnly | QIODevice::Text);
		val = file.readAll();
		file.close();
		QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
		QJsonObject sett = d.object();
		QJsonValue value = sett.value(QString("SysSet"));
		qWarning() << "SysSet:  " << value;

		QJsonObject item = value.toObject();
		QJsonDocument d2(item);
		QJsonObject sett2 = d2.object();
		{
			
			QJsonValue value = sett2.value(QString("hall_server_address"));
			m_data.hall_server_address = value.toString();
		}

		{
			QJsonValue value = sett2.value(QString("web_server_address"));
			m_data.web_server_address = value.toString();
		}

		{
			QJsonValue value = sett2.value(QString("hall_server_port"));
			m_data.hall_server_port = value.toInt();
		}

		{//加载游戏队列的游戏id
			QJsonValue value = sett2.value(QString("gameid"));
			m_data.gameid = value.toInt();
		}

		{//加载游戏队列的区服id
			QJsonValue value = sett2.value(QString("ereaid"));
			m_data.ereaid = value.toInt();
		}

		{//加载游戏队列的比赛id
			QJsonValue value = sett2.value(QString("contestid"));
			m_data.contestid = value.toInt();
		}

		{//需要进入的房间的房间id
			QJsonValue value = sett2.value(QString("roomid"));
			m_data.roomid = value.toInt();
		}
	}
	else
	{
		QString json("{\n"
			"\"SysSet\" :\n"
			"	{\n"
			"	\"hall_server_address\": \"127.0.0.1\",\n"
			"	\"web_server_address\" : \"127.0.0.1\",\n"
			"	\"hall_server_port\" : 13050,\n"
			"	\"gameid\": 0,\n"
			"	\"ereaid\": 0,\n"
			"	\"contestid\": 1,\n"
			"	\"roomid\": 737\n"
			"	}\n"
			"}\n");
		char*  ch;
		QByteArray ba = json.toLocal8Bit();
		ch = ba.data();
		file.open(QIODevice::WriteOnly | QIODevice::Text);
		file.write(ch);
		file.close();
	}
}

//获取数据
const Ccconfig::Ccconfig_data* Ccconfig::GetData()
{
	static bool bInit = false;
	if (!bInit)
	{
		initconfig();
		bInit = true;
	}
	return &m_data;
}