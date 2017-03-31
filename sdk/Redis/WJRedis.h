#ifndef _WJ_WJRedis_H_
#define _WJ_WJRedis_H_

#include <vector>
#include <deque>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <string>
#include <atomic>
#include <list>
#include "common/WJBaseType.h"
#include "redis-2.6/deps/hiredis/hiredis.h"

namespace WJ
{
	//"118.178.25.88", 6380
	//"XnDj"
	//"username:startup"
	///redis数据包头结构

    enum RedisCmdType
    {
        E_NONE,

        E_GET,
        E_HGET,
        E_HMGET,
        E_ZRANGE,

        E_SET,
        E_HSET,
        E_HMSET,

		E_TRANSACTION  //事务，可以有个多个查询
    };

	class CWJRedis;
	class CRedisWorkerThread;

	class CRedisCmdBase
	{
	public:
		CRedisCmdBase() : m_cmdType(E_NONE), m_cmdId(0)
		{
		}
		CRedisCmdBase(RedisCmdType cmdType, UINT cmdId) : m_cmdType(cmdType), m_cmdId(cmdId)
		{

		}
		virtual ~CRedisCmdBase()
		{

		}
		RedisCmdType m_cmdType;
		UINT  m_cmdId;  // 业务id , 表示某个数据库查询或事务
	};

	class CRedisCmdResultBase
	{
	public:
		CRedisCmdResultBase() : m_cmdType(E_NONE), m_cmdId(0)
		{
		}
		CRedisCmdResultBase(RedisCmdType cmdType, UINT cmdId) : m_cmdType(cmdType), m_cmdId(cmdId)
		{

		}
		virtual ~CRedisCmdResultBase()
		{

		}
		RedisCmdType m_cmdType;
		UINT  m_cmdId;  // 业务id , 表示某个数据库查询或事务
	};

	class CRedisCmd : public CRedisCmdBase
    {
    public:
		CRedisCmd() : m_key(NULL), m_value(NULL)
        {
        }
		CRedisCmd(RedisCmdType cmdType, UINT busid, const char *key, const char*value) : CRedisCmdBase(cmdType, busid)
		{
			m_key = NULL;
			m_value = NULL;
			if (key)
			{
				int len = (int)strlen(key);
				m_key = new char[len + 1];
				assert(key);
				memcpy((void*)m_key, (void*)key, len + 1);
				
			}
			if (value)
			{
				int len = (int)strlen(value);
				m_value = new char[len + 1];
				assert(m_value);
				memcpy((void*)m_value, (void*)value, len + 1);
			}
		}
		virtual ~CRedisCmd()
		{
			if (m_key)
			{
				delete[] m_key;
				m_key = NULL;
			}
			if (m_value)
			{
				delete[] m_value;
				m_value = NULL;
			}
		}

        char *m_key;
        char *m_value;
    };

	class CRedisCmdResult : public CRedisCmdResultBase
    {
    public:
		CRedisCmdResult() : m_pResult(NULL)
        {
        }
        virtual ~CRedisCmdResult()
        {
            if (m_pResult)
            {
                delete m_pResult;
                m_pResult = NULL;
            }
        }

        std::vector<std::string> *m_pResult;
    };


	class CRedisTransaction : public CRedisCmdBase
	{
	public:
		CRedisTransaction() : m_pTransactionFunction(NULL), m_pArg(NULL)
		{

		}
		CRedisTransaction(RedisCmdType cmdType,
							UINT transId, 
							void(*pTransactionFunction)(CRedisWorkerThread *, void*), 
							void *pArg)
							: CRedisCmdBase(cmdType, transId)
		{
			m_pTransactionFunction = pTransactionFunction;
			m_pArg = pArg;
		}
		void(*m_pTransactionFunction)(CRedisWorkerThread *pDbWorker, void*);  //事务函数. 数据库worker 执行该事务函数，事务函数中可进行多个同步的数据库请求
		void *m_pArg; // 事务函数的参数，一般是一个对象，注意：客户代码递交了一个事务后，在事务结束前，不要操作该指针指向的内容，否则有线程安全问题
	};

	class CRedisTransactionResult : public CRedisCmdResultBase
	{
	public:
		CRedisTransactionResult() : m_pArg(NULL)
		{

		}
		void *m_pArg; //事务完成后传回对象
	};



