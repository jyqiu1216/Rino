#include "stdafx.h"
#include <thread>
#include <functional>
#include <cassert>
#include <thread>
#include <chrono>
#include <stdio.h>
#include "Redis/WJRedis.h"
#include "common/QkLogger.h"

using namespace std;

#define MAX_REDIS_COMMAND_LEN  512

#define MAX_REDIS_CMD_QUEUE  100000
#define MAX_REDIS_CMD_RESULT_QUEUE  100000

namespace WJ
{
    /******************************** class CRedisWorkerThread **************************************************/

	bool CRedisWorkerThread::ConnectAndAuth()
	{
		if (m_bConnOk) return true;
		m_pConnect = redisConnect(m_host.c_str(), m_port);
		if (NULL == m_pConnect)
		{
			QK_ErrorLog("connect redis server %s:%d failed", m_host.c_str(), m_port);
			return false;
		}
		// auth
		redisReply * pReply = (redisReply*)redisCommand(m_pConnect, "AUTH %s", m_pass.c_str());
		if (nullptr == pReply)
		{//异常错误
			QK_ErrorLog("auth to redis server %s:%d exception, password=%s", m_host.c_str(), m_port, m_pass.c_str());
			return false;
		}
		if (!(pReply->type == REDIS_REPLY_STATUS && strcmp(pReply->str, "OK") == 0))
		{//执行结果错误
			QK_ErrorLog("auth to redis server %s:%d failed, password=%s", m_host.c_str(), m_port, m_pass.c_str());
			freeReplyObject(pReply);
            redisFree(m_pConnect);
            m_pConnect = NULL;
			return false;
		}
		m_bConnOk = true;
		QK_InfoLog("connect redis server %s:%d ok", m_host.c_str(), m_port);
		return true;
	}

	void CRedisWorkerThread::ReconnectDB()
	{
		QK_ErrorLog("Need reconnect db, reconnecting ...");
		m_bConnOk = false;
		redisFree(m_pConnect);
		m_pConnect = NULL;
		while (!m_pRedis->m_bStop && !ConnectAndAuth()) // 重连
		{
			Sleep(1000);
		}
	}

	bool CRedisWorkerThread::PushRedisCmd(const CRedisCmdBase *pCmd)
	{
		std::unique_lock<std::mutex> lck(m_cmdQueueMutex);
		if (m_cmdQueue.size() >= MAX_REDIS_CMD_QUEUE)
		{
			QK_ErrorLog("CRedisWorkerThread::PushRedisCmd m_cmdQueue.size() >= 10000");
			return false;
		}
		m_cmdQueue.push_back(pCmd);
		m_cmdQueueCV.notify_all();
		return true;
	}

