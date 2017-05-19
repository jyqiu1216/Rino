//
//  macrodefinition.h
//  MusicalNotation
//
//  Created by mac on 16-1-26.
//  Copyright (c) 2016年 zhengbensuyuan. All rights reserved.
//

#ifndef _MACRODEFINITION_macrodefinition_H_
#define _MACRODEFINITION_macrodefinition_H_

//安全释放指针对象
#define SafeReleasePtr_C(ptr) if(ptr){free(ptr); ptr = nullptr;}
  
//安全释放指针对象
#define SafeReleasePtr(ptr) if(ptr){delete ptr; ptr = nullptr;}
//安全释放数组对象
#define SafeReleasePtr8(ptr) if(ptr){delete []ptr; ptr = nullptr;}
//安全释放类 com的对象
#define SafeReleaseObject(object) if(object){ object->Release(); object = nullptr;}
//安全释放句柄的资源 主要用于类似windows环境中的资源句柄的释放
#define SafeReleaseHandle(handle) if(handle){CloseHandle(handle) ; handle = nullptr;}

#define arraySize(a) (sizeof(a)/sizeof(a[0]))     /* Returns (at compile time) the number of elements in an array */

#ifdef _WIN32
#include <Windows.h>
#include <string>
#define ThreadSleep(ms) Sleep(ms)
#else
#include "unistd.h"
#define ThreadSleep(ms) sleep(ms/1000)
#endif //_WIN32


#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif


#ifndef wchar_t
#ifdef _WIN32
//#define wchar_t unsigned short
#else
//#define wchar_t unsigned int
#endif
#endif

//值类型定义
typedef unsigned char     	BYTE;			// 1 byte
typedef char				CHAR;			// 1 byte
#ifndef	UCHAR
typedef unsigned char   	UCHAR; 		// 1 byte
#endif

typedef short				SHORT;			// 2 bytes
typedef unsigned short		USHORT;		// 2 btyes
typedef unsigned short    	WORD;			// 2 bytes
#ifndef DWORD
typedef unsigned long      	DWORD;		// 4 bytes
#endif

typedef int                 BOOL;			// 4 bytes
typedef int              	INT;			// 4 bytes
#ifndef UINT
typedef unsigned int       	UINT;			// 4 bytes
#endif
typedef long				LONG;			// 4 bytes or 8 bytes
typedef unsigned long		ULONG;			// 4 bytes or 8 bytes
typedef long long 			INT64;			// 8 bytes
typedef long long 			LONGLONG;		// 8 bytes
typedef unsigned long long 	UINT64;		// 8 bytes
typedef unsigned long long 	ULONGLONG;	// 8 bytes
typedef float               FLOAT;			// 4 btyes
typedef double				DOUBLE;		// 8 bytes
#ifndef LDOUBLE
typedef long double			LONGDOUBLE;		// 12 or 16 bytes
#endif

//pointer in 32 bit pc it's 4btyes ,and in 64 bit pc it's 8 bytes
typedef BYTE	*			PBYTE;
typedef CHAR	* 			PCHAR;
typedef UCHAR	*		PUCHAR;
typedef SHORT	*		PSHORT;
typedef USHORT	*		PUSHORT;
typedef WORD	*			PWORD;
typedef DWORD	*		PDWORD;
typedef BOOL	*			PBOOL;
typedef INT		*		PINT;
typedef LONG	*			PLONG;
typedef ULONG	*		PULONG;
typedef LONGLONG *		PLONGLONG;
typedef ULONGLONG*		PULONGLONG;
typedef UINT		*		PUINT;
typedef INT64	*		PINT64;
typedef UINT64	*		PUINT64;
typedef FLOAT	*		PFLOAT;
typedef DOUBLE	*		PDOUBLE;
typedef LONGDOUBLE	*	PLONGDOUBLE;
typedef   void	* 			PVOID;


#define ZZERO            0
typedef enum RESULT_BOOL
{
    RE_INIT = -3, //返回值为-3 表示未初始化
    RE_END = -2,
    RE_ERRFIRST = -1,
	RE_NULL_IN = 0,//返回值为0 表示传递空指针
	RE_NULL_OUT = 0,//返回为0表示过程中出现空指针异常
    RE_OK = 0, //返回值为0表示成功
    RE_FALSE, //返回值为1表示失败
    
    
} BResult,*PBResult;
typedef enum LUA_RESULT{
	LUA_ERROR_NULL = -1,//空指针异常
	LUA_ERROR_OK = 0,//返回正确
	LUA_ERROR_FALSE,//返回错误
	
}Lua_Result;

