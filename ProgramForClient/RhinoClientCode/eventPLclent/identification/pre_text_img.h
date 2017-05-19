#ifndef _PRE_TEXT_IMG_pre_tex_timg_H_
#define _PRE_TEXT_IMG_pre_tex_timg_H_


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
#include "timemanager.h"

using namespace std;
using namespace cv;


void Preprocess( Mat& sourceImg );
void Preprocess_canny( Mat& sourceImg,Mat &edgeImg,int canny_1 = 100 );
void colorFilter(Mat& inputImage, Mat& outputImage);
void colorFilterGay(Mat& inputImage, Mat& outputImage,int grayLevel1 = 50,int grayLevel2 = 150);
void findTextRegion(Mat& srcMat,vector<Rect>& rect);
void constrastAndBright(Mat& srcImag,Mat& desImag,int constrast,int bright);


//heart stone  mat preopt  ---begin--

//本期采用颜色过滤方式进行系统一期，处理,二期采用机器学习方式进行系统文字识别处理
//1-炉石好友颜色分离,只保留好友名称，好友状态等关键颜色，其他颜色系统不予保留!
void heartStone_friendList_color(Mat& srcImage,Mat& resultImage);

//2-前函数处理后的图片作为本函数入参,进行轮廓判断，查询得到关键文字信息并返回
vector<Rect> get_heartStone_friendName_rect(Mat& _image);

//heart stone  mat preopt  ---end--


#endif // _PRE_TEXT_IMG_pre_tex_timg_H_