	void CRedisWorkerThread::Run()
	{
		while (!m_pRedis->m_bStop && !ConnectAndAuth()) 
		{
			Sleep(1000);
		}
		deque<const CRedisCmdBase*> tasks;
		while (!m_pRedis->m_bStop)
		{
			std::unique_lock<std::mutex> lck(m_cmdQueueMutex);
			while (m_cmdQueue.empty())
			{
				if (m_pRedis->m_bStop)
				{
					return;
				}
				m_cmdQueueCV.wait_for(lck, chrono::seconds(1));
			}
			do
			{
				tasks.push_back(m_cmdQueue.front());
				m_cmdQueue.pop_front();
			} while (!m_cmdQueue.empty());
			lck.unlock();

			while (!tasks.empty())
			{
				const CRedisCmdBase *pCmd = tasks.front();
				tasks.pop_front();
				if (pCmd)
				{
					bool bEnqued = false;
					switch (pCmd->m_cmdType)
					{
						case E_GET:
						{
							CRedisCmdResult *pCmdResult = new CRedisCmdResult;
							assert(pCmdResult);
							pCmdResult->m_cmdType = pCmd->m_cmdType;
							pCmdResult->m_cmdId = pCmd->m_cmdId;
							pCmdResult->m_pResult = new vector<string>(1);
							assert(pCmdResult->m_pResult);
							get(((CRedisCmd*)pCmd)->m_key, pCmdResult->m_pResult->front());
							bEnqued = m_pRedis->PushDbResult(pCmdResult);
							if (!bEnqued)
							{
								delete pCmdResult;
								pCmdResult = NULL;
							}
							break;
						}
						case E_HGET:
						{
							CRedisCmdResult *pCmdResult = new CRedisCmdResult;
							assert(pCmdResult);
							pCmdResult->m_cmdType = pCmd->m_cmdType;
							pCmdResult->m_cmdId = pCmd->m_cmdId;
							pCmdResult->m_pResult = new vector<string>(1);
							assert(pCmdResult->m_pResult);
							hget(((CRedisCmd*)pCmd)->m_key, pCmdResult->m_pResult->front());
							bEnqued = m_pRedis->PushDbResult(pCmdResult);
							if (!bEnqued)
							{
								delete pCmdResult;
								pCmdResult = NULL;
							}
							break;
						}
						case E_HMGET:
						{
							CRedisCmdResult *pCmdResult = new CRedisCmdResult;
							assert(pCmdResult);
							pCmdResult->m_cmdType = pCmd->m_cmdType;
							pCmdResult->m_cmdId = pCmd->m_cmdId;
							pCmdResult->m_pResult = new vector<string>();
							assert(pCmdResult->m_pResult);
							hmget(((CRedisCmd*)pCmd)->m_key, *pCmdResult->m_pResult);
							bEnqued = m_pRedis->PushDbResult(pCmdResult);
							if (!bEnqued)
							{
								delete pCmdResult;
								pCmdResult = NULL;
							}
							break;
						}
						case E_ZRANGE:
						{
							CRedisCmdResult *pCmdResult = new CRedisCmdResult;
							assert(pCmdResult);
							pCmdResult->m_cmdType = pCmd->m_cmdType;
							pCmdResult->m_cmdId = pCmd->m_cmdId;
							pCmdResult->m_pResult = new vector<string>();
							assert(pCmdResult->m_pResult);
							zrange(((CRedisCmd*)pCmd)->m_key, *pCmdResult->m_pResult);
							bEnqued = m_pRedis->PushDbResult(pCmdResult);
							if (!bEnqued)
							{
								delete pCmdResult;
								pCmdResult = NULL;
							}
							break;
						}

					    ///////  wrtie etc.
						case E_SET:
						{
							set(((CRedisCmd*)pCmd)->m_key, ((CRedisCmd*)pCmd)->m_value);
							break;
						}
						case E_HSET:
						{
							hset(((CRedisCmd*)pCmd)->m_key, ((CRedisCmd*)pCmd)->m_value);
							break;
						}
						case E_HMSET:
						{
							hmset(((CRedisCmd*)pCmd)->m_key, ((CRedisCmd*)pCmd)->m_value);
							break;
						}
						case E_TRANSACTION:
						{
							CRedisTransaction *pTrans = (CRedisTransaction*)pCmd;
							CRedisTransactionResult *pTransResult = new CRedisTransactionResult;
							assert(pTransResult);
							pTransResult->m_cmdType = pTrans->m_cmdType;
							pTransResult->m_cmdId = pTrans->m_cmdId;
							pTransResult->m_pArg = pTrans->m_pArg;
							pTrans->m_pTransactionFunction(this, pTrans->m_pArg);
							bEnqued = m_pRedis->PushDbResult(pTransResult);
							if (!bEnqued)
							{
								delete pTransResult;
								pTransResult = NULL;
							}
							break;
						}
						default:
						{
							QK_ErrorLog("unknow redis cmd %d", pCmd->m_cmdType);
						}
					}
					delete pCmd;
					pCmd = NULL;
				}
			}
		}
	}
	
    void CRedisWorkerThread::get(const char *key, std::string &res)
    {
        char cmdBuf[MAX_REDIS_COMMAND_LEN];
        _snprintf_s(cmdBuf, sizeof(cmdBuf), "get %s", key);
        redisReply * pReply = (redisReply*)redisCommand(m_pConnect, cmdBuf);
		if (NULL == pReply)
		{//异常错误
			ReconnectDB();
			pReply = (redisReply*)redisCommand(m_pConnect, cmdBuf);
			assert(pReply);
		}
		if (pReply->type == REDIS_REPLY_STRING)
		{
            res = pReply->str;
		}
        else if (pReply->type == REDIS_REPLY_ERROR)
        {
			QK_WarnLog("CRedisWorkerThread::get redis cmd: %s  error:%s", cmdBuf, pReply->str);
        }
        else
        {
            QK_InfoLog("CRedisWorkerThread::get redis cmd: %s  unexpected data type", cmdBuf);
        }
		freeReplyObject(pReply);
    }

