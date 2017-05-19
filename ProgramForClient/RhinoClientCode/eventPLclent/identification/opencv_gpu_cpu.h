#ifndef _OPENCV_GPU_CPU_opencv_gpu_cpu_H_
#define _OPENCV_GPU_CPU_opencv_gpu_cpu_H_


#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/gpu/gpu.hpp>
#include <opencv2/nonfree/gpu.hpp>
#include <opencv2/nonfree/features2d.hpp>


#include <iostream>
#include <string.h>
#include <vector>
#include <map>

using namespace std;
using namespace cv;

/************************************************************************/
/* 
系统CPU GPU可执行状态，用于OPENCV 的模式识别 
*/
/************************************************************************/

enum EGPUCPUStauts{
	EGPUCPU_Invalid = -1,
	EGPUCPU_CPU,
	EGPUCPU_GPU
};

class COpencvGC
{ 
	COpencvGC();
public:
	static COpencvGC* getInstance(){ static COpencvGC* p = new COpencvGC; return p; }
    /**
     * @brief init_gpu 初始化GPU
     * @return
     */
    bool init_gpu();
 
	//
	/**
	* @brief getOpencvGpuStatus 获得当前是否允许GPU加速
	* @return  EGPUCPU_GPU 表示GPU加速可用
	*/
	EGPUCPUStauts getOpencvGpuStatus(){ return m_opencvGC; }
protected:
 
	EGPUCPUStauts m_opencvGC;

  
};

#endif // _OPENCV_GPU_CPU_opencv_gpu_cpu_H_
