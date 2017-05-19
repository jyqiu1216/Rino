#ifndef _CaffeManager_H_
#define _CaffeManager_H_


#define CMD_TEXT		1
#define CMD_TEXT_FRIEND	2
#define CMD_IMGAGE_HS	3

#define CMD_END_THREAD	100

#include "caffe/CaffeRecognizer.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

#include <Windows.h>


/**
* \brief:	识别炉石英雄角色图像
*/
class RoleRecognizer
{
public:
	RoleRecognizer(ModelInfo& _modelInfo)
		: m_caffeRecognizer(nullptr)
	{
		m_caffeRecognizer = new CaffeRecognizer(_modelInfo);
	}

	bool getRoleType(Mat& _img, PredictionRes& _res)
	{
		if (!m_caffeRecognizer)
			return false;

		return m_caffeRecognizer->getClass(_img, _res);
	}

private:
	CaffeRecognizer *m_caffeRecognizer;
};



struct PredictionRes;
class RoleRecognizer;

class CaffeManager
{
public:
	CaffeManager();
	~CaffeManager();

public:
	bool init(ModelInfo& _modelInfo);
	bool initRoleRecognizer(ModelInfo& _modelInfo);

	void beginThread();
	void mainLoop();

	void setCmd(int _cmd) { m_cmd = _cmd; }

	bool recogizeWindowImage(const char* _windowTitle, cv::Mat &_img, PredictionRes &_res);

	bool getWindowMat(HWND _hWnd, cv::Mat& _img);
	void recogineHS_friend();
	void recognizeText();

	bool getRoleType(Mat& _img, PredictionRes& _res);


	bool recogineImage(PredictionRes &_res, cv::Mat &_img);

private:


private:
	int m_status;

	void *m_threadHandle;

	int m_cmd;


	CaffeRecognizer *m_caffeRecognizer;
	RoleRecognizer* m_roleRecognizer;
};



#endif // !_CaffeManager_H_