    void CRedisWorkerThread::hget(const char *key, std::string &res)
    {
        char cmdBuf[MAX_REDIS_COMMAND_LEN];
        _snprintf_s(cmdBuf, sizeof(cmdBuf), "hget %s", key);
        redisReply * pReply = (redisReply*)redisCommand(m_pConnect, cmdBuf);
		if (NULL == pReply)
		{//异常错误
			ReconnectDB();
			pReply = (redisReply*)redisCommand(m_pConnect, cmdBuf);
			assert(pReply);
		}
		if (pReply->type == REDIS_REPLY_STRING)
		{
            res = pReply->str;
		}
        else if (pReply->type == REDIS_REPLY_ERROR)
        {
            QK_WarnLog("CRedisWorkerThread::hget redis cmd: %s  error:%s", cmdBuf, pReply->str);
        }
        else
        {
			QK_InfoLog("CRedisWorkerThread::hget redis cmd: %s  unexpected data type", cmdBuf);
        }
		freeReplyObject(pReply);
    }

    void CRedisWorkerThread::hmget(const char *keyAndFields, std::vector<std::string> & res)
    {
        char cmdBuf[MAX_REDIS_COMMAND_LEN];
        _snprintf_s(cmdBuf, sizeof(cmdBuf), "hmget %s", keyAndFields);
        redisReply * pReply = (redisReply*)redisCommand(m_pConnect, cmdBuf);
		if (NULL == pReply)
		{//异常错误
			ReconnectDB();
			pReply = (redisReply*)redisCommand(m_pConnect, cmdBuf);
			assert(pReply);
		}

		if (pReply->type == REDIS_REPLY_ARRAY)
		{
            for (size_t i = 0; i < pReply->elements; ++i)
            {
                if (pReply->element[i]->type == REDIS_REPLY_STRING)
                {
                    res.push_back(pReply->element[i]->str);
                }
				else if (pReply->element[i]->type == REDIS_REPLY_NIL)
				{
					res.push_back("");
					QK_InfoLog("CRedisWorkerThread::hmget redis cmd: %s  unexpected data type NIL in reply array", cmdBuf);
				}
            }
		}
        else if (pReply->type == REDIS_REPLY_ERROR)
        {
			QK_WarnLog("CRedisWorkerThread::hmget redis cmd: %s  error:%s", cmdBuf, pReply->str);
        }
        else
        {
			QK_WarnLog("CRedisWorkerThread::hmget redis cmd: %s  unexpected data type", cmdBuf);
        }
		freeReplyObject(pReply);
    }

    void CRedisWorkerThread::zrange(const char *key, std::vector<std::string> & res)
    {
        char cmdBuf[MAX_REDIS_COMMAND_LEN];
        _snprintf_s(cmdBuf, sizeof(cmdBuf), "zrange %s 0 -1", key);
        redisReply * pReply = (redisReply*)redisCommand(m_pConnect, cmdBuf);
		if (NULL == pReply)
		{//异常错误
			ReconnectDB();
			pReply = (redisReply*)redisCommand(m_pConnect, cmdBuf);
			assert(pReply);
		}
		if (pReply->type == REDIS_REPLY_ARRAY)
		{
            for (size_t i = 0; i < pReply->elements; ++i)
            {
                if (pReply->element[i]->type == REDIS_REPLY_STRING)
                {
                    res.push_back(pReply->element[i]->str);
                }
				else if (pReply->element[i]->type == REDIS_REPLY_NIL)
				{
					res.push_back("");
					QK_InfoLog("CRedisWorkerThread::zrange redis cmd: %s  unexpected data type in reply array", cmdBuf);
				}
            }
		}
        else if (pReply->type == REDIS_REPLY_ERROR)
        {
			QK_WarnLog("CRedisWorkerThread::zrange redis cmd: %s  error:%s", cmdBuf, pReply->str);
        }
        else
        {
			QK_WarnLog("CRedisWorkerThread::zrange redis cmd: %s  unexpected data type", cmdBuf);
        }
		freeReplyObject(pReply);
    }

	void CRedisWorkerThread::rpop(const char *key, std::vector<std::string> &res)
	{
		res.clear();
		char cmdBuf[MAX_REDIS_COMMAND_LEN];
		_snprintf_s(cmdBuf, sizeof(cmdBuf), "RPOP %s", key);
		redisReply * pReply = nullptr;
		do
		{	
			pReply = (redisReply*)redisCommand(m_pConnect, cmdBuf);
			if (NULL == pReply)
			{//异常错误
				ReconnectDB();
				pReply = (redisReply*)redisCommand(m_pConnect, cmdBuf);
				assert(pReply);
				break;
			}
			else if (REDIS_REPLY_NIL == pReply->type)
			{
				break;
			}
			else
			{
				if (pReply->type == REDIS_REPLY_STRING)
				{
					res.push_back(pReply->str);
				}
			}
		} while (1);

		freeReplyObject(pReply);
	}

