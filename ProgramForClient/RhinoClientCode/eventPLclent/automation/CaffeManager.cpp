
#include "caffe/CaffeRecognizer.h"
#include "CaffeManager.h"
//#include "RoleRecognizer.h"
//#include <qt_windows.h>

bool CaffeManager::getWindowMat(HWND _hWnd, Mat& _img)
{
	if (IsIconic(_hWnd)) // 窗口最小化时无法拿到DC画面
		return false;

	HDC hdc = ::GetDC(_hWnd);
	if (!hdc)
		return false;

	HDC hdcMem = ::CreateCompatibleDC(hdc);
	if (!hdcMem)
		return false;

	RECT rt;
	::GetClientRect(_hWnd, &rt);

	HBITMAP hbitmap = ::CreateCompatibleBitmap(hdc, rt.right - rt.left, rt.bottom - rt.top);
	if (!hbitmap)
		return false;

	::SelectObject(hdcMem, hbitmap);
	::PrintWindow(_hWnd, hdcMem, PW_CLIENTONLY);

	BITMAP bmp;
	GetObject(hbitmap, sizeof(BITMAP), &bmp);
	int nChannels = (bmp.bmBitsPixel == 1 ? 1 : bmp.bmBitsPixel / 8);
	int depth = bmp.bmBitsPixel == 1 ? IPL_DEPTH_1U : IPL_DEPTH_8U;

	_img.release();
	_img.create(cvSize(bmp.bmWidth, bmp.bmHeight), CV_MAKETYPE(CV_8U, nChannels));
	GetBitmapBits(hbitmap, bmp.bmHeight*bmp.bmWidth*nChannels, _img.data);
	
	//cv::rectangle(_img, Point(_img.cols*0.25, _img.rows*0.25), Point(_img.cols*0.75, _img.rows*0.75), Scalar())
	//imshow("HelloWorld", _img);
	//waitKey();

	::DeleteObject(hbitmap);
	::DeleteObject(hdcMem);
	::ReleaseDC(_hWnd, hdc);

	return true;
}


CaffeManager::CaffeManager()
	: m_status(0)
	, m_threadHandle(0)
	, m_cmd(0)
{
	m_cmd = CMD_IMGAGE_HS;
}

CaffeManager::~CaffeManager()
{
}


static unsigned int __stdcall threadProc(void *_p)
{
	CaffeManager *caffeMgr = (CaffeManager *)_p;
	if (caffeMgr)
	{
		//caffeMgr->mainLoop();
		Sleep(2000);
	}
	return 0;
}



#include <process.h>
void CaffeManager::beginThread()
{
	m_threadHandle = (HANDLE)_beginthreadex(nullptr, 0, threadProc, this, 0, nullptr);
}



void CaffeManager::mainLoop()
{
	Mat imgT;

	double tick = 0;
	do
	{
		tick = GetTickCount();
		if (CMD_END_THREAD == m_cmd)
			break;

		switch (m_cmd)
		{
		case CMD_TEXT: 
			recognizeText();
			break;

		case CMD_TEXT_FRIEND:
			recogineHS_friend();
			break;

		case CMD_IMGAGE_HS:
		{
			PredictionRes res;
			if (!recogizeWindowImage("", imgT, res))
			{
				cout << "ERROR: " << res.name.c_str() << endl;;
				Sleep(500);
			}
			else
			{

			}
			break;
		}
	
		default:
			break;
		}
		

		double t = GetTickCount() - tick - 100;
		if (t > 0)
		{
			Sleep(t);
		}

	} while (true);

		m_threadHandle = 0;
}


bool CaffeManager::init(ModelInfo& _modelInfo)
{
	m_caffeRecognizer = new CaffeRecognizer(_modelInfo);
	return true;
}

bool CaffeManager::initRoleRecognizer(ModelInfo& _modelInfo)
{
	m_roleRecognizer = new RoleRecognizer(_modelInfo);
	return true;
}


