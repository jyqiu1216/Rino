
#include "web/WJMainWindow.h"
#include <DbgHelp.h>
#include "ErrorModule.hpp"
#include <QtWidgets/QApplication>
#include <QFile>
#include "common/Common.h"
#include "common/Log.h"
#include "hook/ExtendGameManager.h"
#include <tchar.h>
#include <QLibrary>
#include <windows.h>
#include "web/zsxcbeventfilter.h"
#include "websocket/WebSocketServer.h"
#include "DataDrocessing.h"
#include "WebSocketMessInfo.h"
#include "Qthread\RoomLogic.h"
#include "resource.h"
#include "common/LoadWindowsMess.h"
#include "clientsocket/commonfunction_c.h"
extern "C" { _declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001; }
extern "C" { __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1; }

#include <qdatetime.h>

#ifdef _DEBUG

#else

//隐藏dos窗口
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

#endif // _DEBUG


void dbg_dump(struct _EXCEPTION_POINTERS* ExceptionInfo)
{
	//创建 Dump 文件
	HANDLE hDumpFile = CreateFileA(QTime::currentTime().toString("HH-mm-ss-zzz.dmp").toStdString().c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hDumpFile != INVALID_HANDLE_VALUE){
		//Dump信息
		MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
		dumpInfo.ExceptionPointers = ExceptionInfo;
		dumpInfo.ThreadId = GetCurrentThreadId();
		dumpInfo.ClientPointers = TRUE;
		//写入Dump文件内容
		MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);
	}


	wchar_t wchBuffer[1024] = { 0 };
	wchar_t wchModuleName[512] = { 0 };
	AddrGetModule((DWORD)ExceptionInfo->ExceptionRecord->ExceptionAddress, wchModuleName, ARRAYSIZE(wchModuleName));

	::swprintf_s(wchBuffer, ARRAYSIZE(wchBuffer), L"异常模块:\n%ws\nCRASH CODE:0x%.8X\nADDR=0x%.8X\nFLAGS=0x%.8X\nPARAMS=0x%.8X\n"
		L"EAX=%.8X\nECX=%.8X\nESP=%.8X\nEBP=%.8X\n",
		wchModuleName,
		ExceptionInfo->ExceptionRecord->ExceptionCode,
		(DWORD)ExceptionInfo->ExceptionRecord->ExceptionAddress,
		ExceptionInfo->ExceptionRecord->ExceptionFlags,
		ExceptionInfo->ExceptionRecord->NumberParameters,
		ExceptionInfo->ContextRecord->Eax,
		ExceptionInfo->ContextRecord->Ecx,
		ExceptionInfo->ContextRecord->Esp,
		ExceptionInfo->ContextRecord->Ebp);

	::MessageBox(NULL, wchBuffer, L"崩溃了", MB_OK);
}

LONG WINAPI bad_exception(struct _EXCEPTION_POINTERS* ExceptionInfo)
{
	dbg_dump(ExceptionInfo);
	return true;
}

//#include "common/WJGameInfo.h"

int main(int argc, char *argv[])
{
	::SetUnhandledExceptionFilter(::bad_exception);

	OutputDebugStringA("sdp_test c main!");
	HANDLE hMutex = ::CreateMutex(NULL, TRUE, L"eventPLclent");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{//只运行开一个客户端
		//并且把先前的窗口显示出来
		HWND hWnd = ::FindWindowA(NULL, "犀牛电竞");
		if (nullptr != hWnd)
		{
			::PostMessage(hWnd, WM_HALL_MESS_1013, 0, 0);
		}
		return 0;
	}

	QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
	QApplication a(argc, argv);
	ZsXcbEventFilter* Filter = nullptr;
	try
	{
		Filter = new ZsXcbEventFilter;
	}
	catch (...)
	{
		WJ_SAFE_QBoxMess(WJMainWindow(), QStringLiteral("系统错误程序即将关闭！(1)"));
		return 0;
	}
	a.installNativeEventFilter(Filter);


	try
	{//实例化各种单例

		SystemLog()->initialize();	//初始化日志类
		CommonData();
		ClientLink();
		RoomLogic();
		WJ::PlatformLogic();
		WJMainWindow();
	}
	catch (...)
	{
		WJ_SAFE_QBoxMess(WJMainWindow(), QStringLiteral("系统错误程序即将关闭！(2)"));
		return 0;
	}

	/*
	//从文件加载光标图标(同时H5应该屏蔽光标改变)
	std::string s = GetAppPath();
	s += "Resource\\H5\\images\\cursor1.cur";
	HCURSOR   hcur = ::LoadCursorFromFileA(s.c_str());//加载光标
	::SetClassLong((HWND)WJMainWindow()->winId(), GCL_HCURSOR, (LONG)hcur);
	*/

	SystemLog()->LogToFile("犀牛平台开启!Log 1.2, for 平台异常退出. 2016.12.13 ");
	//监测版本
	OutputDebugStringA("sdp_test c main!监测版本");

	//#ifdef NDEBUG
	//	CommonData()->VersionUpdate();	//发布的时候把这个打开
	//#endif

	//设置配置文件文件夹
	QDir dir;
	if (!dir.exists(CommonData()->currentpath + QStringLiteral("/Config")))
	{
		dir.mkdir(CommonData()->currentpath + QStringLiteral("/Config"));
	}


	//获取所有游戏文件路径
	CommonData()->GetAllGameRoute();

	WebSocketServer::inobgect()->start();//websocket线程
	DataDrocessing::inobgect()->SetCallback(WebSocketServer::inobgect());
	WebSocketServer::inobgect()->setEstablishwebsoocket(WEB_SOCKET_PORT, DataDrocessing::inobgect());//创建websocket

	WJMainWindow()->show();/// 网页客户端

	//////////测试
	//CommonData()->m_NowGame = e_gameid_10000002;
	//IBaseMyGameRoom* pgr = CommonData()->GetMyGameRoom(e_gameid_10000002);
	//if (nullptr != pgr)
	//{
	//	pgr->OnStart();
	//}
	//////////

	///发送日活越
	//////////////////////////////////////////////////////////////////////////
	QEventLoop loop;
	QNetworkAccessManager m_networkAccessManger;
	QNetworkRequest request;

	QString str = Q_WEB_ROOT_ADDRESS + QStringLiteral("count/useractive?");
	request.setUrl(QUrl(str));

	SystemLog()->LogToFile("程序双击次数记录");

	//发送GET请求
	QNetworkReply *reply = m_networkAccessManger.get(request);
	QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));//关键之处1
	loop.exec();//关键之处2
	/////////////////////////////////////////////
	QByteArray data = reply->readAll();//网页内容输出
	QString ret = QString::fromUtf8(data);

	reply->deleteLater();

	//QWebEngineView *p = new QWebEngineView();
	//p->load(QUrl(CommonData()->currentpath + "/Resource/H5/eventHall.html"));
	//p->showFullScreen();
	return a.exec();
	
}