    void CRedisWorkerThread::set(const char *key, const char *value)
    {
        char cmdBuf[MAX_REDIS_COMMAND_LEN];
        _snprintf_s(cmdBuf, sizeof(cmdBuf), "set %s %s", key, value);
        redisReply * pReply = (redisReply*)redisCommand(m_pConnect, cmdBuf);
		if (NULL == pReply)
		{//异常错误
			ReconnectDB();
			pReply = (redisReply*)redisCommand(m_pConnect, cmdBuf);
			assert(pReply);
		}
		if (!(pReply->type == REDIS_REPLY_STATUS && strcmp(pReply->str, "OK") == 0))
		{
			QK_WarnLog("CRedisWorkerThread::set  !(pReply->type == REDIS_REPLY_STATUS && strcmp(pReply->str, \"OK\") == 0)");
		}
        else if (pReply->type == REDIS_REPLY_ERROR)
        {
			QK_WarnLog("CRedisWorkerThread::set redis cmd: %s  error:%s", cmdBuf, pReply->str);
        }
		freeReplyObject(pReply);
    }

    //value 形如: field value
    void CRedisWorkerThread::hset(const char *key, const char *value) 
    {
        char cmdBuf[MAX_REDIS_COMMAND_LEN];
        _snprintf_s(cmdBuf, sizeof(cmdBuf), "hset %s %s", key, value);
        redisReply * pReply = (redisReply*)redisCommand(m_pConnect, cmdBuf);
		if (NULL == pReply)
		{//异常错误
			ReconnectDB();
			pReply = (redisReply*)redisCommand(m_pConnect, cmdBuf);
			assert(pReply);
		}
		if (pReply->type != REDIS_REPLY_INTEGER)
		{
			QK_WarnLog("CRedisWorkerThread::hset pReply->type != REDIS_REPLY_INTEGER)");
		}
		if (pReply->type == REDIS_REPLY_ERROR)
        {
			QK_WarnLog("CRedisWorkerThread::hset redis cmd: %s  error:%s", cmdBuf, pReply->str);
        }
		freeReplyObject(pReply);
        
    }

    // value 形如： field1 value1 [ field2 value2 ... ]
    void CRedisWorkerThread::hmset(const char *key, const char *value)
    {
        char cmdBuf[MAX_REDIS_COMMAND_LEN];
        _snprintf_s(cmdBuf, sizeof(cmdBuf), "hmset %s %s", key, value);
        redisReply * pReply = (redisReply*)redisCommand(m_pConnect, cmdBuf);
		if (NULL == pReply)
		{//异常错误
			ReconnectDB();
			pReply = (redisReply*)redisCommand(m_pConnect, cmdBuf);
			assert(pReply);
		}
		if (!(pReply->type == REDIS_REPLY_STATUS && strcmp(pReply->str, "OK") == 0))
		{
			QK_WarnLog("CRedisWorkerThread::hmset  !(pReply->type == REDIS_REPLY_STATUS && strcmp(pReply->str, \"OK\") == 0)");
		}
        else if (pReply->type == REDIS_REPLY_ERROR)
        {
			QK_WarnLog("CRedisWorkerThread::hmset redis cmd: %s  error:%s", cmdBuf, pReply->str);
        }
		freeReplyObject(pReply);
    }

	void CRedisWorkerThread::lpush(const char *key, const char *value)
	{
		char cmdBuf[MAX_REDIS_COMMAND_LEN];
		_snprintf_s(cmdBuf, sizeof(cmdBuf), "lpush %s %s", key, value);
		redisReply * pReply = (redisReply*)redisCommand(m_pConnect, cmdBuf);
		if (NULL == pReply)
		{//异常错误
			ReconnectDB();
			pReply = (redisReply*)redisCommand(m_pConnect, cmdBuf);
			assert(pReply);
		}
		if (pReply->type != REDIS_REPLY_INTEGER)
		{
			QK_WarnLog("CRedisWorkerThread::lpush  pReply->type != REDIS_REPLY_INTEGER ");
		}
		else if (pReply->type == REDIS_REPLY_ERROR)
		{
			QK_WarnLog("CRedisWorkerThread::lpush redis cmd: %s  error:%s", cmdBuf, pReply->str);
		}
		freeReplyObject(pReply);
	}

