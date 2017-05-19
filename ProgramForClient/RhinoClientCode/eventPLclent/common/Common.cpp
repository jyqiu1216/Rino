#include "Common.h"
#include "common/WJGameInfo.h"
#include <assert.h>
#include "Psapi.h"
#include "clientsocket/commonfunction_c.h"
#include <QLibrary>
#include <tlhelp32.h>

typedef QString(*printlog)();
typedef void(*SetLogFileName)();
typedef QString(*EstablishJournal)(QString);
typedef void(*outputlogMessage)(QtMsgType, const QMessageLogContext&, const QString&);

typedef std::function<IBaseMyDataDrocessing* ()> GAME_CREATE_MY_DATA_DROCESSING;
typedef std::function<IBaseMyHallLogic* ()> GAME_CREATE_MY_HALL_LOGIC;
typedef std::function<IBaseMyGameRoom* ()> GAME_CREATE_MY_ROOM;

struct RegisterGameStuct
{
	E_GameID iGameid;
	GAME_CREATE_MY_DATA_DROCESSING pClass_MyDataDrocessing;
	GAME_CREATE_MY_HALL_LOGIC pClass_MyGameHall;
	GAME_CREATE_MY_ROOM pClass_MyGameRoom;
};

static RegisterGameStuct g_GameLogic[] = 
{
	//英雄联盟
	{ e_gameid_10000001, WJ_gameid_10000001::CMyDataDrocessing::Create_MyDataDrocessing
	, WJ_gameid_10000001::CMyHallLogic::Create_MyHallLogic
	, WJ_gameid_10000001::CMyGameRoom::Create_MyGameRoom }
	,
	//炉石传说
	{ e_gameid_10000002, WJ_gameid_10000002::CMyDataDrocessing::Create_MyDataDrocessing
	, WJ_gameid_10000002::CMyHallLogic::Create_MyHallLogic
	, WJ_gameid_10000002::CMyGameRoom::Create_MyGameRoom }

};

CCommon* g_pCCommon = nullptr;

// 注册游戏列表
std::unordered_map<E_GameID, RegisterGameStuct> g_game_creators;//注册的游戏

QString GBK2UTF8(const QString &inStr)
{
	QTextCodec *gbk = QTextCodec::codecForName("GB18030");
	QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");

	QString g2u = gbk->toUnicode(gbk->fromUnicode(inStr));              // gbk  convert utf8  
	return g2u;
}

QString UTF82GBK(const QString &inStr)
{
	QTextCodec *gbk = QTextCodec::codecForName("GB18030");
	QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");

	QString utf2gbk = gbk->toUnicode(inStr.toLocal8Bit());
	return utf2gbk;
}

std::string UnicodeToUTF8(const wchar_t* str)
{
	std::string	result;

	int n = WideCharToMultiByte(CP_UTF8, 0, str, -1, 0, 0, 0, 0);
	if (n == 0)
		result.resize(0);
	else
		result.resize(n - 1);
	WideCharToMultiByte(CP_UTF8, 0, str, -1, (char*)result.c_str(), (int)result.length(), 0, 0);

	return result;
}

//
// GetDebugPriv
// 在 Windows NT/2000/XP 中可能因权限不够导致以上函数失败
// 如以　System 权限运行的系统进程，服务进程
// 用本函数取得　debug 权限即可,Winlogon.exe 都可以终止哦 :)
//
BOOL GetDebugPriv()
{
	HANDLE hToken;
	LUID sedebugnameValue;
	TOKEN_PRIVILEGES tkp;
	if (!OpenProcessToken(GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{
		return FALSE;
	}

	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &sedebugnameValue))
	{
		CloseHandle(hToken);
		return FALSE;
	}
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = sedebugnameValue;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof tkp, NULL, NULL))
	{
		CloseHandle(hToken);
		return FALSE;
	}
	return TRUE;
}

//
// FindProcess
// 这个函数唯一的参数是你指定的进程名，如:你的目标进程
// 是 "Notepad.exe",返回值是该进程的ID，失败返回0
//
DWORD FindProcess(char *strProcessName)
{
	DWORD aProcesses[1024], cbNeeded, cbMNeeded;
	HMODULE hMods[1024];
	HANDLE hProcess;
	char szProcessName[MAX_PATH];
	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) return 0;
	for (int i = 0; i < (int)(cbNeeded / sizeof(DWORD)); i++)
	{
		//_tprintf(_T("%d/t"), aProcesses[i]);  
		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, aProcesses[i]);
		EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbMNeeded);
		GetModuleFileNameExA(hProcess, hMods[0], szProcessName, sizeof(szProcessName));

		if (strstr(szProcessName, strProcessName))
		{
			//_tprintf(_T("%s;"), szProcessName);  
			return (aProcesses[i]);
		}
		//_tprintf(_T("/n"));  
	}
	return 0;
}

