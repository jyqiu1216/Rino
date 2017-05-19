#include "imageidentification_gpu.h"
 

GPUImageIdentification::GPUImageIdentification()
{

}

bool GPUImageIdentification::initResourceImage(IndentificationMap& imagesMap)
{
	for (auto imageVecIt = imagesMap.begin(); imageVecIt != imagesMap.end(); imageVecIt++)
	{
		//读取指定要求识别的图片资源key vector
		string keyStr = imageVecIt->first;
		ImageFileVector& imageFileVec = imageVecIt->second;
		vector<ImageDescriptors*> imageDescVector;
		for (auto file : imageFileVec)
		{
			Mat imagesrc = imread(file, 0);
			if (!imagesrc.data)
				continue;
			ImageDescriptors* pTemp = new GPUImageDescriptors(imagesrc);
			if (pTemp != nullptr)
			{
				imageDescVector.push_back(pTemp);
			}
		}
		m_imageDescriptorsMap.insert(ImageDescriptorsMap::value_type(keyStr, imageDescVector));

	}
	return true;

}
 bool GPUImageIdentification::initResourceLocateImage(IndentificationMap& imagesMap)
 {
    return false;
 }


 char* GPUImageIdentification::shibie(void* pImage)
{
	//转为gpu mat
	gpu::GpuMat dst(*(Mat*)pImage);

	//创建
	gpu::GpuMat descriptors_gpu_dsc;
	std::vector<KeyPoint> keypoints_src;
	cv::gpu::GpuMat fullmask_1(dst.size(), CV_8U, 0xFF);

	gpu::ORB_GPU orb_finder(500);
	orb_finder.blurForDescriptor = true;   //设置模糊  
	orb_finder(dst, fullmask_1, keypoints_src, descriptors_gpu_dsc);


	for (auto imageVec = m_imageDescriptorsMap.begin(); imageVec != m_imageDescriptorsMap.end();imageVec++)
	{
		vector<ImageDescriptors*>& imageDescVector = (vector<ImageDescriptors*>)imageVec->second;
		
		for (auto gpuImageDes : imageDescVector)
		{
			int pipei = gpuImageDes->ORBMatch(&descriptors_gpu_dsc);
			if (pipei > 50)
			{
				return (char*)imageVec->first.c_str();
			}
		}
	}

	return nullptr;
}


int GPUImageIdentification::shibie(const char* _id, void* pImage)
{
	int size = 0;

	//转为gpu mat
	gpu::GpuMat dst(*(Mat*)pImage);

	//创建
	gpu::GpuMat descriptors_gpu_dsc;
	std::vector<KeyPoint> keypoints_src;
	cv::gpu::GpuMat fullmask_1(dst.size(), CV_8U, 0xFF);

	gpu::ORB_GPU orb_finder(500);
	orb_finder.blurForDescriptor = true;   //设置模糊  
	orb_finder(dst, fullmask_1, keypoints_src, descriptors_gpu_dsc);


	for (auto imageVec = m_imageDescriptorsMap.begin(); imageVec != m_imageDescriptorsMap.end(); imageVec++)
	{
		vector<ImageDescriptors*>& imageDescVector = (vector<ImageDescriptors*>)imageVec->second;

		for (auto gpuImageDes : imageDescVector)
		{
			int curSize = gpuImageDes->ORBMatch(&descriptors_gpu_dsc);
			if (curSize>size)
			{
				size = curSize;
			}
		}
	}

	return size;
}

 Rect GPUImageIdentification::getImageLocation(string& tempName,void* pImage)
 {


    return Rect();
 }

Rect GPUImageIdentification::isFriendListOn(void* pImage)
{

    return Rect();
}