    /******************************** class CWJRedis *************************************************************/

	CWJRedis::CWJRedis(const string &ip, const string &password, int port, UINT poolSize)
	{
		if (poolSize > 0 && poolSize <= E_REDIS_CONN_POOL_MAX)
		{
			m_poolSize = poolSize;
		}
		else
		{
			m_poolSize = 10;
		}
		m_host = ip;
		m_pass = password;
		m_port = port;
		m_pSyncRedis = NULL;
		m_bInited = false;
		m_bStop = false;
		Init();
		
	}

	void CWJRedis::Init()
    {
		if (m_bInited) return;
		//初始同步连接
		m_pSyncRedis = redisConnect(m_host.c_str(), m_port);
		if (NULL == m_pSyncRedis)
		{
			QK_ErrorLog("CWJRedis::Init : connect redis server %s:%d failed", m_host.c_str(), m_port);
			return;
		}
		// auth
		redisReply * pReply = (redisReply*)redisCommand(m_pSyncRedis, "AUTH %s", m_pass.c_str());
		
		if (nullptr == pReply)
		{//异常错误
			QK_ErrorLog("CWJRedis::Init : auth to redis server %s:%d exception, password=%s", m_host.c_str(), m_port, m_pass.c_str());
			return;
		}
		if (!(pReply->type == REDIS_REPLY_STATUS && strcmp(pReply->str, "OK") == 0))
		{//执行结果错误
			QK_ErrorLog("CWJRedis::Init : auth to redis server %s:%d failed, password=%s", m_host.c_str(), m_port, m_pass.c_str());
			freeReplyObject(pReply);
			redisFree(m_pSyncRedis);
			m_pSyncRedis = NULL;
			return;
		}
		//初始异步线程
		for (UINT i = 0; i < m_poolSize; ++i)
		{
			m_workers[i].m_pRedis = this;
			m_workers[i].m_host = m_host;
			m_workers[i].m_port = m_port;
			m_workers[i].m_pass = m_pass;
			m_workers[i].m_workerThread = thread(&CRedisWorkerThread::Run, &m_workers[i]);
		}
		m_bInited = true;
		QK_InfoLog("CWJRedis::Init m_host=%s,m_port=%d,m_pass=%s", m_host.c_str(), m_port, m_pass.c_str());
		return;
	}
	void CWJRedis::StopWorkers()
	{
		if (NULL != m_pSyncRedis)
		{
			redisFree(m_pSyncRedis);
			m_pSyncRedis = NULL;
		}
		m_bStop = true;
		for (UINT i = 0; i < m_poolSize; ++i)
		{
			m_workers[i].m_workerThread.join();
		}
		m_bInited = false;
	}

	void CWJRedis::get(UINT busid, UINT routeid, const char *key)
	{
		if (NULL == key) return;
		int index = SelectConnIndex(routeid);
		CRedisCmd *pCmd = new CRedisCmd(E_GET, busid, key, NULL);
		assert(pCmd);
		if (!m_workers[index].PushRedisCmd(pCmd))
		{
			delete pCmd;
		}
	}

	void CWJRedis::hget(UINT busid, UINT routeid, const char *keyAndField)
	{
		if (NULL == keyAndField) return;
		int index = SelectConnIndex(routeid);
		CRedisCmd *pCmd = new CRedisCmd(E_HGET, busid, keyAndField, NULL);
		assert(pCmd);
		if (!m_workers[index].PushRedisCmd(pCmd))
		{
			delete pCmd;
		}
	}

	void CWJRedis::hmget(UINT busid, UINT routeid, const char *keyAndFields)
	{
		if (NULL == keyAndFields) return;
		int index = SelectConnIndex(routeid);
		CRedisCmd *pCmd = new CRedisCmd(E_HMGET, busid, keyAndFields, NULL);
		assert(pCmd);
		if (!m_workers[index].PushRedisCmd(pCmd))
		{
			delete pCmd;
		}
	}
	void CWJRedis::zrange(UINT busid, UINT routeid, const char *key)
	{
		if (NULL == key) return;
		int index = SelectConnIndex(routeid);
		CRedisCmd *pCmd = new CRedisCmd(E_ZRANGE, busid, key, NULL);
		assert(pCmd);
		if (!m_workers[index].PushRedisCmd(pCmd))
		{
			delete pCmd;
		}
	}

