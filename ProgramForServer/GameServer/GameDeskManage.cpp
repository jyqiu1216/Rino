
#include <windows.h>
#include "GameDeskManage.h"
#include "common/WJGameInfo.h"
#include "10000001_lol/server/MyGameDesk_10000001.h"
#include "10000002_hearth_stone/server/MyGameDesk_10000002.h"
#include "common/QkLogger.h"

namespace WJ
{
	//DLL 接口函数定义
	typedef ISysDeskInterface* (CreateServiceInterface)(void);
	
	CGameDeskManage::CGameDeskManage()
	{
		m_hDllInstance = nullptr;
		m_gameinfo = nullptr;
	}

	CGameDeskManage::~CGameDeskManage()
	{
		auto iter = m_pDeskArray.begin();
		while (iter != m_pDeskArray.end())
		{
			WJ_SAFE_DELETE((*iter));
			iter = m_pDeskArray.erase(iter);
		}
	}

	//初始化游戏桌
	bool CGameDeskManage::InitGameDesk(UINT uDeskCount, UINT uDeskPeople, Room_ManageInfoStruct* pData, void* pDataManage/* CGameRoom 's poiter */)
	{
		if (0 == uDeskCount || 0 == uDeskPeople || nullptr == pData)
		{
			return false;
		}
		m_gameinfo = pData;
		m_hDllInstance = pData->hDllInstance;
		//建立游戏桌子
		CreateDeskObject(uDeskCount);
		if ((0 == m_pDeskArray.size()))
		{
            QK_ErrorLog("CGameDeskManage::InitGameDesk  : 0 == m_pDeskArray.size()");
            return false;
        }
		//设置指针
		for (UINT i = 0; i < uDeskCount; ++i)
		{
			m_pDeskArray.at(i)->Init(i, uDeskPeople, pDataManage);
		}

		return true;
	}

	///获取游戏桌子信息
	void CGameDeskManage::CreateDeskObject(UINT uInitDeskCount)
	{
		//if (nullptr != m_hDllInstance)
		//{
		//	for (UINT i = 0; i < uInitDeskCount; ++i)
		//	{
		//		//获取管理接口
		//		CreateServiceInterface * pCreateServiceInterface = (CreateServiceInterface *)GetProcAddress((HMODULE)m_hDllInstance, "CreateServiceInterface");
		//		if (pCreateServiceInterface != nullptr)
		//		{
		//			ISysDeskInterface *p = (ISysDeskInterface *)pCreateServiceInterface();
		//			m_pDeskArray.push_back(p);
		//		}
		//	}
		//}

		for (UINT i = 0; i < uInitDeskCount; ++i)
		{
			CGameDesk *p = nullptr;
			switch (m_gameinfo->uGameID)
			{
			case e_gameid_10000001:{p = new WJ_gameid_10000001::CMyGameDesk; }break;
			case e_gameid_10000002:{p = new WJ_gameid_10000002::CMyGameDesk; }break;
			default:{return; }
			}
			m_pDeskArray.push_back(p);
		}

	}

	UINT CGameDeskManage::GetCount()
	{
		return (UINT)m_pDeskArray.size();
	}

	ISysDeskInterface* CGameDeskManage::At(UINT index)
	{
		if (index >= m_pDeskArray.size())
		{
			return nullptr;
		}

		return m_pDeskArray.at(index);
	}
}