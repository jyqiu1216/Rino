#pragma once
#include "resource.h"
class CBKWnd : public WindowImplBase
{
public:
	CBKWnd();

	~CBKWnd();

	virtual LPCTSTR GetWindowClassName() const
	{
		return _T("BKWnd");
	}

	virtual CDuiString GetSkinFile()
	{
		return _T("BKWnd.xml");
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

	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
};