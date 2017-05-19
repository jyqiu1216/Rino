#include "imageidentification_cpu-flann.h"
#include <windows.h>
#include "timemanager.h"

CPUImgIdentify::CPUImgIdentify()
{

}
bool CPUImgIdentify::initResourceImage(IndentificationMap& imagesMap)
{
	for (auto imageVecIt = imagesMap.begin(); imageVecIt != imagesMap.end(); imageVecIt++)
	{
		//读取指定要求识别的图片资源key vector
		string keyStr = imageVecIt->first;
		ImageFileVector& imageFileVec = imageVecIt->second;
		vector<ImageDescriptors*> imageDescVector;
		for (auto file : imageFileVec)
		{
			ImageDescriptors* pTemp = makeImageDescriptors(file);
			if (pTemp != nullptr)
			{
				imageDescVector.push_back(pTemp);
			}
		}
		m_imageDescriptorsMap.insert(ImageDescriptorsMap::value_type(keyStr, imageDescVector));

	}
	return true;

}
ImageDescriptors* CPUImgIdentify::makeImageDescriptors(string& fileName)
{
	//【0】载入源图，显示并转化为灰度图
	Mat srcImage = imread(fileName, 0);

	if (srcImage.data == nullptr)
	{
		return nullptr;
	}
	ImageDescriptors* pImageDescriptors = new ImageDescriptors();


	/*  Mat grayImage;
	*	cvtColor(srcImage, grayImage, CV_BGR2GRAY);
	*/
	//------------------检测SIFT特征点并在图像中提取物体的描述符----------------------

	//【1】参数定义
	OrbFeatureDetector featureDetector;


	//【2】调用detect函数检测出特征关键点，保存在vector容器中
	featureDetector.detect(srcImage, pImageDescriptors->keypoints_obj);

	//【3】计算描述符（特征向量）
	OrbDescriptorExtractor featureExtractor;
	featureExtractor.compute(srcImage, pImageDescriptors->keypoints_obj, pImageDescriptors->descriptors);

	//【4】基于FLANN的描述符对象匹配
   // flann::Index flannIndex(pImageDescriptors->descriptors, flann::LshIndexParams(12, 20, 2), cvflann::FLANN_DIST_HAMMING);

	pImageDescriptors->pFlannIndex = new flann::Index(pImageDescriptors->descriptors, flann::LshIndexParams(12, 20, 2), cvflann::FLANN_DIST_KL);
	return pImageDescriptors;
}

char* CPUImgIdentify::shibie(void* pImage)
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

	cout << "CPUImgIdentify::shibie time :" << CTimeManager::getInstance()->time_update() << endl;

	for (auto imageDesVec = m_imageDescriptorsMap.begin(); imageDesVec != m_imageDescriptorsMap.end(); imageDesVec++)
	{
		//读取指定要求识别的图片资源key vector
		string keyStr = imageDesVec->first;
		vector<ImageDescriptors*>& imageDescVector = imageDesVec->second;
		//int imageDescSize = imageDescVector.size();
		//int iCounts = 0;
		cout << "key:" << keyStr << " ";
		for (auto pImageDescriptors : imageDescVector)
		{
			if (pImageDescriptors)
			{
				int pipeiSize = matchByDescriptors(captureDescriptionDes, pImageDescriptors);
				cout << "last time :" << CTimeManager::getInstance()->time_update() << "  pipei size" << pipeiSize << endl;
				if (pipeiSize > 45)
				{
					return (char*)imageDesVec->first.c_str();
				}
			}
		}
	}
	return nullptr;
}


int CPUImgIdentify::shibie(const char* _id, void* pImage)
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

	//cout << "CPUImgIdentify::shibie time :" << CTimeManager::getInstance()->time_update() << endl;

	int pipeiSize = 0;
	auto iter = m_imageDescriptorsMap.find(_id);
	if (iter != m_imageDescriptorsMap.end())
	{
		//读取指定要求识别的图片资源key vector
		string keyStr = iter->first;
		vector<ImageDescriptors*>& imageDescVector = iter->second;
		//int imageDescSize = imageDescVector.size();
		//int iCounts = 0;
		for (auto pImageDescriptors : imageDescVector)
		{
			if (pImageDescriptors)
			{
				pipeiSize = matchByDescriptors(captureDescriptionDes, pImageDescriptors);
				//cout << "PiPei----[" << iter->first.c_str() << "]: " << pipeiSize << endl;
			}
		}
	}
	return pipeiSize;
}


int CPUImgIdentify::matchByDescriptors(Mat& captureDescriptionDes, ImageDescriptors* pImageDescriptors)
{
	//【10】匹配和测试描述符，获取两个最邻近的描述符
	Mat matchIndex(captureDescriptionDes.rows, 2, CV_32SC1), matchDistance(captureDescriptionDes.rows, 2, CV_32FC1);
	//cout << "matchByDescriptors begin :" << CTimeManager::getInstance()->time_update() << "  --- ";
	pImageDescriptors->pFlannIndex->knnSearch(captureDescriptionDes, matchIndex, matchDistance, 2, flann::SearchParams());//调用K邻近算法
	//cout << "matchByDescriptors end :" << CTimeManager::getInstance()->time_update() << "  --- ";
	//【11】根据劳氏算法（Lowe's algorithm）选出优秀的匹配
	vector<DMatch> goodMatches;
	for (int i = 0; i < matchDistance.rows; i++)
	{
		if (matchDistance.at<float>(i, 0) < 0.6 * matchDistance.at<float>(i, 1))
		{
			DMatch dmatches(i, matchIndex.at<int>(i, 0), matchDistance.at<float>(i, 0));
			goodMatches.push_back(dmatches);
		}
	}
	int iResult = goodMatches.size();
	return iResult;
}

int CPUImgIdentify::match(Mat& image, ImageDescriptors* pImageDescriptors)
{

	OrbFeatureDetector featureDetector;

	//【3】计算描述符（特征向量）
	OrbDescriptorExtractor featureExtractor;

	//【7】检测SIFT关键点并提取测试图像中的描述符
	vector<KeyPoint> captureKeyPoints;
	Mat captureDescription;

	//【8】调用detect函数检测出特征关键点，保存在vector容器中
	featureDetector.detect(image, captureKeyPoints);

	//识别点小于5的放弃识别
	if (captureKeyPoints.size() <= 5)
		return 0;
	//【9】计算描述符
	featureExtractor.compute(image, captureKeyPoints, captureDescription);

	//【10】匹配和测试描述符，获取两个最邻近的描述符
	Mat matchIndex(captureDescription.rows, 2, CV_32SC1), matchDistance(captureDescription.rows, 2, CV_32FC1);
	pImageDescriptors->pFlannIndex->knnSearch(captureDescription, matchIndex, matchDistance, 2, flann::SearchParams());//调用K邻近算法

	//【11】根据劳氏算法（Lowe's algorithm）选出优秀的匹配
	vector<DMatch> goodMatches;
	for (int i = 0; i < matchDistance.rows; i++)
	{
		if (matchDistance.at<float>(i, 0) < 0.6 * matchDistance.at<float>(i, 1))
		{
			DMatch dmatches(i, matchIndex.at<int>(i, 0), matchDistance.at<float>(i, 0));
			goodMatches.push_back(dmatches);
		}
	}
	int iResult = goodMatches.size();
	return iResult;
}
