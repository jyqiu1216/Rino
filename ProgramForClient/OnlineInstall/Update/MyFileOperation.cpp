/*Copyright (c) 2012—2015 Beijing TianRuiDiAn Network Technology Co,Ltd. All rights reserved*/
#include "stdafx.h"
#include "MyFileOperation.h"

#include<shellapi.h>

CMyFileOperation::CMyFileOperation()
{
}

CMyFileOperation::~CMyFileOperation()
{
}

/////////////////////////////////////
//函数名:DeleteFolder
//输入参数：LpszPath 要删除的路径指针
//作用：删除指定文件夹以及里面的文件
//
/////////////////////////////////////
BOOL CMyFileOperation::DeleteFolder(LPCTSTR lpszPath)
{
	int nLength = strlen(lpszPath);
	char *NewPath = new char[nLength+2];
	strcpy(NewPath,lpszPath);
	NewPath[nLength] = '\0';
	NewPath[nLength+1] = '\0';
	SHFILEOPSTRUCT FileOp;
	ZeroMemory((void*)&FileOp,sizeof(SHFILEOPSTRUCT));
	FileOp.fFlags = FOF_NOCONFIRMATION;
	FileOp.hNameMappings = NULL;
	FileOp.hwnd = NULL;
	FileOp.lpszProgressTitle = NULL;
	FileOp.pFrom = NewPath;
	FileOp.pTo = NULL;
	FileOp.wFunc = FO_DELETE;
	return SHFileOperation(&FileOp) == 0;
}

/////////////////////////////////////
//函数名：CopyFolder
//参数：lpszFromPath 源文件夹的路径 。 lpszToPath 目的文件夹的路径
//作用：拷贝文件夹及其文件夹中的所有内容
//
//////////////////////////////////////
BOOL CMyFileOperation::CopyFolder(LPCTSTR lpszFromPath,LPCTSTR lpszToPath)
{
	int nLengthFrm = strlen(lpszFromPath);
	char *NewPathFrm = new char[nLengthFrm+2];
	strcpy(NewPathFrm,lpszFromPath);
	NewPathFrm[nLengthFrm] = '\0';
	NewPathFrm[nLengthFrm+1] = '\0';
	SHFILEOPSTRUCT FileOp;
	ZeroMemory((void*)&FileOp,sizeof(SHFILEOPSTRUCT));
	FileOp.fFlags = FOF_NOCONFIRMATION ;
	FileOp.hNameMappings = NULL;
	FileOp.hwnd = NULL;
	FileOp.lpszProgressTitle = NULL;
	FileOp.pFrom = NewPathFrm;
	FileOp.pTo = lpszToPath;
	FileOp.wFunc = FO_COPY;
	return SHFileOperation(&FileOp) == 0;
}

/////////////////////////////////////
//函数名：MoveFolder
//参数：lpszFromPath 源文件夹路径 。lpszToPath 目的文件夹路径
//作用：移动原文件夹及其中文件都指定的路径下
//
/////////////////////////////////////
BOOL CMyFileOperation::MoveFolder(LPCTSTR lpszFromPath,LPCTSTR lpszToPath)
{
	int nLengthFrm = strlen(lpszFromPath);
	char *NewPathFrm = new char[nLengthFrm+2];
	strcpy(NewPathFrm,lpszFromPath);
	NewPathFrm[nLengthFrm] = '\0';
	NewPathFrm[nLengthFrm+1] = '\0';
	SHFILEOPSTRUCT FileOp;
	ZeroMemory((void*)&FileOp,sizeof(SHFILEOPSTRUCT));
	FileOp.fFlags = FOF_NOCONFIRMATION ;
	FileOp.hNameMappings = NULL;
	FileOp.hwnd = NULL;
	FileOp.lpszProgressTitle = NULL;
	FileOp.pFrom = NewPathFrm;
	FileOp.pTo = lpszToPath;
	FileOp.wFunc = FO_MOVE;
	return SHFileOperation(&FileOp) == 0;
}

/////////////////////////////////////
//ReNameFolder
//参数：lpszFromPath 源文件夹路径 。lpszToPath 目的文件夹路径
//作用：修改原文件夹的名字。
//
/////////////////////////////////////
BOOL CMyFileOperation::ReNameFolder(LPCTSTR lpszFromPath,LPCTSTR lpszToPath)
{
	int nLengthFrm = strlen(lpszFromPath);
	char *NewPathFrm = new char[nLengthFrm+2];
	strcpy(NewPathFrm,lpszFromPath);
	NewPathFrm[nLengthFrm] = '\0';
	NewPathFrm[nLengthFrm+1] = '\0';
	SHFILEOPSTRUCT FileOp;
	ZeroMemory((void*)&FileOp,sizeof(SHFILEOPSTRUCT));
	FileOp.fFlags = FOF_NOCONFIRMATION ;
	FileOp.hNameMappings = NULL;
	FileOp.hwnd = NULL;
	FileOp.lpszProgressTitle = NULL;
	FileOp.pFrom = NewPathFrm;
	FileOp.pTo = lpszToPath;
	FileOp.wFunc = FO_RENAME;
	return SHFileOperation(&FileOp) == 0;
}