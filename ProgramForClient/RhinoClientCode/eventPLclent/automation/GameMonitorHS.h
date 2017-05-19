#ifndef _ImageRecognition_H_
#define _ImageRecognition_H_



#define WINDOW_TITLE_HS		"炉石传说"

#include <process.h>
#include <Windows.h>
#include <string>

#include "hook/ICapture.h"

#include "hook/IDataRecive.h"
#include "hook/EventSignal.h"
#include "hook/HookEventParam.h"

#include "identification/hs_identification.h"

#include "Monitor_Status_HS.h"

#include <vector>
class CLuaEngine;

typedef std::map<std::string, int>	MAP_StringInt;

class Callback_KnowHeroLS
{
public:
	//当知道了所选英雄时，进行回调
	virtual void onDoForKnowHeroLS(std::string Hero) = 0;
};

class GameMonitorHS : public IDataRecive, public CaptureCallback, public EventSignal
{
public:
	GameMonitorHS();
	~GameMonitorHS();

public:
	/*
	* \brief:	初始化
	* \param:	[_wsPort]: WEB SOCKET 端口
	* \param:	[_urlMatch]: H5 WEB url 地址1 : 匹配页
	* \param:	[_urlCover]: H5 WEB url 地址2 : 进度遮罩页
	*/
	bool initEmbedWinHS(int _wsPort, const char* _urlMatch, const char* _urlCover);

	bool initResource();

	/**
	* \brief:	数据更新, 返回当前的游戏状态
	*/
	EnGameStatusHS& update();

	void saveTempImg(const char* _name, int _val, Mat& _mat);

	bool getWindowMatHS(HWND hWnd, Mat& mat);
	//Mat* getRanderData();


	/**
	* \brief:	图像识别获取用户名称
	* \param:	[_name]: 用户名
	*/
	bool getPlayerName(HWND _hWnd, std::string& _name);

	/*
	* \brief:	图像对比，将指定图像和目标图像，目标图像为HOOK获取到的图像
	* \param:	[_image]:	要识别进行对比的图像
	* \return:	int: 图片的匹配度
	*/
	int imageContrast(Mat& mat, const char* _image, bool _saveImg = false);

	//bool imageContrastRect(const char* _image, float _x, float _y, float _w, float _h);
	int imageContrastRect(Mat& mat, const char* _image, Rect& _rt, bool _saveImg = false);
	//
	///*
	//* \brief:	图像识别，和预设置的所以图片进行对比识别
	//* \return:	匹配的图像的ID名称，null表示和所有图像都不匹配
	//*/
	//char* imageRecognition();


	/**
	* \brief:	文字识别
	*/
	std::string textRecognition(const char* _name, Mat& _mat);

	/**
	* \brief:	获取炉石传说的好友管理窗口区域
	*/
	Rect& getFriendWindowRect() { return m_rectFriendWindow; };
	bool getFriendRect(const char* _name, vector<Rect>& _vecRt);


	HWND getWindowRect(RECT& _rt)
	{
		HWND hWnd = ::FindWindowA(NULL, "炉石传说");
		if (!hWnd)
			return hWnd;

		::GetWindowRect(hWnd, &_rt);
		return hWnd;
	}


	bool textCompare(std::string& _text1, std::string& _text2);

	virtual void DoData(void* pData, size_t size);
	virtual void SendData(void* pData, size_t maxSize);
	virtual void* _callback(void* _data);

	//bool sendWebSocketMsg(const char* _msg);

	//virtual void onClientRecvMsg(const QString &_msg);
	//virtual void onSrvRecvMsg(const QString &_msg);
	//virtual void callbackConn(QWebSocket *_socket);


	void setGameStatus(const EnGameStatusHS& _status);

	EnGameStatusHS& getGameStatus();


	void setEvent(int _eventId) { m_eventId = _eventId; };
	int takeEvent()
	{
		int id = m_eventId;
		m_eventId = 0;
		return id;
	};



	HWND getGameWindowHwnd() { return ::FindWindowA(NULL, WINDOW_TITLE_HS); }

	void showFriendWindow(HWND _hWnd);

	void setGameWindwoSize(int _w, int _h);

	std::string getGameImage(Mat& mat, int& _size);

	void clickWindow(HWND _hWnd, float _x, float _y, int _sleep = 0);

	//初始化炉石识别英雄
	int InitKnowHeroLS(std::string &modelFile, std::string &deployFile, std::string &meanFile, std::string &labelFile);

	//设置炉石识别英雄的回调类
	void SetKnowHeroLSCallBack(Callback_KnowHeroLS* pCallback);
private:
	/**
	* \brief:	初始化D3D HOOK
	*/
	//bool initD3D();

	/**
	* \brief:	初始化共享纹理
	*/
	bool initShareTexture(CaptureInfo& _info);


	int getImgThreshold(const char* _name)
	{
		auto iter = m_mapImgThreshold.find(_name);
		if (m_mapImgThreshold.end() == iter)
			return -1;

		return iter->second;
	}


public:
	/**
	* \brief:	图像识别类
	*/
	CHSIdentification m_imageIdentifucation;

	
	//WebSocketServer* m_webSocketSrv;
	//QWebSocket* m_webSocketClient;
	std::string m_appPath;

	bool m_isVaild;

	/**
	* \brief:	游戏当前状态
	*/
	EnGameStatusHS m_gameStatus;

	int m_width;
	int m_height;

	Rect m_rectFriendWindow;


	HANDLE m_initThreadHandle;

	CShareMemory *m_shareMem;

	int m_eventId;


	CLuaEngine m_luaEngine;

	/* 图像对比阈值 */
	MAP_StringInt m_mapImgThreshold;


	vector<std::string> m_vecImg;
	bool m_binit_KnowHeroLS;
	//Mat* m_matTemp;

	Callback_KnowHeroLS* m_pCallback_KnowHeroLS;//炉石识别英雄的回调类
};
//
//
#endif // !_ImageRecognition_H_