	void CWJRedis::set(UINT busid, UINT routeid, const char *key, const char *value)
	{
		if (NULL == key || NULL == value) return;
		int index = SelectConnIndex(routeid);
		CRedisCmd *pCmd = new CRedisCmd(E_SET, busid, key, value);
		assert(pCmd);
		if (!m_workers[index].PushRedisCmd(pCmd))
		{
			delete pCmd;
		}
	}

	// fieldAndValue 形如: name ZhangSan
	void CWJRedis::hset(UINT busid, UINT routeid, const char *key, const char *fieldAndValue)
	{
		if (NULL == key || NULL == fieldAndValue) return;
		int index = SelectConnIndex(routeid);
		CRedisCmd *pCmd = new CRedisCmd(E_HSET, busid, key, fieldAndValue);
		assert(pCmd);
		if (!m_workers[index].PushRedisCmd(pCmd))
		{
			delete pCmd;
		}
	}

	// fieldAndValuePairs 形如： key1 value1 [ key2 value2 ... ]
	void CWJRedis::hmset(UINT busid, UINT routeid, const char *key, const char *fieldAndValuePairs)
	{
		if (NULL == key || NULL == fieldAndValuePairs) return;
		int index = SelectConnIndex(routeid);
		CRedisCmd *pCmd = new CRedisCmd(E_HMSET, busid, key, fieldAndValuePairs);
		assert(pCmd);
		if (!m_workers[index].PushRedisCmd(pCmd))
		{
			delete pCmd;
		}
	}

	void CWJRedis::do_transaction(UINT transactionId,
		UINT routeid,
		void(*pTransactionFunction)(CRedisWorkerThread *, void*),
		void *pTransactionObj)
	{
		if (pTransactionFunction == NULL || pTransactionObj == NULL)   return;
		int index = SelectConnIndex(routeid);
		assert(index >= 0 && index < (int)m_poolSize);
		CRedisTransaction *pTrans = new CRedisTransaction(E_TRANSACTION, transactionId, pTransactionFunction, pTransactionObj);
		assert(pTrans);
		if (!m_workers[index].PushRedisCmd(pTrans))
		{
			delete pTrans;
		}
	}

	void CWJRedis::sync_get(const char *key, std::string &res)
	{
		char cmdBuf[MAX_REDIS_COMMAND_LEN];
		_snprintf_s(cmdBuf, sizeof(cmdBuf), "get %s", key);
		redisReply * pReply = (redisReply*)redisCommand(m_pSyncRedis, cmdBuf);
		if (NULL == pReply)
		{//异常错误
			ReconnectDB();
			pReply = (redisReply*)redisCommand(m_pSyncRedis, cmdBuf);
			assert(pReply);
		}
		if (pReply->type == REDIS_REPLY_STRING)
		{
			res = pReply->str;
		}
		else if (pReply->type == REDIS_REPLY_ERROR)
		{
			QK_WarnLog("CWJRedis::sync_get redis cmd: %s  error:%s", cmdBuf, pReply->str);
		}
		else
		{
			QK_InfoLog("CWJRedis::sync_get redis cmd: %s  unexpected data type(%d)", cmdBuf, pReply->type);
		}
		freeReplyObject(pReply);
	}

	void CWJRedis::sync_hget(const char *key, std::string &res)
	{
		char cmdBuf[MAX_REDIS_COMMAND_LEN];
		_snprintf_s(cmdBuf, sizeof(cmdBuf), "hget %s", key);
		redisReply * pReply = (redisReply*)redisCommand(m_pSyncRedis, cmdBuf);
		if (NULL == pReply)
		{//异常错误
			ReconnectDB();
			pReply = (redisReply*)redisCommand(m_pSyncRedis, cmdBuf);
			assert(pReply);
		}
		if (pReply->type == REDIS_REPLY_STRING)
		{
			res = pReply->str;
		}
		else if (pReply->type == REDIS_REPLY_ERROR)
		{
			QK_WarnLog("CWJRedis::sync_hget redis cmd: %s  error:%s", cmdBuf, pReply->str);
		}
		else
		{
			QK_InfoLog("CWJRedis::sync_hget redis cmd: %s  unexpected data type(%d)", cmdBuf, pReply->type);
		}
		freeReplyObject(pReply);
	}

