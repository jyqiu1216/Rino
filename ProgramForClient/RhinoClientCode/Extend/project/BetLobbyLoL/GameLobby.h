#pragma once

#ifndef GAMELOBBY_H
#define GAMELOBBY_H

//游戏状态
typedef enum _State {

	//未知
	unknown,
	//进入选角色
	inselect,
	//从选角色返回
	breakslect

} State;

class CGameLobby {

private:
	//HOOK地址
	DWORD																												m_HookAddress;
	//HOOK处原始机器码
	byte																												m_HookOpCode[7];
protected:
	//发送数据到平台
	static void																											SendGameData(State pState, int inMode = 0);
	//搜索是否是人机模式
	static bool																											FindMode(int inStart, int inEnd, char* pchData, const char* pchTemp);
public:
	//hook跳转地址
	static DWORD																										m_JmpGameLobby;
public:
	CGameLobby();
	~CGameLobby();

	//过滤游戏Flash发包
	static void	__stdcall																								FilterGameLobby(char* pchBuffer, int inLen);
	//HOOK Flash发包
	void																												HookGameLobby();
	//UnHook Flash发包
	void																												UnHookGameLobby();
};

#endif