//
// Function: ErrorForce
// 此函数中用上面的 FindProcess 函数获得你的目标进程的ID
// 用WIN API OpenPorcess 获得此进程的句柄，再以TerminateProcess
// 强制结束这个进程
//
VOID KillProcess(char *strProcessName)
{
	GetDebugPriv();
	// When the all operation fail this function terminate the "winlogon" Process for force exit the system.  
	HANDLE hYourTargetProcess = OpenProcess(PROCESS_TERMINATE |
		PROCESS_QUERY_INFORMATION |   // Required by Alpha  
		PROCESS_CREATE_THREAD |   // For CreateRemoteThread  
		PROCESS_VM_OPERATION |   // For VirtualAllocEx/VirtualFreeEx  
		PROCESS_VM_WRITE,             // For WriteProcessMemory  
		FALSE, FindProcess(strProcessName));
	if (hYourTargetProcess == NULL)
	{
		return;
	}
	bool bres = TerminateProcess(hYourTargetProcess, 0);
	return;
}
bool ProcessIsLOL(DWORD pid)
{

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hSnapshot)
	{
		return 0;
	}
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(pe32);
	BOOL res;
	for (res = Process32First(hSnapshot, &pe32); res; res = Process32Next(hSnapshot, &pe32))
	{
		if (0 == wcscmp(pe32.szExeFile, L"League of Legends.exe") && pe32.th32ProcessID == pid)
		{
			return true;
		}
	}
	CloseHandle(hSnapshot);

	return false;

}

DWORD GetProcessIdByName(LPCTSTR pName, bool bCheck)
{
	DWORD pID = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hSnapshot)
	{
		return 0;
	}
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(pe32);
	BOOL res;
	for (res = Process32First(hSnapshot, &pe32); res; res = Process32Next(hSnapshot, &pe32))
	{
		if (0 == wcscmp(pe32.szExeFile, pName))
		{
			if (bCheck)
			{

				if (!ProcessIsLOL(pe32.th32ParentProcessID))
				{

					pID = pe32.th32ProcessID;
				}
			}
			else {

				pID = pe32.th32ProcessID;
			}
		}
	}
	CloseHandle(hSnapshot);
	return pID;
}

BOOL GetProcessModule(DWORD dwPID, wchar_t* dll)
{
	BOOL bRet = FALSE;
	BOOL bFound = FALSE;
	HANDLE hModuleSnap = NULL;
	MODULEENTRY32 me32 = { 0 };
	me32.dwSize = sizeof(MODULEENTRY32);

	hModuleSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);//创建进程快照 
	if (hModuleSnap == INVALID_HANDLE_VALUE)
	{
		printf("获取模块失败!\n");
		return FALSE;
	}


	if (::Module32First(hModuleSnap, &me32))//获得第一个模块 
	{
		do{

			//printf("方法1列模块名：%s\n", me32.szExePath);
			if (!wcscmp(me32.szModule, dll))
			{
				bFound = TRUE;
				break;
			}
			Sleep(1);
		} while (::Module32Next(hModuleSnap, &me32));
	}//递归枚举模块 


	CloseHandle(hModuleSnap);
	return bFound;
}

CCommon* CCommon::getInstance()
{
	if (nullptr == g_pCCommon)
	{
		g_pCCommon = new CCommon();
	}

	return g_pCCommon;
}

IBaseMyDataDrocessing* g_pCMyDataDrocessing = nullptr;

CCommon::CCommon(void)
{
	m_NowGame = e_gameid_10000001;
	m_NowContest = 0;
	m_NowRoom = 0;

	currentpath = QCoreApplication::applicationDirPath();//保存当前程序路
	
	//注册游戏
	RegisterGameList();
}

CCommon::~CCommon(void)
{
}

