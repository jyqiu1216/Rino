#pragma once
#include "resource.h"
class CbackgroundWnd : public WindowImplBase
{
public:
	CbackgroundWnd();

	~CbackgroundWnd();

	virtual LPCTSTR GetWindowClassName() const
	{
		return _T("backgroundWnd");
	}

	virtual CDuiString GetSkinFile()
	{
		return _T("backgroundWnd.xml");
	}

	virtual CDuiString GetSkinFolder()
	{
		return  _T("skin\\");
	}

	virtual UILIB_RESOURCETYPE GetResourceType() const
	{
		return UILIB_ZIPRESOURCE;
		return UILIB_FILE;
	}

	virtual LPCTSTR GetResourceID() const
	{
		return MAKEINTRESOURCE(IDR_ZIPRES1);
		return _T("");
	}

	virtual void InitWindow();
	virtual void Notify(TNotifyUI& msg);

private:
	CFadeButtonUI* m_pBtn_ok;
	CFadeButtonUI* m_pBtn_cancel;
};