/*数字序列*/
typedef enum  NUMBER
{
    NUMDEFAULT= -1,   //NUMBER  -1
    NUMZERO,			 //NUMBER  0
    NUMONE,			 //NUMBER  1
    NUMTWO,			 //NUMBER  2
    NUMTHREE,		 //NUMBER  3
    NUMFOUR,               //NUMBER 4
    NUMFIVE,                //NUMBER 5
    NUMSIX,                  //NUMBER 6
    NUMSEVEN,              //NUMBER 7
    NUMEIGHT,             //NUMBER 8
    NUMNINE,               //NUMBER 9
    NUMTEN,                //NUMBER 10
    NUMSIXTEEN = 16,       //NUMBER 16
    NUMHUNDRED = 100,   //NUMBER 100
	NUM256 = 256,   //NUMBER 256
	NUM512 = 512,	//NUMBER 512
    NUMTHOUSAND    = 1000,     //NUMBER 1000
    NUMTWOK = 1024       //NUMBER 1024
} NumSeq;



//MACRO DEFINE
//check and free the memory Address malloced
#define FREE_CHECK(pAdd) \
{                           \
    if (NULL != pAdd)    \
    {                       \
        free(pAdd);      \
        pAdd = NULL;     \
    }                       \
}

#define DELETE_CHECK(temp)\
{							\
	if(NULL != temp){		\
		delete temp;		\
		temp = NULL;		\
	}						\
}
#define DELETEARRAY_CHECK(temp)\
{							\
	if(NULL != temp)			\
	{ 						\
		delete[] temp;		 \
		temp = NULL;			\
	}						\
}


#define DATABUF_LEN   8// NUM512
#define MACSTR_LEN	18 //60-EB-69-54-E7-94 该字符串占17个字节，默认第18个字节为NULL 这样保持字符为空的目的

//内存布局，内存区域按什么格式布局
union UData{
	PCHAR m_data_charptr;
	PUCHAR m_data_ucharptr;
	PINT m_data_intptr;
	PUINT m_data_uintptr;
	PFLOAT m_data_floatptr;
	PDOUBLE m_data_doubleptr;
};

//文件类型，用于不同的文件读取与处理，主要涉及的文件包括DEMO，位图，图形文件，压缩文件以及其他自定义文件
enum EFILETYPE{
	E_PNG,
	E_JPG,
	E_TGA
};
//----------------------------------------------------------------------------
//图像处理的错误类型定义
// Description: Constructor
//
// Use: public.
//
enum EImageError{
	EImage_OK = 0x00			//操作成功
	, EImage_False				//操作失败
	, EImage_File_NULL			//文件为空
	, EImage_File_Name_NULL		//文件名称为空
	, EImage_Memory				//内存错误
};

//像素格式 pixels
enum EPIXELTYPE{
	EPIXEL_U				//灰度图像 ，一个字节，每通道一个字节
	, EPIXEL_R				//红色通道 ， 一个字节，每通道一个字节
	, EPIXEL_G				//绿色通道 ， 一个字节，每通道一个字节
	, EPIXEL_B				//蓝色通道 ，  一个字节，每通道一个字节
	, EPIXEL_A				//aphle 通道，  一个字节，每通道一个字节
	, EPIXEL_RGB			// RGB 三通道  每通道一个字节
	, EPIXEL_RGBA			// RGBA 四通道 每通道一个字节
	, EPIXEL_BGR			// BGR 三通道  每通道一个字节
	, EPIXEL_BGRA			// BGRA 四通道 每通道一个字节
	, EPIXEL_RGB_F			// RGB 三通道  每通道一个float字节
	, EPIXEL_RGBA_F			// RGBA 四通道 每通道一个float字节
	, EPIXEL_BGR_F			// BGR 三通道  每通道一个float字节
	, EPIXEL_BGRA_F			// BGRA 四通道 每通道一个float字节
};

#ifndef PI
#define PI (3.14159265358979323846)

#endif



//内存池相关定义

#ifdef _USER_MEMORY_
#define USER_NEW new
#define USER_DELETE delete
#else
#define USER_NEW new
#define USER_DELETE delete
#define USER_DELETE_ARRAY delete []
#endif  /*_USER_MEMORY_*/


#ifdef UNICODE

#define tstring std::wstring

#else

#define tstring std::string

#endif

// PropertyBuilderByName 用于生成类的成员变量
// 并生成set和get方法
// type 为变量类型
// access_permission 为变量的访问权限(public, priavte, protected)

#define PropertyBuilderByName(type, name, access_permission)\
    access_permission:\
        type m_##name;\
    public:\
    inline void set##name(type v) {\
        m_##name = v;\
    }\
    inline type get##name() {\
        return m_##name;\
    }\

#endif
