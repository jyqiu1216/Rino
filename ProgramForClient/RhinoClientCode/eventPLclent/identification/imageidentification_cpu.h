#ifndef CPUImageIdentification_H
#define CPUImageIdentification_H


#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/nonfree/nonfree.hpp>
#include<opencv2/legacy/legacy.hpp>
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include <opencv2/nonfree/features2d.hpp>

#include "imageidentification.h"
#include "imagelocate_cpu.h"
#include "opencv_gpu_cpu.h"
#include <iostream>
#include <string.h>
#include <vector>
#include <map>

using namespace std;
using namespace cv;


class CPUImageDescriptors :public ImageDescriptors
{
public:
	CPUImageDescriptors(Mat& src_image)
	{ 
        //orb_finder.blurForDescriptor = true;   //设置模糊
        cv::Mat fullmask_1(src_image.size(), CV_8U, 0xFF);                      //
        orb_finder(src_image, fullmask_1, keypoints_obj, descriptors_cpu_src);	//查找特征点
        source_image = src_image;
	}

	virtual int ORBMatch(void* _pMat)
	{
		if (!_pMat)
			return 0;

		cv::Mat& descriptors_cpu_dsc = *(cv::Mat*)_pMat;
		//匹配，有效的特征量
		matcher_lk.match(descriptors_cpu_src, descriptors_cpu_dsc, matches, cv::Mat());

		float max_distance = 60;    //定义特征点好坏衡量距离
		//std::vector<DMatch> good_matches;  //收集较好的匹配点
		int good_matches = 0;
		for (int i = 0; i < descriptors_cpu_src.rows; i++) {
			if (matches[i].distance < max_distance) {
				//good_matches.push_back(matches[i]);
				good_matches++;
			}
		}
		return good_matches;
	}

	virtual std::vector<DMatch> ORBMatch1(void* _pMat)
	{
		if (!_pMat)
			return std::vector<DMatch>();

		cv::Mat& descriptors_gpu_dsc = *(cv::Mat*)_pMat;
		//匹配，有效的特征量
		matcher_lk.match(descriptors_cpu_src, descriptors_gpu_dsc, matches, cv::Mat());

		float max_distance = 60;    //定义特征点好坏衡量距离
		std::vector<DMatch> good_matches;  //收集较好的匹配点

		for (int i = 0; i < descriptors_cpu_src.rows; i++) {
			if (matches[i].distance < max_distance) {
				good_matches.push_back(matches[i]);
			}
		}

		return good_matches;
	}
    virtual void* getDescriptors(){return &descriptors_cpu_src;}

protected:
    Mat source_image;

    cv::ORB orb_finder;	//orb finder对象
    cv::Mat descriptors_cpu_src;	//存储于CPU的图片特征描述信息,用来匹配系统图片资源
    cv::BruteForceMatcher< HammingLUT > matcher_lk; //cpu版本matcher


};
 

class CPUImageIdentification:public CImageIdentification
{
public:
    CPUImageIdentification();

    /**
     * @brief initResourceImage 初始化原始图片识别资源，系统必须进行调用初始化
     *
     * @return
     */
    virtual bool initResourceImage(IndentificationMap& imagesMap);
    virtual bool initResourceLocateImage(IndentificationMap& imagesMap);
    /**
     * @brief shibie    函数识别 程序获取图片之后主动发起调用，该函数将进行识别循环处理
     * @param image 图片对象，使用OPENCV的Mat格式
     * @return  null表示没有找到，否则返回被识别对象名称
     */
	virtual char* shibie(void* pImage);
	virtual int shibie(const char* _id, void* pImage);

    /**
      * @brief getImageLocation
      * @param tempName
      * @param pImage
      * @return
      */
     virtual Rect getImageLocation(string& tempName,void* pImage);

    /**
     * @brief isFriendListOn
     * @param pImage
     * @return
     */
    virtual Rect isFriendListOn(void* pImage);

    /**
     * @brief makeImageDescriptors 创建图片特征值，用于创建初始化
     * @param fileName
     * @return
     */
    ImageDescriptors* makeImageDescriptors(string& fileName);


};



#endif // CPUImageIdentification_H
