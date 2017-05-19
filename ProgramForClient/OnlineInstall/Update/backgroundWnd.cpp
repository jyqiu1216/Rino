/*Copyright (c) 2012¡ª2015 Beijing TianRuiDiAn Network Technology Co,Ltd. All rights reserved*/
#include "stdafx.h"
#include "backgroundWnd.h"

CbackgroundWnd::CbackgroundWnd()
{

}

CbackgroundWnd::~CbackgroundWnd()
{
}

void CbackgroundWnd::InitWindow()
{
	m_pBtn_ok = static_cast<CFadeButtonUI*>(m_PaintManager.FindControl(_T("Btn_ok")));
	m_pBtn_cancel = static_cast<CFadeButtonUI*>(m_PaintManager.FindControl(_T("Btn_cancel")));
}

void CbackgroundWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		if (0 == _tcsicmp(_T("Btn_ok"), msg.pSender->GetName().GetData()))
		{
			ExitProcess(0);
		}
		else if (0 == _tcsicmp(_T("Btn_cancel"), msg.pSender->GetName().GetData()))
		{
			Close();
		}
	}

	__super::Notify(msg);
}