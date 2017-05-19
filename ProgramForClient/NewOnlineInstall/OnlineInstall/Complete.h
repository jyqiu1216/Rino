#pragma once

#ifndef COMPLETE_H
#define COMPLETE_H

class CComplete : public WindowImplBase {

private:
protected:
public:
	CComplete();
	~CComplete();

	//皮肤文件
	CDuiString																				GetSkinFile();
	//窗口类名称
	LPCTSTR																					GetWindowClassName() const;
	//初始化
	void																					InitWindow();
	//点击事件
	void																					Notify(TNotifyUI& msg);

	//启动客户端
	void																					StartClient();
};

#endif