bool CaffeManager::recogizeWindowImage(const char* _windowTitle, cv::Mat &_img, PredictionRes &_res)
{
	static double s_tick = GetTickCount();

	HWND hWnd = ::FindWindowA(NULL, _windowTitle/*"炉石传说"*/);
	if (!hWnd)
	{
		_res.name = "窗口不存在";
		return false;
	}

	if (!getWindowMat(hWnd, _img))
	{
		_res.name = "获取窗口图像异常";
		return false;
	}
	
	return m_caffeRecognizer->getClass(_img, _res);
}


bool CaffeManager::recogineImage(PredictionRes &_res, cv::Mat &_img)
{
	return m_caffeRecognizer->getClass(_img, _res);
}

void CaffeManager::recogineHS_friend()
{
	HWND hWnd = ::FindWindow(NULL, TEXT("炉石传说"));
	if (!hWnd)
	{
		cout << endl << "炉石传说窗口不存在";
		return;
	}

	//RECT rtMainWin;
	//::GetWindowRect(hWnd, &rtMainWin);

	//Size winSize;
	//winSize.width = rtMainWin.right - rtMainWin.left;
	//winSize.height = rtMainWin.bottom - rtMainWin.top;


	Mat imgSrc;
	if (!getWindowMat(hWnd, imgSrc))
		return;

	imgSrc = imgSrc(Rect(0, 0, imgSrc.cols - 16, imgSrc.rows - 38));

	Rect rtWinFriend;
	/* 好友列表窗口 */
	rtWinFriend.x = 4;
	rtWinFriend.height = int(imgSrc.rows*0.672);
	rtWinFriend.y = int(imgSrc.rows - imgSrc.rows*0.052 - rtWinFriend.height);
	rtWinFriend.width = int(rtWinFriend.height*0.56);
	//imgSrc = imgSrc(rtWinFriend);
	imwrite("E:/TEMP/guiFriend.png", imgSrc(rtWinFriend));

	Rect rtTemp;
	rtTemp.x = rtWinFriend.x + rtWinFriend.width*0.06;
	rtTemp.y = rtWinFriend.y + rtWinFriend.width*0.25;
	rtTemp.width = rtWinFriend.width*0.65;
	rtTemp.height = rtWinFriend.height*0.65;

	imgSrc = imgSrc(rtTemp);
	cvtColor(imgSrc, imgSrc, CV_BGR2GRAY);
	imwrite("E:/TEMP/tempFriend.png", imgSrc);

	vector<Rect> rtTextLine;
	m_caffeRecognizer->splitTextLine(imgSrc, rtTextLine);

	for (int row = 0; row < rtTextLine.size(); row += 2)
	{
		Rect& rt = rtTextLine[row];

		char buf[128] = { 0 };
		m_caffeRecognizer->getText(imgSrc(rt));
	}

	imwrite("E:/TEMP/炉石传说-好友列表.png", imgSrc);
	waitKey(0);
}

#include "common/util.hpp"
void CaffeManager::recognizeText()
{
	std::string file;
	if (!Util::openFile(file))
		return;

	std::cout << " ------ 打开图片: " << file << std::endl;
	cv::Mat imgSrc = cv::imread(file, 0);
	if (imgSrc.empty())
	{
		cout << "Unable to decode image " << file << std::endl;
		return;
	}

	vector<Rect> rtTextLine;
	m_caffeRecognizer->splitTextLine(imgSrc, rtTextLine);

	for (int row = 0; row < rtTextLine.size(); row += 2)
	{
		Rect& rt = rtTextLine[row];

		char buf[128] = { 0 };
		m_caffeRecognizer->getText(imgSrc(rt));
	}

	waitKey(0);
}

bool CaffeManager::getRoleType(Mat& _img, PredictionRes& _res)
{
	if (!m_roleRecognizer)
		return false;

	return m_roleRecognizer->getRoleType(_img, _res);
}