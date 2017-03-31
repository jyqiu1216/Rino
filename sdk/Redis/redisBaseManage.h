#ifndef WJ_RedisBaseManage_H
#define WJ_RedisBaseManage_H

#include "Redis/WJRedis.h"
#include "Redis/WJR_agreement_hall.h"

namespace WJ
{
	class InterfaceForWJRedisCallBack
	{//使用CWJRedis类时的回调函数都在此处
	public:
		///redis结果处理
		virtual bool OnRedisResultEvent(RedisResultLine* pResultData, UINT uResultSize) = 0;
	};

	class WJSERVERMANAGELIB_API CRedisBaseManage
	{
	private:
		const ManageInfoStruct* m_pInitData;//服务器参数
		InterfaceForWJRedisCallBack* m_pCallBack;//上层回调接口
	public:
		explicit CRedisBaseManage(const ManageInfoStruct *pInitData, InterfaceForWJRedisCallBack* pCallBack);
		~CRedisBaseManage();

		const ManageInfoStruct* getInitData(){ return m_pInitData; }
		InterfaceForWJRedisCallBack* getCallBack(){ return m_pCallBack; }

		///Redis命令处理接口
		virtual void RedisDataBase(CWJRedis* pCWJRedis, RedisDataBaseLine * pSourceData) = 0;

		///加入处理队列
		bool PushLine(void* pData, UINT uSize, UINT uIndex);
		///redis结果处理
		bool OnRedisResultEvent(RedisResultLine* pResultData, UINT uResultSize);
	};
}

#endif//WJ_RedisBaseManage_H
