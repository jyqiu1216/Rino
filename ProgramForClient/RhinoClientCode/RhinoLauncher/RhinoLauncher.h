#ifndef __WJ_RhinoLauncher_H__
#define __WJ_RhinoLauncher_H__
#include <string>
#include <windows.h>
DWORD FindProcess(char *strProcessName);
void KillProcess(char *strProcessName);

class CRhinoLauncher
{
public:
	CRhinoLauncher();
	~CRhinoLauncher();

	//监测版本是否可以更新
	bool VersionUpdate();
	//启动update.exe
	/*
	std::string version 主版本
	std::string smallversion 子版本
	std::string _filename 下载的文件
	std::string strDownLoadUpdatepADDR 下载的根地址
	std::string startgamename 启动的程序名称
	*/
	bool updateOnline(std::string version, std::string smallversion, std::string _filename, std::string strDownLoadUpdatepADDR, std::string startgamename);
	//获取http数据
	void GetHttpData(std::wstring str_web_addr, int iport, std::wstring str_web_content, std::string &str_res);
private:

};

#endif//__WJ_RhinoLauncher_H__