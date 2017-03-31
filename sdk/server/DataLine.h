#ifndef _WJ_DataLineHead_H_
#define _WJ_DataLineHead_H_

#include <mutex>
#include <list>
#include "common/WJBaseType.h"

namespace WJ
{
	//变量宏定义
#define LD_DEF_STEP				4096								//数据队列缓冲步长
#define LD_MAX_PART				8192								//数据包最大长度//3096 -> 1024*5 百家乐 modify by wlr 20090716
#define LD_MAX_DATA				650000								//数据队列最大长度

	
#ifndef STRUCT_DATALINEHEAD
#define STRUCT_DATALINEHEAD

	enum DataLine_UserDataTag
	{//底层操作几乎都是异步操作，但上层只可以是一个队列单线程操作，所以需要有一个队列衔接，
		//这一点用CDataLine已经实现了，但是一个服务器他可能存在多个实例的同一类底层处理，
		//这样都放到队列中去的时候如果没有标记进行区别是无法区分的。所以每一个队列节点都应该存在
		//使用者标记，以便上层业务区别对待。

		e_UserDataTag_def = 0,//主业务,默认处理
		e_UserDataTag_LGsocketclient = 1,//游戏服务器中,游戏服务器与登陆服务器通信
		e_UserDataTag_LGsocketserver = 2,//登陆服务器中,游戏服务器与登陆服务器通信
	};
	
	//数据队列信息头
	struct DataLineHead
	{
		UINT						uSize;								//数据大小
		UINT						uDataKind;							//数据类型
		DataLine_UserDataTag		tag;								//用户数据标记

		DataLineHead()
		{
			std::memset(this, 0, sizeof(DataLineHead));
		}
	};
#endif //STRUCT_DATALINEHEAD
	struct RedisDataBaseLine
	{
		DataLineHead					LineHead;					///队列头
		UINT							uRediscommandid;			///redis命令id
		UINT							uIndex;						///对象索引
		RedisDataBaseLine()
		{
			memset(this, 0, sizeof(RedisDataBaseLine));
		}
	};

	///redis结果消息结构定义
	struct RedisResultLine
	{
		DataLineHead						LineHead;					///队列头
		UINT								uHandleRusult;				///结果结果
		UINT								uRediscommandid;			///redis命令id
		UINT								uIndex;						///对象索引
	};

	/*
	Struct		:ListItemData
	Memo		:链表项数据结构
	Author		:Fred Huang
	Add Data	:2008-3-4
	Modify Data	:none
	Parameter	:
	stDataHead	:数据包头
	pData		:每个链表项数据的指针，使用用new方式申请的内存，注意，在出队列时，要显式的delete 该内存
	*/
	struct ListItemData
	{
		DataLineHead				stDataHead;
		BYTE						* pData;
	};
	//数据队列类
	class WJSERVERMANAGELIB_API CDataLine
	{
		//变量定义
	public:
		std::recursive_mutex m_dataMutex_CDataLine;						//互斥量，方便线程对互斥量上锁
	private:
		std::list<ListItemData*> m_DataList;
		//函数定义
	public:
		//构造函数
		CDataLine();
		//析构函数
		virtual ~CDataLine(void);

		//功能函数
	public:
		//清理所有数据
		bool CleanLineData();
		//加入消息队列
		virtual UINT AddData(DataLineHead * pDataInfo, UINT uAddSize, UINT uDataKind, void * pAppendData = nullptr, UINT uAppendAddSize = 0);
		//提取消息数据
		virtual UINT GetData(DataLineHead * pDataBuffer, UINT uBufferSize);
	public:
		int GetDataCount(void);
	};
}



#endif //_WJ_DataLineHead_H_