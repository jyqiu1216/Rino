// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 LOLEXTEND_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// LOLEXTEND_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef LOLEXTEND_EXPORTS
#define LOLEXTEND_API __declspec(dllexport)
#else
#define LOLEXTEND_API __declspec(dllimport)
#endif


#include <string>
#include "hook/GameInfo.h"
#include <windows.h>
#include <map>
#include <process.h>
class LOLEXTEND_API CLOLExtend
{
public:
	CLOLExtend(void);
	// TODO:  在此添加您的方法。

	/**
	* @date: 2016/5/13
	* @author: Steven
	* @func:   getUserInfo		从内存中查找指定的用户的信息
	* @version: V1.0.0.1
	* @param: std::string& imageFile	文件名称

	* @return: void
	**/
	GameInfoLOL& getUserInfo();
	void ExitGame();

public:
	void IsLOLExist();//检查LOL是否正常退出
	void CloseLol();//关闭卡主的lol
	DWORD GetGameKey(DWORD dwCall, int inOffset);//游戏编号
	HWND m_hWindow;	///窗口句柄
	
};

extern LOLEXTEND_API int nLOLExtend;

LOLEXTEND_API int fnLOLExtend(void);
