//Copyright (c) 2012—2015 Beijing TianRuiDiAn Network Technology Co,Ltd. All rights reserved
#pragma once

#ifndef WJ_INTERFACE_FILE
#define WJ_INTERFACE_FILE

#include "../common/WJBaseType.h"
#include "../message/WJNetMessageHead.h"
/********************************************************************************************/

///服务接口定义

/********************************************************************************************/
namespace WJ
{
	class CTCPClientSocket;				///<SOCKET 类

	///服务器网络服务接口 
	class IServerSocketService
	{
		///接口函数
	public:
		//存在客户端连接成功
		virtual bool OnSocketConnectEvent_s(ULONG uAccessIP, UINT uIndex, UINT tag=0) = 0;
		///网络关闭处理
		virtual bool OnSocketCloseEvent_s(ULONG uAccessIP, UINT uIndex, UINT tag = 0) = 0;
		///网络消息处理
		virtual bool OnSocketReadEvent_s(ULONG uAccessIP, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uIndex, UINT tag = 0) = 0;
	};

	///客户端网络服务接口
	class IClientSocketService
	{
		///接口函数 
	public:
		///网络读取消息
		virtual bool OnSocketReadEvent(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket) = 0;
		///网络连接消息
		virtual bool OnSocketConnectEvent(UINT uErrorCode, CTCPClientSocket * pClientSocket) = 0;
		///网络关闭消息
		virtual bool OnSocketCloseEvent() = 0;
	};

	/////数据库结果服务接口
	//class IDataBaseResultService
	//{
	//	///接口函数
	//public:
	//	///数据库结果处理
	//	virtual bool OnDataBaseResultEvent(DataBaseResultLine * pResultData, UINT uHandleKind, UINT uHandleResult, UINT uResultSize, UINT uIndex)=0;
	//};
	//
	/////数据库处理服务接口
	//class IDataBaseHandleService
	//{
	//	///接口函数
	//public:
	//	///设置参数
	//	virtual bool SetParameter(IDataBaseResultService * pRusultService, CAFCDataBaseManage * pDataBaseManage, ManageInfoStruct * pInitData, KernelInfoStruct * pKernelData)=0;
	//	///数据库处理接口
	//	virtual UINT HandleDataBase(DataBaseLineHead * pSourceData)=0;
	//};
	//
}

#endif//WJ_INTERFACE_FILE