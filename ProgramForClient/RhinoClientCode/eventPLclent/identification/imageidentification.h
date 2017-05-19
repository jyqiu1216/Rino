#ifndef CIMAGEIDENTIFICATION_H
#define CIMAGEIDENTIFICATION_H


#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/legacy/legacy.hpp>
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include <opencv2/nonfree/features2d.hpp>

#include "opencv_gpu_cpu.h"
#include <iostream>
#include <string.h>
#include <vector>
#include <map>
#include <memory>
using namespace std;
using namespace cv;


class ImageDescriptors{
public:
    ImageDescriptors(){}
    ImageDescriptors(Mat& ){}
    virtual int ORBMatch(void* _pMat){return 0;}
 
    virtual std::vector<DMatch> ORBMatch1(void* _pMat){return std::vector<DMatch>();}

public:
    //locate

    virtual std::vector<DMatch> flannBasedMatcher(void* _pMat){return std::vector<DMatch>();}

    /**
     * @brief getImageLocate 从指定图片中获取指定模板的位置信息
     * @param _pMat         灰色检测图
     * @return
     */
    virtual Rect getImageLocate(void* _pMat){return Rect();}

public:
    //get property
    virtual std::vector<KeyPoint>& getKeyPoints(){return keypoints_obj;}
    virtual std::vector<DMatch>& getMatches(){return matches;}
    virtual void* getDescriptors() {return nullptr;}

public:
    std::vector<DMatch> matches;
    std::vector<KeyPoint> keypoints_obj;
	Mat descriptors;
	flann::Index *pFlannIndex;
};

typedef std::vector<std::string> ImageFileVector;
//用来初始化系统资源，比如本系统主要识别游戏开始，游戏胜利和游戏失败，而每个不同识别的状态包含主要关键的几张识别资源，
//通过资源生成原始识别因子//通过关键帧图片进行图片识别和后续处理-- key 主要包括 heartstone_start  heartstone_Victory  heartstone_Fail
typedef std::map<std::string,ImageFileVector> IndentificationMap;
typedef  std::map<string, vector<ImageDescriptors*>> ImageDescriptorsMap;

struct TextArea{
    string text;
    Rect rect;
};

class CImageIdentification
{ 
public:
    /**
     * @brief initResourceImage 初始化原始图片识别资源，系统必须进行调用初始化
     *
     * @return
     */
   virtual bool initResourceImage(IndentificationMap& imagesMap) = 0;
   virtual bool initResourceLocateImage(IndentificationMap& imagesMap) = 0;
    /**
     * @brief shibie    函数识别 程序获取图片之后主动发起调用，该函数将进行识别循环处理
     * @param image 图片对象，使用OPENCV的Mat格式
     * @return  null表示没有找到，否则返回被识别对象名称
     */
   virtual char* shibie(void* pImage) = 0; 
   virtual int shibie(const char* _id, void* pImage) = 0;

    /**
    * @brief getImageLocation 从指定图片中获取指定模型的位置
    * @param tempName
    * @param pImage
    * @return
    */
   virtual Rect getImageLocation(string& tempName,void* pImage) = 0;


    /**
     * @brief isFriendListOn    判断用户界面是否已经打开
     * @param pImage
     * @return
     */
    virtual Rect isFriendListOn(void* pImage) = 0;


    /**
     * @brief getText 获得当前页面图片中的文字
     * @param pImage
     * @return
     */
    vector<TextArea> getText(void* pImage);

protected:
	ImageDescriptorsMap m_imageDescriptorsMap;

    ImageDescriptorsMap m_imageLocateMap;

	std::map<ImageDescriptors*, string> m_imageDescriptors2stringMap;



};


/**
 * @brief The ImageLocate class 图片识别基类
 */
class ImageLocate
{
public:
    /**
     * @brief initResourceImage 初始化原始图片识别资源，系统必须进行调用初始化
     *
     * @return
     */
   virtual bool initResourceImage(IndentificationMap& imagesMap) = 0;

    /**
     * @brief shibie    函数识别 程序获取图片之后主动发起调用，该函数将进行识别循环处理
     * @param image 图片对象，使用OPENCV的Mat格式
     * @return  null表示没有找到，否则返回被识别对象名称
     */
   virtual char* shibie(void* pImage) = 0;


    /**
    * @brief getImageLocation 从指定图片中获取指定模型的位置
    * @param tempName
    * @param pImage
    * @return
    */
   virtual Rect getImageLocation(string& tempName,void* pImage){return Rect();}


    /**
     * @brief isFriendListOn    判断用户界面是否已经打开
     * @param pImage
     * @return
     */
    virtual Rect isFriendListOn(void* pImage) = 0;
protected:
    ImageDescriptorsMap m_imageDescriptorsMap;
    std::map<ImageDescriptors*, string> m_imageDescriptors2stringMap;



};

#endif // CIMAGEIDENTIFICATION_H