	void CWJRedis::sync_hmget(const char *keyAndFields, std::vector<std::string> & res)
	{
		char cmdBuf[MAX_REDIS_COMMAND_LEN];
		_snprintf_s(cmdBuf, sizeof(cmdBuf), "hmget %s", keyAndFields);
		redisReply * pReply = (redisReply*)redisCommand(m_pSyncRedis, cmdBuf);
		if (NULL == pReply)
		{//异常错误
			ReconnectDB();
			pReply = (redisReply*)redisCommand(m_pSyncRedis, cmdBuf);
			assert(pReply);
		}

		if (pReply->type == REDIS_REPLY_ARRAY)
		{
			for (size_t i = 0; i < pReply->elements; ++i)
			{
				if (pReply->element[i]->type == REDIS_REPLY_STRING)
				{
					res.push_back(pReply->element[i]->str);
				}
				else if (pReply->element[i]->type == REDIS_REPLY_NIL)
				{
					res.push_back("");
					QK_InfoLog("CWJRedis::sync_hmget redis cmd: %s  unexpected data type NIL in reply array, i=%d", cmdBuf, i);
				}
			}
		}
		else if (pReply->type == REDIS_REPLY_ERROR)
		{
			QK_WarnLog("CWJRedis::sync_hmget redis cmd: %s  error:%s", cmdBuf, pReply->str);
		}
		else
		{
			QK_WarnLog("CWJRedis::sync_hmget redis cmd: %s  unexpected data type(%d)", cmdBuf, pReply->type);
		}
		freeReplyObject(pReply);
	}

	void CWJRedis::sync_zrange(const char *key, std::vector<std::string> & res)
	{
		char cmdBuf[MAX_REDIS_COMMAND_LEN];
		_snprintf_s(cmdBuf, sizeof(cmdBuf), "zrange %s 0 -1", key);
		redisReply * pReply = (redisReply*)redisCommand(m_pSyncRedis, cmdBuf);
		if (NULL == pReply)
		{//异常错误
			ReconnectDB();
			pReply = (redisReply*)redisCommand(m_pSyncRedis, cmdBuf);
			assert(pReply);
		}
		if (pReply->type == REDIS_REPLY_ARRAY)
		{
			for (size_t i = 0; i < pReply->elements; ++i)
			{
				if (pReply->element[i]->type == REDIS_REPLY_STRING)
				{
					res.push_back(pReply->element[i]->str);
				}
				else if (pReply->element[i]->type == REDIS_REPLY_NIL)
				{
					res.push_back("");
					QK_InfoLog("CWJRedis::sync_zrange redis cmd: %s  unexpected data type NIL in reply array", cmdBuf);
				}
			}
		}
		else if (pReply->type == REDIS_REPLY_ERROR)
		{
			QK_WarnLog("CWJRedis::sync_zrange redis cmd: %s  error:%s", cmdBuf, pReply->str);
		}
		else
		{
			QK_WarnLog("CWJRedis::sync_zrange redis cmd: %s  unexpected data type(%d)", cmdBuf, pReply->type);
		}
		freeReplyObject(pReply);
	}

	void CWJRedis::sync_set(const char *key, const char *value)
	{
		char cmdBuf[MAX_REDIS_COMMAND_LEN];
		_snprintf_s(cmdBuf, sizeof(cmdBuf), "set %s %s", key, value);
		redisReply * pReply = (redisReply*)redisCommand(m_pSyncRedis, cmdBuf);
		if (NULL == pReply)
		{//异常错误
			ReconnectDB();
			pReply = (redisReply*)redisCommand(m_pSyncRedis, cmdBuf);
			assert(pReply);
		}
		if (!(pReply->type == REDIS_REPLY_STATUS && strcmp(pReply->str, "OK") == 0))
		{
			QK_WarnLog("CWJRedis::sync_set  !(pReply->type == REDIS_REPLY_STATUS && strcmp(pReply->str, \"OK\") == 0)");
		}
		else if (pReply->type == REDIS_REPLY_ERROR)
		{
			QK_WarnLog("CWJRedis::sync_set redis cmd: %s  error:%s", cmdBuf, pReply->str);
		}
		freeReplyObject(pReply);
	}

	//value 形如: field value
	void CWJRedis::sync_hset(const char *key, const char *value)
	{
		char cmdBuf[MAX_REDIS_COMMAND_LEN];
		_snprintf_s(cmdBuf, sizeof(cmdBuf), "hset %s %s", key, value);
		redisReply * pReply = (redisReply*)redisCommand(m_pSyncRedis, cmdBuf);
		if (NULL == pReply)
		{//异常错误
			ReconnectDB();
			pReply = (redisReply*)redisCommand(m_pSyncRedis, cmdBuf);
			assert(pReply);
		}
		if (pReply->type != REDIS_REPLY_INTEGER)
		{
			QK_WarnLog("CWJRedis::sync_hset pReply->type != REDIS_REPLY_INTEGER)");
		}
		if (pReply->type == REDIS_REPLY_ERROR)
		{
			QK_WarnLog("CWJRedis::sync_hset redis cmd: %s  error:%s", cmdBuf, pReply->str);
		}
		freeReplyObject(pReply);
	}

