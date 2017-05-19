#ifndef _IMAGELOCATEGPU_imagelocategpu_H_
#define _IMAGELOCATEGPU_imagelocategpu_H_


#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/gpu/gpu.hpp>
#include <opencv2/nonfree/gpu.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include "opencv2/nonfree/nonfree.hpp"


#include <iostream>
#include <string.h>
#include <vector>
#include <map>
#include "timemanager.h"
#include "imageidentification.h"

using namespace std;
using namespace cv;


class ImageLocate_GPU:public ImageDescriptors
{
public:
    ImageLocate_GPU(Mat& _img);



    virtual std::vector<DMatch> flannBasedMatcher(void* _pMat);

    /**
     * @brief getImageLocate 从指定图片中获取指定模板的位置信息
     * @param _pMat         灰色检测图
     * @return
     */
    virtual Rect getImageLocate(void* _pMat);
    virtual Rect getImageLocate1(void* _pMat);
protected:
      gpu::GpuMat source_image;
      cv::Mat descriptors_cpu_src;	//存储于CPU的图片特征描述信息,用来匹配系统图片资源
};


#endif // _IMAGELOCATEGPU_imagelocategpu_H_