//注册游戏
void CCommon::RegisterGameList()
{
	// 添加游戏
	int gameCount = sizeof(g_GameLogic) / sizeof(RegisterGameStuct);
	for (int i = 0; i < gameCount; i++)
	{
		assert((0 != g_GameLogic[i].iGameid)
			&& (nullptr != g_GameLogic[i].pClass_MyDataDrocessing)
			&& (nullptr != g_GameLogic[i].pClass_MyGameHall)
			&& (nullptr != g_GameLogic[i].pClass_MyGameRoom));

		g_GameLogic[i].pClass_MyDataDrocessing()->InitData(g_GameLogic[i].pClass_MyGameHall(), g_GameLogic[i].pClass_MyGameRoom());
		g_GameLogic[i].pClass_MyGameHall()->InitData(g_GameLogic[i].pClass_MyDataDrocessing(), g_GameLogic[i].pClass_MyGameRoom());
		g_GameLogic[i].pClass_MyGameRoom()->InitData(g_GameLogic[i].pClass_MyDataDrocessing(), g_GameLogic[i].pClass_MyGameHall());

		auto value = std::make_pair(g_GameLogic[i].iGameid, g_GameLogic[i]);
		g_game_creators.insert(value);
	}
}

IBaseMyDataDrocessing* CCommon::GetMyDataDrocessing(E_GameID igameid)
{
	auto iter = g_game_creators.find(igameid);
	if (iter != g_game_creators.end())
	{
		return (*iter).second.pClass_MyDataDrocessing();
	}

	return nullptr;
}

IBaseMyHallLogic* CCommon::GetMyHallLogic(E_GameID igameid)
{
	auto iter = g_game_creators.find(igameid);
	if (iter != g_game_creators.end())
	{
		return (*iter).second.pClass_MyGameHall();
	}

	return nullptr;
}

IBaseMyGameRoom* CCommon::GetMyGameRoom(E_GameID igameid)
{
	auto iter = g_game_creators.find(igameid);
	if (iter != g_game_creators.end())
	{
		return (*iter).second.pClass_MyGameRoom();
	}

	return nullptr;
}

//弹出框
bool CCommon::BoxMess(QWidget *widget, QString title, QString okButton, QString noButton, int butNub)
{
	if (nullptr == widget)
		return false;

	QMbox m_QMbox;
	m_QMbox.setParent(widget);
	widget->raise();
	m_QMbox.setcontent(title);
	m_QMbox.SetTextOKBtn(okButton);
	m_QMbox.SetTextNOBtn(noButton);
	if (butNub < 2)
	{
	m_QMbox.hideBtuno();
	}
	int wd = widget->width();
	int ht = widget->height();
	m_QMbox.move((wd - m_QMbox.width()) / 2, (ht - m_QMbox.height()) / 2);
	m_QMbox.show();
	return (m_QMbox.exec() == QDialog::Accepted) ? true : false;
}

//设置游戏文件
void CCommon::GetAllGameRoute()
{
	QFile file;
	file.setFileName(currentpath + "/Config/Routeconfig.xml");
	if (file.exists())
	{
		file.open(QIODevice::ReadOnly | QIODevice::Text);
		QJsonParseError json_errorver;
		QJsonDocument doucmentver = QJsonDocument::fromJson(QString::fromLocal8Bit(file.readAll()).toStdString().c_str(), &json_errorver);
		if (json_errorver.error == QJsonParseError::NoError)
		{
			if (doucmentver.isObject())
			{
				if (doucmentver.object().contains("lol"))
				{//英雄联盟
					lolroute = doucmentver.object().value("lol").toString();
				}
				if (doucmentver.object().contains("HearthStone"))
				{//炉石传说
					HearthStoneroute = doucmentver.object().value("HearthStone").toString();

					QString str1 = HearthStoneroute.toStdString().c_str();
					char a[MAX_PATH] = { 0 };
					sprintf_s(a, "sdp_test c str1=%s", str1.toStdString().c_str());
					OutputDebugStringA(a);
				}
			}
		}
		file.close();
	}
	else
	{
		file.open(QIODevice::ReadWrite | QIODevice::Text);
		QTextStream out(&file);

		QJsonObject jsonpart;
		jsonpart.insert(QStringLiteral("lol"), lolroute);
		jsonpart.insert(QStringLiteral("HearthStone"), HearthStoneroute);
		QJsonDocument documentpack;
		documentpack.setObject(jsonpart);
		QString outjson(documentpack.toJson(QJsonDocument::Compact));
		qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("游戏路径保存成功");
		out << outjson;
		out.flush();
		file.close();
	}
}

