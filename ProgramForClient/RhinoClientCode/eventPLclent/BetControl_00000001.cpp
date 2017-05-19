#include "BetControl_00000001.h"
#include "web/WJMainWindow.h"
#include "../Extend/project/BetWindowLoL/betwindowlol.h"
#include "websocket/WebSocketMessInfo.h"

CBetControl_00000001::CBetControl_00000001()
{

}

CBetControl_00000001::~CBetControl_00000001()
{
}

bool CBetControl_00000001::BeginShowBet()
{
	bool bRet = false;

	try {

		//HWND hWnd = ::FindWindow(NULL, TEXT("League of Legends (TM) Client"));
		HWND hWnd = ::FindWindow(NULL, TEXT("PVP.net 客户端"));

		if (NULL != hWnd) 
		{
			if (CommonData()->m_BetInfo.size() > 0) 
			{
				if (::IsWindowVisible(hWnd)) {

					if (nullptr != WJMainWindow()->m_pBetWindowLoL) {

						WJMainWindow()->m_pBetWindowLoL->close();
						delete WJMainWindow()->m_pBetWindowLoL;
						WJMainWindow()->m_pBetWindowLoL = nullptr;
					}

					if ((nullptr != WJMainWindow()) && (NULL == WJMainWindow()->m_pBetWindowLoL))
					{ //LOL内嵌QT窗口
						QRect _rect(0, 0, 0, 0);
						WJMainWindow()->m_pBetWindowLoL = new BetWindowLoL(hWnd, (HWND)WJMainWindow()->winId(), _rect);

						WJMainWindow()->m_pBetWindowLoL->initialize(NULL, QUrl(""), WEB_SOCKET_PORT);
						WJMainWindow()->m_pBetWindowLoL->show();
					}
				}
			}
		}
	}
	catch (...) {

		bRet = false;
	}

	return bRet;
}

bool CBetControl_00000001::EndShowBet()
{
	bool bRet = false;

	try {

		if (CommonData()->m_BetInfo.size() > 0) {

			if (nullptr != WJMainWindow()->m_pBetWindowLoL) {

				WJMainWindow()->m_pBetWindowLoL->close();
				delete WJMainWindow()->m_pBetWindowLoL;
				WJMainWindow()->m_pBetWindowLoL = nullptr;
			}
		}
	}
	catch (...) {

		bRet = false;
	}

	return bRet;
}
