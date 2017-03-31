//Copyright (c) 2012—2015 Beijing TianRuiDiAn Network Technology Co,Ltd. All rights reserved
#pragma once

#ifndef __WJ_PtrArray_H__
#define __WJ_PtrArray_H__
#include <mutex>
#include "common/WJCommonMarco.h"

namespace WJ
{
	///指针数据类（离散方式）
	template <class ClassType> class CWJPtrArray
	{
		///变量定义
	private:
		UINT						m_uActiveCount;						///当前有效数目
		UINT						m_uPermitCount;						///最大容纳数目
		ClassType					* * m_pActivePtr;					///有效索引
		ClassType					* * m_pSleepPtr;					///无效索引
		ClassType					* m_pClassArray;					///资料数组
		std::recursive_mutex					m_dataMutex;						//互斥量，方便线程对互斥量上锁
		///
		///Kylin 20090116 Vip登陆功能
		UINT						m_uVipReseverCount;						///Vip保留登陆数目
		///
		///函数定义
	public:
		///构造函数
		CWJPtrArray();
		///析构函数
		~CWJPtrArray();

		///功能函数
	public:
		///初始化
		bool Init(UINT uPermitCount,UINT uVipReseverCount);
		///卸载
		bool UnInit();
		///获取项（离散方式）
		ClassType * NewItem();
		///释放项（离散方式）
		bool FreeItem(ClassType * pItem);
		///获取项（顺序方式）
		ClassType * NewItem_Order(UINT uIndex);

		///辅助函数
	public:
		///获取有效数目
		UINT GetActiveCount() { return m_uActiveCount; };
		///获取容纳数目
		UINT GetPermitCount() { return m_uPermitCount; };
		///
		///Kylin 20090116 Vip登陆功能	
		UINT GetVipReseverCount() { return m_uVipReseverCount; };
		///
		///通过活动索引获取项（激活项）
		ClassType * GetActiveItem(UINT uIndex) { return *(m_pActivePtr+uIndex); };
		///通过索引获取项（内存项）
		ClassType * GetArrayItem(UINT uIndex) { return (m_pClassArray+uIndex); };
		///通过项获取索引（内存项）
		UINT GetItemIndex(ClassType * pFindItem) { return (UINT)(pFindItem-m_pClassArray); };
		///获取内存数组
		ClassType * GetClassArrayPtr() { return m_pClassArray; };
		///获取锁
		std::recursive_mutex *GetArrayLock() { return &m_dataMutex; }
	};

	///构造函数
	template <class ClassType> CWJPtrArray<ClassType>::CWJPtrArray()
	{
		m_uPermitCount=0;
		m_uActiveCount=0;
		m_pActivePtr = nullptr;
		m_pSleepPtr = nullptr;
		m_pClassArray = nullptr;
		return;
	}

	///析构函数
	template <class ClassType> CWJPtrArray<ClassType>::~CWJPtrArray()
	{
		UnInit();
		return;
	}

	///初始化函数
	///
	///Kylin 20090116 Vip登陆功能
	template<class ClassType> bool CWJPtrArray<ClassType>::Init(UINT uPermitCount, UINT uVipReseverCount)
	{
		try
		{
			///申请内存
			m_pClassArray=new ClassType [uPermitCount];
			m_pSleepPtr=new ClassType * [uPermitCount];
			m_pActivePtr=new ClassType * [uPermitCount];

			///效验错误
			if ((m_pClassArray == nullptr) || (m_pSleepPtr == nullptr) || (m_pActivePtr == nullptr))
			{
				UnInit();
				return false;
			}

			///设置参数
			memset(m_pActivePtr,0,sizeof(ClassType *)*uPermitCount);
			for (UINT i=0;i<uPermitCount;i++) *(m_pSleepPtr+i)=m_pClassArray+i;
			m_uActiveCount=0;
			m_uPermitCount=uPermitCount;
			m_uVipReseverCount=uVipReseverCount;
		
	///	
			return true;
		}
		catch (...) 
		{
			//TRACE("CATCH:%s with %s\n",__FILE__,__FUNCTION__);
			UnInit(); 
		}
	
		return false;
	}

	///卸载函数
	template<class ClassType> bool CWJPtrArray<ClassType>::UnInit()
	{
		WJ_SAFE_DELETE_ARRAY(m_pActivePtr);
		WJ_SAFE_DELETE_ARRAY(m_pSleepPtr);
		WJ_SAFE_DELETE_ARRAY(m_pClassArray);
		m_uPermitCount=0;
		m_uActiveCount=0;
		m_pActivePtr=nullptr;
		m_pSleepPtr=nullptr;

		return true;
	}

	///获取新项（离散方式）
	template<class ClassType> ClassType * CWJPtrArray<ClassType>::NewItem()
	{
		if (m_uActiveCount<m_uPermitCount)
		{
			*(m_pActivePtr+m_uActiveCount)=*m_pSleepPtr;
			MoveMemory(m_pSleepPtr,m_pSleepPtr+1,(m_uPermitCount-m_uActiveCount-1)*sizeof(ClassType *));
			*(m_pSleepPtr + m_uPermitCount - m_uActiveCount - 1) = nullptr;
			return *(m_pActivePtr+m_uActiveCount++);
		}
		return nullptr;
	}

	///释放项（离散方式）
	template<class ClassType> bool CWJPtrArray<ClassType>::FreeItem(ClassType * pItem)
	{
		if (pItem != nullptr)
		{
			for (UINT i=0;i<m_uActiveCount;i++)
			{
				if (*(m_pActivePtr+i)==pItem)
				{
					*(m_pSleepPtr+m_uPermitCount-m_uActiveCount)=*(m_pActivePtr+i);
					*(m_pActivePtr + i) = nullptr;
					*(m_pActivePtr+i)=*(m_pActivePtr+(--m_uActiveCount));
					return true;
				}
			}
		}
		return false;
	}
}

#endif//__WJ_PtrArray_H__