	// value 形如： field1 value1 [ field2 value2 ... ]
	void CWJRedis::sync_hmset(const char *key, const char *value)
	{
		char cmdBuf[MAX_REDIS_COMMAND_LEN];
		_snprintf_s(cmdBuf, sizeof(cmdBuf), "hmset %s %s", key, value);
		redisReply * pReply = (redisReply*)redisCommand(m_pSyncRedis, cmdBuf);
		if (NULL == pReply)
		{//异常错误
			ReconnectDB();
			pReply = (redisReply*)redisCommand(m_pSyncRedis, cmdBuf);
			assert(pReply);
		}
		if (!(pReply->type == REDIS_REPLY_STATUS && strcmp(pReply->str, "OK") == 0))
		{
			QK_WarnLog("CWJRedis::sync_hmset  !(pReply->type == REDIS_REPLY_STATUS && strcmp(pReply->str, \"OK\") == 0)");
		}
		else if (pReply->type == REDIS_REPLY_ERROR)
		{
			QK_WarnLog("CWJRedis::sync_hmset redis cmd: %s  error:%s", cmdBuf, pReply->str);
		}
		freeReplyObject(pReply);
	}

	//-----------------------
	bool CWJRedis::PushDbResult(const CRedisCmdResultBase *pDbResult)
    {
#if 0 // 条件变量的来实现, 阻塞方式
		std::unique_lock<std::mutex> lck(m_resultQueueMutex);
		if (m_resultQueue.size() >= MAX_REDIS_CMD_RESULT_QUEUE)
		{
			QK_ErrorLog("CWJRedis::PushDbResult  m_resultQueue.size() >= 50000");
			return false;
		}
        m_resultQueue.push_back(pDbResult);
        m_resultQueueCV.notify_all();
#else //不用条件变量， 非阻塞方式
        m_resultQueueMutex.lock();
		if (m_resultQueue.size() >= MAX_REDIS_CMD_RESULT_QUEUE)
		{
			QK_ErrorLog("CWJRedis::PushDbResult  m_resultQueue.size() >= 50000");
            m_resultQueueMutex.unlock();
			return false;
		}
        m_resultQueue.push_back(pDbResult);
        m_resultQueueMutex.unlock();
		return true;
#endif
    }

    void CWJRedis::ProcessDbResults(list<const CRedisCmdResultBase*> & results)
    {
        // 非阻塞方式取
		results.clear();
        m_resultQueueMutex.lock();
        while (!m_resultQueue.empty())
        {
            results.push_back(m_resultQueue.front());
            m_resultQueue.pop_front();
        }
        m_resultQueueMutex.unlock();
    }

	bool CWJRedis::ConnectAndAuth()
	{
		m_pSyncRedis = redisConnect(m_host.c_str(), m_port);
		if (NULL == m_pSyncRedis)
		{
			QK_ErrorLog("connect redis server %s:%d failed", m_host.c_str(), m_port);
			return false;
		}
		// auth
		redisReply * pReply = (redisReply*)redisCommand(m_pSyncRedis, "AUTH %s", m_pass.c_str());
		if (nullptr == pReply)
		{//异常错误
			QK_ErrorLog("auth to redis server %s:%d exception, password=%s", m_host.c_str(), m_port, m_pass.c_str());
			return false;
		}
		if (!(pReply->type == REDIS_REPLY_STATUS && strcmp(pReply->str, "OK") == 0))
		{//执行结果错误
			QK_ErrorLog("auth to redis server %s:%d failed, password=%s", m_host.c_str(), m_port, m_pass.c_str());
			freeReplyObject(pReply);
			redisFree(m_pSyncRedis);
			m_pSyncRedis = NULL;
			return false;
		}
		QK_InfoLog("connect redis server %s:%d ok", m_host.c_str(), m_port);
		return true;
	}

	void CWJRedis::ReconnectDB()
	{
		QK_ErrorLog("Need reconnect db, reconnecting ...");
		redisFree(m_pSyncRedis);
		m_pSyncRedis = NULL;
		while (!m_bStop && !ConnectAndAuth()) // 重连
		{
			Sleep(1000);
		}
	}
}

