// MemeryShare.h
 
#ifndef _SHAREMEMORY_sharememory_H_
#define _SHAREMEMORY_sharememory_H_
#include <Windows.h>
//#include "base3.h"
#include "IDataRecive.h"
#include <string>
#include "macrodefinition.h"

#define MEMERY_DEFAULT_SIZE		(0xA00000)

#define SHAREMEMORY_DEFAULT_NAME	TEXT("ZSSHAREMEMORY")

 

class CShareMemory {
public:
	CShareMemory(IDataRecive* pDataRecive = nullptr,LPCTSTR shareMemoryName = SHAREMEMORY_DEFAULT_NAME);
	~CShareMemory();


	/**
	* @date: 2016/8/2
	* @author: Steven
	* @brief:   OpenShareFile 打开共享内存，如果共享内存不存在，则创建一个新的共享内存
	* @version: V1.0.0.1
	* @param:	LPCTSTR shareName 共享内存名称，建议使用GUII字符串编号
	* @return:  BOOL
	**/
	BOOL OpenShareFile(LPCTSTR shareName = nullptr);

	/**
	* @date: 2016/8/2
	* @author: Steven
	* @brief:   ReadFromShareMemory  从共享文件夹中读取信息，如果注册了回调函数，回调函数可以自行接收数据和处理
	* @version: V1.0.0.1
	* @param:	size_t size, 读取的内存大小
	* @param:	LPVOID pData, 共享内存中的数据存储的位置
	* @return:  BOOL
	**/ 

	BOOL ReadFromShareMemory(LPVOID pData,size_t size);

	/**
	* @date: 2016/8/2
	* @author: Steven
	* @brief:   ReadFromShareMemory  从共享文件夹中读取信息，如果注册了回调函数，回调函数可以自行接收数据和处理
	* @version: V1.0.0.1 
	* @param:	IDataRecive* pDataRecive, 传递用户回调接口，直接回调实现的数据结构
	* @return:  BOOL
	**/
	BOOL ReadFromShareMemory(IDataRecive* pDataRecive = nullptr);


	/**
	* @date: 2016/8/2
	* @author: Steven
	* @brief:   WriteShareMemory  windows系统创建共享文件资源，用户进程间快速通信
	* @version: V1.0.0.1
	* @param:	LPCTSTR shareName 名称，建议使用GUII字符串编号
	* @return:  BOOL
	**/
	BOOL WriteShareMemory(LPVOID pData, size_t size);
	BOOL WriteShareMemory();
protected:
	/**
	* @date: 2016/8/2
	* @author: Steven
	* @brief:   CreateShareFile  windows系统创建共享文件资源，用户进程间快速通信
	* @version: V1.0.0.1
	* @param:	LPCTSTR shareName 名称，建议使用GUII字符串编号
	* @return:  BOOL
	**/
	BOOL CreateShareFile(LPCTSTR shareName = nullptr);

	/**
	* @date: 2016/8/2
	* @author: Steven
	* @brief:   InitMutex  初始化线程锁
	* @version: V1.0.0.1
	* @param:	无
	* @return:  BOOL
	**/
	BOOL InitMutex();
protected:
	/************************************************************************/
	/* sharememory name                                                     */
	/************************************************************************/
	tstring	m_shareMemoryName;

	/************************************************************************/
	/* share memory handle                                            */
	/************************************************************************/
	 
	HANDLE m_shareMemoryHandle;
	/************************************************************************/
	/* mapped memory address                                            */
	/************************************************************************/
	LPVOID m_lpBuffer;

	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	//HANDLE m_hMutex;

	IDataRecive* m_pIDataRecive;
};



#endif  /*_SHAREMEMORY_sharememory_H_*/
