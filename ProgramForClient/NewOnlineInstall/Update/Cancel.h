#pragma once

#ifndef CANCEL_H
#define CANCEL_H

class CCancel : public WindowImplBase {

private:
protected:
public:
	CCancel();
	~CCancel();

	//皮肤
	CDuiString																				GetSkinFile();
	//类名
	LPCTSTR																					GetWindowClassName() const;
	//初始化
	void																					InitWindow();
	//响应
	void																					Notify(TNotifyUI& msg);
};

#endif