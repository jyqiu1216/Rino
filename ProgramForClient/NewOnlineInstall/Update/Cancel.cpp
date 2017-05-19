#include "stdafx.h"
#include "Cancel.h"
#include "DownloadZip.h"

//обть
extern CDownloadZip g_DownloadZip;

CCancel::CCancel()
{

}

CCancel::~CCancel()
{

}

DuiLib::CDuiString CCancel::GetSkinFile()
{
	return TEXT("Cancel.xml");
}

LPCTSTR CCancel::GetWindowClassName() const
{
	return TEXT("Update.Cancel");
}

void CCancel::InitWindow()
{

}

void CCancel::Notify(TNotifyUI& msg)
{
	try {

		if (msg.sType == TEXT("windowinit")) {

		}
		else if (msg.sType == TEXT("click")) {

			CDuiString CstrName = msg.pSender->GetName();

			if (CstrName == TEXT("BTN_OK")) {

				g_DownloadZip.CancelDownLoad();

				::PostQuitMessage(0);
			}
			else if (CstrName == TEXT("BTN_CANCEL")) {

				Close();
			}
		}
	}
	catch (...) {

	}


}