bool CCommon::GetLolRoute()
{
	QFile file;
	file.setFileName(currentpath + "/Config/Routeconfig.xml");
	if (file.exists())
	{
		file.open(QIODevice::ReadOnly | QIODevice::Text);
		QJsonParseError json_errorver;
		QJsonDocument doucmentver = QJsonDocument::fromJson(QString::fromLocal8Bit(file.readAll()).toStdString().c_str(), &json_errorver);
		if (json_errorver.error == QJsonParseError::NoError)
		{
			if (doucmentver.isObject())
			{
				if (doucmentver.object().contains("lol"))
				{//英雄联盟
					lolroute = doucmentver.object().value("lol").toString();
					if (lolroute != QStringLiteral(""))
					{
						file.close();
						return true;
					}
				}
			}
		}
		file.close();
	}
	return false;
}
//写入游戏文件地址
void CCommon::SetGameRoute()
{
	QString str = currentpath + "/Config/Routeconfig.xml";
	QString str_gb = UTF82GBK(str);

	//配置文件只读处理
	DWORD dwAttributes = ::GetFileAttributesA(str_gb.toStdString().c_str());

	if (dwAttributes & FILE_ATTRIBUTE_READONLY) {

		DWORD dwTempAttribute = dwAttributes & (~FILE_ATTRIBUTE_READONLY);

		::SetFileAttributesA(str_gb.toStdString().c_str(), dwTempAttribute);
	}

	QFile file;
	file.setFileName(str);
	if (file.exists())
	{
		file.open(QIODevice::WriteOnly | QIODevice::Text);
		QTextStream out(&file);

		QJsonObject jsonpart;
		jsonpart.insert(QStringLiteral("lol"), lolroute);
		jsonpart.insert(QStringLiteral("HearthStone"), HearthStoneroute);
		QJsonDocument documentpack;
		documentpack.setObject(jsonpart);
		QString outjson(documentpack.toJson(QJsonDocument::Compact));

		out << outjson;
		out.flush();
		file.close();
	}
	else
	{
		GetAllGameRoute();
	}
}

///保存大厅信息
void CCommon::SetHallData(QString str)
{
	CommonData()->HallData.clear();
	HALLDATA HallDataH;
	QJsonParseError json_error;
	QJsonDocument parse_doucment = QJsonDocument::fromJson(str.toStdString().c_str(), &json_error);
	if (json_error.error == QJsonParseError::NoError)
	{
		if (parse_doucment.isArray())
		{
			QJsonArray array = parse_doucment.array();
			int size = array.size();
			for (int i = 0; i < size; i++)
			{
				QJsonObject object = array.at(i).toObject();
				if (object.contains("id"))
				{
					QJsonValue name_value = object.value("id");
					HallDataH.id = name_value.toInt();
				}
				if (object.contains("name"))
				{
					QJsonValue name_value = object.value("name");
					HallDataH.name = name_value.toString();
				}
				if (object.contains("serverId"))
				{
					QJsonValue name_value = object.value("serverId");
					HallDataH.server_id = name_value.toInt();
				}
				if (object.contains("pointType"))
				{
					QJsonValue name_value = object.value("pointType");
					HallDataH.point_type = name_value.toInt();
				}
				if (object.contains("pointName"))
				{
					QJsonValue name_value = object.value("pointName");
					HallDataH.pointName = name_value.toString();
				}
				if (object.contains("type"))
				{
					QJsonValue name_value = object.value("type");
					if (name_value.toInt())
						HallDataH.type = name_value.toInt();
				}
				if (object.contains("icon"))
				{
					QJsonValue name_value = object.value("icon");
					HallDataH.icon = name_value.toString();
				}
				if (object.contains("profit"))
				{
					QJsonValue name_value = object.value("profit");
					HallDataH.profit = name_value.toInt();
				}
				if (object.contains("profitType"))
				{
					QJsonValue name_value = object.value("profitType");
					HallDataH.profitType = name_value.toInt();
				}
				if (object.contains("criteria"))
				{
					QJsonValue name_value = object.value("criteria");
					HallDataH.criteria = name_value.toInt();
				}
				if (object.contains("mode"))
				{
					QJsonValue name_value = object.value("mode");
					HallDataH.mode = name_value.toInt();
				}
				if (object.contains("maxNumber"))
				{
					QJsonValue name_value = object.value("maxNumber");
					HallDataH.max_number = name_value.toInt();
				}
				if (object.contains("sortNo"))
				{
					QJsonValue name_value = object.value("sortNo");
					HallDataH.sort_no = name_value.toInt();
				}
				if (object.contains("prize"))
				{
					QJsonValue name_value = object.value("prize");
					HallDataH.prize = name_value.toString();
				}

				CommonData()->HallData.insert(HallDataH.id, HallDataH);
			}
		}
	}
}

//获得当前比赛详情
void CCommon::GetNowContestInfo()
{
	for (auto iter = CommonData()->HallData.begin(); iter != CommonData()->HallData.end(); iter++)
	{
		if (iter.value().id == CommonData()->m_NowContest)
		{
			m_NowContestInfo = (*iter);
			break;
		}
	}
}