#ifndef _HSIDENTIFICATION_hsidentification_H_
#define _HSIDENTIFICATION_hsidentification_H_

#ifdef _DEBUG
#pragma comment(lib, "opencv_calib3d2413d.lib")
#pragma comment(lib, "opencv_contrib2413d.lib")
#pragma comment(lib, "opencv_core2413d.lib")
#pragma comment(lib, "opencv_gpu2413d.lib")
#pragma comment(lib, "opencv_highgui2413d.lib")
#pragma comment(lib, "opencv_imgproc2413d.lib")
#pragma comment(lib, "opencv_ml2413d.lib")
#pragma comment(lib, "opencv_ocl2413d.lib")
#pragma comment(lib, "opencv_features2d2413d.lib")
#pragma comment(lib, "opencv_superres2413d.lib")
#pragma comment(lib, "opencv_stitching2413d.lib")
#pragma comment(lib, "opencv_legacy2413d.lib")
#pragma comment(lib, "opencv_ts2413d.lib")
#pragma comment(lib, "opencv_flann2413d.lib")
#pragma comment(lib, "opencv_nonfree2413d.lib")
#else
#pragma comment(lib, "opencv_calib3d2413.lib")
#pragma comment(lib, "opencv_contrib2413.lib")
#pragma comment(lib, "opencv_core2413.lib")
#pragma comment(lib, "opencv_gpu2413.lib")
#pragma comment(lib, "opencv_highgui2413.lib")
#pragma comment(lib, "opencv_imgproc2413.lib")
#pragma comment(lib, "opencv_ml2413.lib")
#pragma comment(lib, "opencv_ocl2413.lib")
#pragma comment(lib, "opencv_features2d2413.lib")
#pragma comment(lib, "opencv_superres2413.lib")
#pragma comment(lib, "opencv_stitching2413.lib")
#pragma comment(lib, "opencv_legacy2413.lib")
#pragma comment(lib, "opencv_ts2413.lib")
#pragma comment(lib, "opencv_flann2413.lib")
#pragma comment(lib, "opencv_nonfree2413.lib")
#endif





#include "common/LuaEngine.h"
#include "imageidentification.h"
//炉石传说的识别封装类，本类进行资源初始化，基础配置管理初始化等
#define HSLUA_ASSETS_FILE	"hs.out"
#define HSLUA_IDENTIFICATION_KEY	"distinguish_map"

enum EHStatus{
    HS_MAINGUI,                 //主页面状态
    HS_FRIENDGUI,               //好友页面
    HS_FIGHT,                   //进入对战主窗口
    HS_CHANNENGE,               //进入随机主窗口
    HS_JINJI,                   //竞技主窗口
    HS_PLAY_BEGIN,              //比赛开始
    HS_WIN,                     //比赛胜利
    HS_LOSE                     //比赛失败
};


class CHSIdentification{

public:
    CHSIdentification();
    ~CHSIdentification();

	/**
	* @date: 2016/8/2
	* @author: Steven
	* @brief:   init   初始化信号管理类
	* @version: V1.0.0.1 
	* @return:  bool ;true 初始化ok; false 初始化失败
	**/
    //bool init(const char* initFile,bool gpu = false);

	bool initImage(IndentificationMap& _vecImg, bool _isGpu);

    /**
     * @brief shibie 炉石识别函数，系统将图像传递到本类，由本类记录当前系统状态
     * @param image 炉石传说界面对象
     * @return
     */
    //char* shibie(void* pImage, const char* _id);

	int shibie(void* pImage, const char* _id);

	bool shibieLocation(const char* _id, void* pImage);

	int shibie_flann(void* pImage, const char* _id);

protected:
	//lua资源类
	CLuaEngine m_assetLuaEngine;

	//模式识别类
    CImageIdentification* m_imageIdentification;

	CImageIdentification* m_imageIdentificationflann;

    //当前炉石被识别的状态
    EHStatus m_hscurStatus;

    //历史状态
    std::vector<EHStatus> m_historyStatus;

};


#endif // _HSIDENTIFICATION_hsidentification_H_

/**

1 bool isGPUOn = COpencvGC::getInstance()->init_gpu();

2 CHSIdentification hsIdentification;

3 hsIdentification.init("hs.out",isGPUOn);

**/