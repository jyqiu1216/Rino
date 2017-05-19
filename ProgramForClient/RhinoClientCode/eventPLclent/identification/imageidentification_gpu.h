#ifndef _IMAGEIDENTIFICATION_GPU_imageidentification_gpu_H_
#define _IMAGEIDENTIFICATION_GPU_imageidentification_gpu_H_

#include "imageidentification.h"


class GPUImageDescriptors :public ImageDescriptors
{ 
public:

	GPUImageDescriptors(gpu::GpuMat& src_gpu_image)
	{ 
        orb_finder.blurForDescriptor = true;   //设置模糊
		cv::gpu::GpuMat fullmask_1(src_gpu_image.size(), CV_8U, 0xFF);
        orb_finder(src_gpu_image, fullmask_1, keypoints_obj, descriptors_gpu_src);
        source_image = src_gpu_image;
	}

	GPUImageDescriptors(Mat& src_image)
    {
        gpu::GpuMat src_gpu_image(src_image);
		orb_finder.blurForDescriptor = true;   //设置模糊 
        std::vector<KeyPoint> keypoints_obj;
        cv::gpu::GpuMat fullmask_1(src_gpu_image.size(), CV_8U, 0xFF);		//
        orb_finder(src_gpu_image, fullmask_1, keypoints_obj, descriptors_gpu_src);	//查找特征点

        source_image = src_gpu_image;
	}

	virtual int ORBMatch(void* _pMat)
	{
		if (!_pMat)
			return 0;

		cv::gpu::GpuMat& descriptors_gpu_dsc = *(cv::gpu::GpuMat*)_pMat;
		//匹配，有效的特征量
		matcher_lk.match(descriptors_gpu_src, descriptors_gpu_dsc, matches, gpu::GpuMat());

		float max_distance = 60;    //定义特征点好坏衡量距离
		//std::vector<DMatch> good_matches;  //收集较好的匹配点
		int good_matches = 0;
		for (int i = 0; i < descriptors_gpu_src.rows; i++) {
			if (matches[i].distance < max_distance) {
				//good_matches.push_back(matches[i]);
				good_matches++;
			}
		}
		return good_matches; 
	}

	virtual ::vector<DMatch> ORBMatch1(void* _pMat)
	{
		if (!_pMat)
			return std::vector<DMatch>();

		cv::gpu::GpuMat& descriptors_gpu_dsc = *(cv::gpu::GpuMat*)_pMat;

		//匹配，有效的特征量
		matcher_lk.match(descriptors_gpu_src, descriptors_gpu_dsc, matches, gpu::GpuMat());

		float max_distance = 60;    //定义特征点好坏衡量距离
		std::vector<DMatch> good_matches;  //收集较好的匹配点
		
		for (int i = 0; i < descriptors_gpu_src.rows; i++) {
			if (matches[i].distance < max_distance) {
				good_matches.push_back(matches[i]);				
			}
		}
		return good_matches;
	}
      virtual void* getDescriptors(){return &descriptors_gpu_src;}

protected:
    gpu::GpuMat source_image;
	gpu::ORB_GPU orb_finder;	//orb finder对象
	gpu::GpuMat descriptors_gpu_src;	//存储于GPU的描述信息,用来匹配系统图片资源
	gpu::BruteForceMatcher_GPU< HammingLUT > matcher_lk; //gpu版本matcher 
    //std::vector<DMatch> matches;

};


class GPUImageIdentification :public CImageIdentification
{
public:
	GPUImageIdentification();

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
};



#endif // _IMAGEIDENTIFICATION_GPU_imageidentification_gpu_H_
