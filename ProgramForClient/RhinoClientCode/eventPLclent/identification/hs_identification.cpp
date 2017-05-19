
#ifdef WIN32
#include <windows.h>
#endif
#include "hs_identification.h"
#include "imageidentification_cpu.h"
#include "imageidentification_gpu.h"
#include "macrodefinition.h"
#include "imageidentification_cpu-flann.h"

#include <iostream>
#include <string.h>
using namespace std;


CHSIdentification::CHSIdentification() :m_imageIdentification(nullptr)
{

}
CHSIdentification::~CHSIdentification()
{

}


bool CHSIdentification::initImage(IndentificationMap& _vecImg, bool _isGpu)
{
	SafeReleasePtr(m_imageIdentification);
	if (_isGpu)
		m_imageIdentification = new GPUImageIdentification();
	else
		m_imageIdentification = new CPUImageIdentification();


	m_imageIdentificationflann = new CPUImgIdentify();

	m_imageIdentificationflann->initResourceImage(_vecImg);
	m_imageIdentification->initResourceImage(_vecImg);
	return true;
}

//bool CHSIdentification::init(const char* initFile, bool gpu)
//{
//	//获得当前dll的路径
//    if(!initFile)
//        return false;
//
//    if (!m_assetLuaEngine.loadLuaFile(initFile))
//		return false;
//
//	LuaDataMap* pLuaData = m_assetLuaEngine.getTable(HSLUA_IDENTIFICATION_KEY);
//
//	if (!pLuaData)
//		return false;
//
//    IndentificationMap indentifimapTemp;
//
//	//获取配置表
//	for (auto data = pLuaData->begin(); data != pLuaData->end(); data++)
//	{
//		
//		//LuaDataMap* 
//		
//		LuaDataMap* subData = data->second->value.value_map;
//		if (!subData)
//		{
//			cout << "LUA 数据解析错误" << endl;
//		}
//		else
//		{
//			string* name = ((*subData)["name"])->value.value_str;
//			string* imageFile = ((*subData)["file"])->value.value_str;
//			int threshold = ((*subData)["threshold"])->value.value_int;
//
//			ImageFileVector imageFileVector;
//			imageFileVector.push_back(imageFile->c_str());
//			indentifimapTemp.insert(IndentificationMap::value_type(name->c_str(), imageFileVector));
//		}
//
//
//
//		//if(!pSubData)
//		//	continue;
//  //      ImageFileVector imageFileVector;
//		//for (auto image = pSubData->begin(); image != pSubData->end(); image++)
//  //      {
//  //          string* imageFile = image->second->value.value_str;
//  //          imageFileVector.push_back(imageFile->c_str());
//		//}
//  //      //添加初始化记录
//  //      indentifimapTemp.insert(IndentificationMap::value_type(keyName,imageFileVector));
//	}
//	SafeReleasePtr(m_imageIdentification);
//	if (gpu)
//		m_imageIdentification = new GPUImageIdentification();
//	else
//		m_imageIdentification = new CPUImageIdentification();
//
//    if(!m_imageIdentification->initResourceImage(indentifimapTemp))
//        return false;
//
//	if (!m_imageIdentification->initResourceLocateImage(indentifimapTemp))
//		return false;
//
//	return true;
//}

//char* CHSIdentification::shibie(void* pImage, const char* _id)
// {
//	//if (_id)
//	//{
//	//	return m_imageIdentification->shibie(_id, pImage);
//	//}
//	//else
//	//{
//	//	return m_imageIdentification->shibie(pImage);
//	//}
// }

int CHSIdentification::shibie(void* pImage, const char* _id)
{
	return m_imageIdentification->shibie(_id, pImage);
}

int CHSIdentification::shibie_flann(void* pImage, const char* _id)
{
	return m_imageIdentificationflann->shibie(_id, pImage);
}

bool CHSIdentification::shibieLocation(const char* _id, void* pImage)
{
	std::string name = _id;
	Rect rt = m_imageIdentification->getImageLocation(name, pImage);
	
	Mat* matImg = (Mat*)pImage;
	int x = abs(matImg->cols - rt.width);
	int y = abs(matImg->rows - rt.height);

	if (x>=50 || y>=50)
	{
		return false;
	}
	else
	{
		return true;
	}
}





