#pragma once

#ifndef BETCONTROL_00000001
#define BETCONTROL_00000001

#include <windows.h>

//LOL押注窗口控制
class CBetControl_00000001 {

private:
protected:
public:
	CBetControl_00000001();
	~CBetControl_00000001();

	//开始显示
	bool	BeginShowBet();

	//结束显示
	bool	EndShowBet();
};

#endif