    class CRedisWorkerThread
    {
		friend class CWJRedis;
    public:
		CRedisWorkerThread() : m_pRedis(NULL), m_port(0), m_bConnOk(false), m_pConnect(NULL)
        {
        }
		~CRedisWorkerThread()
		{
			if (m_pConnect)
			{
				redisFree(m_pConnect);
				m_pConnect = NULL;
			}
			m_bConnOk = false;
		}
		bool ConnectAndAuth();
		void ReconnectDB();
		bool PushRedisCmd(const CRedisCmdBase *pCmd); // called by orther thread
		void Run();
    public:
        // 读相关, 同步操作
		void get(const char *key, std::string &res);
		void hget(const char *key, std::string &res);
		void hmget(const char *keyAndFields, std::vector<std::string> & res);
		void zrange(const char *key, std::vector<std::string> & res);
		void rpop(const char *key, std::vector<std::string> &res);
        // 写相关, 同步操作
		void set(const char *key, const char *value);
		void hset(const char *key, const char *value);
		void hmset(const char *key, const char *value); //此时 value 的内容形如： field1 value1 [ field2 value2 ...]
		void lpush(const char *key, const char *value);
	private:
		CWJRedis * m_pRedis;
		std::string m_pass; //密码
		std::string m_host; //ip地址
		int m_port;  //端口号
		std::atomic<bool> m_bConnOk;
        redisContext *m_pConnect;
        std::thread   m_workerThread;
        std::deque<const CRedisCmdBase *> m_cmdQueue;
        std::mutex    m_cmdQueueMutex;
        std::condition_variable m_cmdQueueCV;
    };

	class /* WJSERVERMANAGELIB_API */ CWJRedis
	{
		friend class CRedisWorkerThread;
	public:
        enum 
        {
            E_REDIS_CONN_POOL_MAX = 20
        };
		CWJRedis(const std::string &ip, const std::string &password, int port, UINT poolSize);
		~CWJRedis()
		{
		}

	public:
        void Init();
        void StopWorkers();
		int SelectConnIndex(UINT routeid)
		{
			if (routeid == -1)
			{
				return rand() % m_poolSize;
			}
			return routeid % m_poolSize;
		}
		// 异步操作数据库
		// busid 业务id， routeid 选择db连接(保证数据一致性,例如同一用户的routeid应该相同)
		void get(UINT busid, UINT routeid, const char *key);
		void hget(UINT busid, UINT routeid, const char *keyAndField);
		void hmget(UINT busid, UINT routeid, const char *keyAndFields);
		void zrange(UINT busid, UINT routeid, const char *key);
		void set(UINT busid, UINT routeid, const char *key, const char *value);
		void hset(UINT busid, UINT routeid, const char *key, const char *fieldAndValue);
		void hmset(UINT busid, UINT routeid, const char *key, const char *fieldAndValuePairs);
		// 异步事务型操作（包括两个以上的数据库操作）
		void do_transaction(UINT transactionId, 
			UINT routeid, 
			void(*pTransactionFunction)(CRedisWorkerThread *, void*),
			void *pTransactionObj);
		// 同步操作数据库
		void sync_get(const char *key, std::string &res);
		void sync_hget(const char *key, std::string &res);
		void sync_hmget(const char *keyAndFields, std::vector<std::string> & res);
		void sync_zrange(const char *key, std::vector<std::string> & res);
		void sync_set(const char *key, const char *value);
		void sync_hset(const char *key, const char *value);
		void sync_hmset(const char *key, const char *value); //此时 value 的内容形如： field1 value1 [ field2 value2 ...]
    public:
		bool PushDbResult(const CRedisCmdResultBase *pDbResult); // called by workerthread 
		void ProcessDbResults(std::list<const CRedisCmdResultBase*> & results);
	private:
		bool ConnectAndAuth();
		void ReconnectDB();
	private:
		CRedisWorkerThread  m_workers[E_REDIS_CONN_POOL_MAX];//连接指针
        UINT          m_poolSize;
		std::string m_pass; //密码
		std::string m_host; //ip地址
		int m_port;  //端口号
		redisContext * m_pSyncRedis;   //同步db连接
		bool  m_bInited;
		std::atomic<bool> m_bStop;
		std::deque<const CRedisCmdResultBase*> m_resultQueue;
        std::mutex    m_resultQueueMutex;
        //std::condition_variable m_resultQueueCV;
	};
}

#endif//_WJ_WJRedis_H_