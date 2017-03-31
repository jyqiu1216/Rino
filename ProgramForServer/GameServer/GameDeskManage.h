#ifndef WJ_GAMEDESKMANAGE_H
#define WJ_GAMEDESKMANAGE_H

#include <mutex>
#include <vector>
#include "common/WJBaseType.h"
#include "server/comstruct.h"
#include "GameDesk.h"
namespace WJ
{
	class CGameDeskManage
	{
	private:
		std::vector<ISysDeskInterface*>			m_pDeskArray;		//游戏桌子列表
		HANDLE								m_hDllInstance;		//组件句柄
		Room_ManageInfoStruct*				m_gameinfo;			//游戏信息
	public:
		CGameDeskManage();
		~CGameDeskManage();

		///初始化游戏桌
		//uDeskCount 房间的桌子数
		//uDeskPeople 每一桌坐几个人
		bool InitGameDesk(UINT uDeskCount, UINT uDeskPeople, Room_ManageInfoStruct* pData, void* pDataManage);
		///获取游戏桌子信息
		void CreateDeskObject(UINT uInitDeskCount);
		//桌子数
		UINT GetCount();
		//返回具体一个桌子
		ISysDeskInterface* At(UINT index);
	};
}

#endif//WJ_GAMEDESKMANAGE_H