/*Copyright (c) 2012—2015 Beijing TianRuiDiAn Network Technology Co,Ltd. All rights reserved*/
#include "stdafx.h"
#include "BKWnd.h"

//
BOOL LoadImageFromResource(CImage *pImage, UINT nResID,LPCTSTR lpTyp)
{
	if( pImage ==NULL)return false;

	pImage->Destroy();
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// 查找资源
	HRSRC hRsrc =::FindResource(nullptr/*AfxGetResourceHandle()*/, MAKEINTRESOURCE(nResID), lpTyp);
	if(hRsrc ==NULL)return false;

	// 加载资源
	HGLOBAL hImgData =::LoadResource(nullptr/*AfxGetResourceHandle()*/, hRsrc);
	if(hImgData ==NULL)
	{
		::FreeResource(hImgData);
		return false;
	}

	// 锁定内存中的指定资源
	LPVOID lpVoid    =::LockResource(hImgData);

	LPSTREAM pStream =NULL;
	DWORD dwSize    =::SizeofResource(nullptr/*AfxGetResourceHandle()*/, hRsrc);
	HGLOBAL hNew    =::GlobalAlloc(GHND, dwSize);
	LPBYTE lpByte    =(LPBYTE)::GlobalLock(hNew);
	::memcpy(lpByte, lpVoid, dwSize);

	// 解除内存中的指定资源
	::GlobalUnlock(hNew);

	// 从指定内存创建流对象
	HRESULT ht =::CreateStreamOnHGlobal(hNew,TRUE,&pStream);
	if( ht != S_OK )
	{
		GlobalFree(hNew);
	}
	else
	{
		// 加载图片
		pImage->Load(pStream);

		GlobalFree(hNew);
	}
	// 释放资源
	::FreeResource(hImgData);
	return true;
}

CBKWnd::CBKWnd()
{
}

CBKWnd::~CBKWnd()
{
}

LRESULT CBKWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_EXSTYLE);
	styleValue |= WS_EX_LAYERED;
	::SetWindowLong(*this, GWL_EXSTYLE, styleValue);

	CImage img;
	bool bres = LoadImageFromResource(&img, IDB_PNG1, TEXT("PNG"));
	if (!bres)
	{
		return 0;
	}
	SIZE sz;//图片大小
	sz.cx = img.GetWidth();
	sz.cy = img.GetHeight();

	SetWindowPos(m_hWnd, NULL, 0, 0, sz.cx, sz.cy, SWP_NOREDRAW);//将窗口大小设置为图片大小使之相互合适

	HDC hdc = GetDC(m_hWnd);//获取窗口设备句柄
	HDC hdcMem = CreateCompatibleDC(hdc);//创建一个与hdc相兼容的内存设备句柄
	HBITMAP    hBitmap = CreateCompatibleBitmap(hdc, sz.cx, sz.cy);
	SelectObject(hdcMem,(HGDIOBJ)hBitmap);
	img.Draw(hdcMem, 0, 0, sz.cx, sz.cy, 0, 0, sz.cx, sz.cy);

	POINT pt;
	pt.x = 0;
	pt.y = 0;

	BLENDFUNCTION  bf;
	bf.AlphaFormat = AC_SRC_ALPHA;
	bf.BlendFlags = 0;
	bf.BlendOp = AC_SRC_OVER;
	bf.SourceConstantAlpha = 255;

	UpdateLayeredWindow(m_hWnd, hdc, NULL, &sz, hdcMem, &pt, NULL, &bf, ULW_ALPHA);
	ReleaseDC(m_hWnd, hdc);

	return 0/*WindowImplBase::OnCreate(uMsg, wParam, lParam, bHandled)*/;
}

LRESULT CBKWnd::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = TRUE;
	//ExitProcess(0);
	return 0;
}