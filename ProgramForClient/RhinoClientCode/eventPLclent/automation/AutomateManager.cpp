#include "AutomateManager.h"



void loginrst(const char* account, int resultcode)
{
	printf("login account:%s,resultcode:%d\n", account, resultcode);
}

void createroomrst(const char* roomname, const char* account, int resultcode)
{
	printf("createroom roomname:%s,account:%s,resultcode:%d\n", roomname, account, resultcode);
}

void joinroomrst(const char* roomname, const char* account, int resultcode)
{
	printf("createroom roomname:%s,account:%s,resultcode:%d\n", roomname, account, resultcode);
}

bool AutomateManager::execute(int _gameId, int _cmd, PFuncCallBack _pFunc)
{
	return true;
}


bool AutomateManager::init(const char* _luaFile)
{
	return WJAM::initLua(_luaFile);
}

int AutomateManager::executeLogin(WJAM::LOGININFO& _loginInfo, PFuncCallBack _pFunc)
{
	//WJAM::setCallback(_pFunc);
	return WJAM::login(&_loginInfo);
}

int AutomateManager::executeCreateRoom(WJAM::ROOMINFO& _roomInfo, PFuncCallBack _pFunc)
{
	//WJAM::setCallback(_pFunc);
	return WJAM::createRoom(&_roomInfo);
}

int AutomateManager::executeJoinRoom(WJAM::ROOMINFO& _roomInfo, PFuncCallBack _pFunc)
{
	//WJAM::setCallback(_pFunc);
	return WJAM::joinRoom(&_roomInfo);
}


#include "hook/ExtendGameManager.h"

int AutomateManager::executeStartGame(WJAM::GAME_ID _gameId, bool _isRoomOwner)
{
	int res = WJAM::startGame(_gameId, _isRoomOwner);
	if (WJAM_RES_OK == res)
	{
		ExtendGameManager* pEventMgr = new ExtendGameManager;
		pEventMgr->initEventSignal(EVENT_SERVER_DEFAULT_NAME, EVENT_CLIENT_DEFAULT_NAME, EVENT_EXIT_DEFAULT_NAME, pEventMgr);
		pEventMgr->StartThread();
	}

	return res;
}

//初始化炉石识别英雄
int AutomateManager::InitKnowHeroLS(std::string &modelFile, std::string &deployFile, std::string &meanFile, std::string &labelFile)
{
	return WJAM::InitKnowHeroLS(modelFile, deployFile, meanFile, labelFile);
}

//炉石识别英雄
void AutomateManager::SetKnowHeroLSCallBack(Callback_KnowHeroLS* pCallback)
{
	WJAM::SetKnowHeroLSCallBack(pCallback);
}