#include "imageidentification_cpu.h"
#include <windows.h>
#include "timemanager.h"

CPUImageIdentification::CPUImageIdentification()
{

}
bool CPUImageIdentification::initResourceImage(IndentificationMap& imagesMap)
{
	for (auto imageVecIt = imagesMap.begin(); imageVecIt != imagesMap.end(); imageVecIt++)
	{
		//读取指定要求识别的图片资源key vector
		string keyStr = imageVecIt->first;
		ImageFileVector& imageFileVec = imageVecIt->second;
		vector<ImageDescriptors*> imageDescVector;
		for (auto file : imageFileVec)
		{
            //【0】载入源图，显示并转化为灰度图
            Mat srcImage = imread(file, 0);
            if (srcImage.data == nullptr)
            {
               continue;
            }
            ImageDescriptors* pImageDescriptors = new CPUImageDescriptors(srcImage);
            if (pImageDescriptors != nullptr)
			{
                imageDescVector.push_back(pImageDescriptors);
			}
		}
		m_imageDescriptorsMap.insert(ImageDescriptorsMap::value_type(keyStr, imageDescVector));

	}
	return true;
}

bool CPUImageIdentification::initResourceLocateImage(IndentificationMap& imagesMap)
{
	for (auto imageVecIt = imagesMap.begin(); imageVecIt != imagesMap.end(); imageVecIt++)
	{
		//读取指定要求识别的图片资源key vector
		string keyStr = imageVecIt->first;
		ImageFileVector& imageFileVec = imageVecIt->second;
		vector<ImageDescriptors*> imageDescVector;
		for (auto file : imageFileVec)
		{
			//【0】载入源图，显示并转化为灰度图
			Mat srcImage = imread(file, 0);
			if (srcImage.data == nullptr)
			{
				continue;
			}
			ImageDescriptors* pImageDescriptors = new ImageLocate_CPU(srcImage);
			if (pImageDescriptors != nullptr)
			{
				imageDescVector.push_back(pImageDescriptors);
			}
		}
		m_imageLocateMap.insert(ImageDescriptorsMap::value_type(keyStr, imageDescVector));

	}
	return true;
}

ImageDescriptors* CPUImageIdentification::makeImageDescriptors(string& fileName)
{


	//【0】载入源图，显示并转化为灰度图
	Mat srcImage = imread(fileName, 0);

	if (srcImage.data == nullptr)
	{
		return nullptr;
	}
	ImageDescriptors* pImageDescriptors = new CPUImageDescriptors(srcImage);

	return pImageDescriptors;
}

char* CPUImageIdentification::shibie(void* pImage)
{
	Mat& imageDes = *(Mat*)pImage;

	CTimeManager::getInstance()->init_time_update();

	OrbFeatureDetector featureDetector;

	//【3】计算描述符（特征向量）
	OrbDescriptorExtractor featureExtractor;

	//【7】检测SIFT关键点并提取测试图像中的描述符
	vector<KeyPoint> captureKeyPoints;
	Mat captureDescriptionDes;

	//【8】调用detect函数检测出特征关键点，保存在vector容器中
	featureDetector.detect(imageDes, captureKeyPoints);

	//识别点小于5的放弃识别
	if (captureKeyPoints.size() <= 5)
		return 0;
	//【9】计算描述符
	featureExtractor.compute(imageDes, captureKeyPoints, captureDescriptionDes);

	//cout << "CPUImageIdentification::shibie time :" << CTimeManager::getInstance()->time_update() << endl;

	for (auto imageDesVec = m_imageDescriptorsMap.begin(); imageDesVec != m_imageDescriptorsMap.end(); imageDesVec++)
	{
		//读取指定要求识别的图片资源key vector
		string keyStr = imageDesVec->first;
		vector<ImageDescriptors*>& imageDescVector = imageDesVec->second;
		//int imageDescSize = imageDescVector.size();
		//int iCounts = 0;
		//cout << "key:" << keyStr << " ";
		for (auto pImageDescriptors : imageDescVector)
		{
			if (pImageDescriptors)
			{

				int pipeiSize = pImageDescriptors->ORBMatch(&captureDescriptionDes);
				//cout << "last time :" << CTimeManager::getInstance()->time_update() << "  pipei size" << pipeiSize << endl;
				if (pipeiSize > 50)
				{
					return (char*)imageDesVec->first.c_str();
				}
			}

		}
	}
	return nullptr;
}

int CPUImageIdentification::shibie(const char* _id, void* pImage)
{
	Mat& imageDes = *(Mat*)pImage;

	CTimeManager::getInstance()->init_time_update();

	OrbFeatureDetector featureDetector;

	//【3】计算描述符（特征向量）
	OrbDescriptorExtractor featureExtractor;

	//【7】检测SIFT关键点并提取测试图像中的描述符
	vector<KeyPoint> captureKeyPoints;
	Mat captureDescriptionDes;

	//【8】调用detect函数检测出特征关键点，保存在vector容器中
	featureDetector.detect(imageDes, captureKeyPoints);

	//识别点小于5的放弃识别
	if (captureKeyPoints.size() <= 5)
		return 0;
	//【9】计算描述符
	featureExtractor.compute(imageDes, captureKeyPoints, captureDescriptionDes);

	//cout << "CPUImageIdentification::shibie time :" << CTimeManager::getInstance()->time_update() << endl;

	int pipeiSize = 0;
	auto iter = m_imageDescriptorsMap.find(_id);
	if (iter != m_imageDescriptorsMap.end())
	{
		//cout << "KEY: " << _id << "    ";
		vector<ImageDescriptors*>& imageDescVector = iter->second;
		for (auto pImageDescriptors : imageDescVector)
		{
			if (pImageDescriptors)
			{
				pipeiSize = pImageDescriptors->ORBMatch(&captureDescriptionDes);
				cout << "PiPei度 [" << iter->first.c_str() << "]: " << pipeiSize << endl;
				//cout << "last time :" << CTimeManager::getInstance()->time_update() << "  pipei size :" << pipeiSize;
				//cout << " KEY: " << _id << "(" << pipeiSize << ")";
			}
		}
		//cout << "   =====> NO." << endl;
	}
	return pipeiSize;
}

Rect CPUImageIdentification::getImageLocation(string& tempName,void* pImage)
{
	auto iter = m_imageLocateMap.find(tempName);
	if (iter != m_imageLocateMap.end())
	{
		vector<ImageDescriptors*>& vecImgDesc = iter->second;
		for (auto imgDesc : vecImgDesc)
		{
			return imgDesc->getImageLocate(pImage);
		}
	}
	return Rect(0, 0, 0, 0);
}

Rect CPUImageIdentification::isFriendListOn(void* pImage)
{
    Mat& srcImage2 = *(Mat*)pImage;
    Rect rect(0,srcImage2.rows/2,srcImage2.rows/2,srcImage2.cols/2);

    //pipei

    Mat rioImg = srcImage2(rect);


    imshow("rioshow",rioImg);


    return Rect();
}
