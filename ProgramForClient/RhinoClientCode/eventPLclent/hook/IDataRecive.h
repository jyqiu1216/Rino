// MemeryShare.h
 
#ifndef _IDATARECIVE_idatarecive_H_
#define _IDATARECIVE_idatarecive_H_
 

class IDataRecive {
public:
	/**
	* @date: 2016/8/2
	* @author: Steven
	* @brief:   DoData  回调处理的接口，共享内存中读取数据进行处理和回调的接口函数
	* @version: V1.0.0.1
	* @param:
	* @return:  bool
	**/
	virtual void DoData(void* pData,size_t size) = 0;
	/**
	* @date: 2016/8/2
	* @author: Steven
	* @brief:   SendData   默认执行函数的函数约定
	* @version: V1.0.0.1
	* @param:
	* @return:  bool
	**/
	virtual void SendData(void* pData,size_t maxSize) = 0;
};
 

#endif  /*_IDATARECIVE_idatarecive_H_*/
