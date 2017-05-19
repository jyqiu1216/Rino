#pragma once

#ifdef _DEBUG
#pragma comment(lib, "opencv_calib3d2410d.lib")
#pragma comment(lib, "opencv_contrib2410d.lib")
#pragma comment(lib, "opencv_core2410d.lib")
#pragma comment(lib, "opencv_gpu2410d.lib")
#pragma comment(lib, "opencv_highgui2410d.lib")
#pragma comment(lib, "opencv_imgproc2410d.lib")
#pragma comment(lib, "opencv_ml2410d.lib")
#pragma comment(lib, "opencv_ocl2410d.lib")
#pragma comment(lib, "opencv_features2d2410d.lib")
#pragma comment(lib, "opencv_superres2410d.lib")
#pragma comment(lib, "opencv_stitching2410d.lib")
#pragma comment(lib, "opencv_legacy2410d.lib")
#pragma comment(lib, "opencv_ts2410d.lib")
#pragma comment(lib, "opencv_flann2410d.lib")
#pragma comment(lib, "opencv_nonfree2410d.lib")
#else
#pragma comment(lib, "opencv_calib3d2410.lib")
#pragma comment(lib, "opencv_contrib2410.lib")
#pragma comment(lib, "opencv_core2410.lib")
#pragma comment(lib, "opencv_gpu2410.lib")
#pragma comment(lib, "opencv_highgui2410.lib")
#pragma comment(lib, "opencv_imgproc2410.lib")
#pragma comment(lib, "opencv_ml2410.lib")
#pragma comment(lib, "opencv_ocl2410.lib")
#pragma comment(lib, "opencv_features2d2410.lib")
#pragma comment(lib, "opencv_superres2410.lib")
#pragma comment(lib, "opencv_stitching2410.lib")
#pragma comment(lib, "opencv_legacy2410.lib")
#pragma comment(lib, "opencv_ts2410.lib")
#pragma comment(lib, "opencv_flann2410.lib")
#pragma comment(lib, "opencv_nonfree2410.lib")
#endif

#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/nonfree/features2d.hpp"

#include <iostream>
#include <string.h>
#include <vector>
#include <map>
#include <memory>
using namespace std;
using namespace cv;


class ImageProcessor
{
public:
	// ∂˛÷µªØ
	static void threshold(Mat& _img, double _thresh, double _maxval)
	{
		cv::threshold(_img, _img, _thresh, _maxval, CV_THRESH_BINARY_INV);
	}


	// ≈Ú’Õ
	

	// ∏Ø ¥
	static void saveBound(Mat& _imgSrc, vector<Rect>& _vecBound, Rect rtBound);

	// ±ﬂøÚ
	static Rect getBound(Mat& _img);

	//static void getBound(Mat& _img, vector<Rect>& rtBound, int _offsetX, int _offsetY)
	//{
	//	vector<vector<cv::Point> > vecContours;
	//	vector<Vec4i> vecHierarchy;
	//	findContours(_img, vecContours, vecHierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	//	for (int i = 0; i < vecContours.size(); i++)
	//	{
	//		Rect rt = boundingRect(Mat(vecContours[i]));
	//		//rectangle(img_src, rect.tl(), rect.br(), Scalar(0, 0, 255));
	//		rt.x += _offsetX;
	//		rt.y += _offsetY;
	//		rtBound.push_back(rt);
	//	}
	//}

	// ±ﬂøÚ
	static void getBoundRect(Mat& _img, vector<Rect>& rtBound);

	static void splitHorizontal(Mat& _img, vector<Rect>& _rt);
	static void splitVetrical(Mat& _img, int _row1, int _row2, vector<Rect>& _vecRt);
	static int splitVetrical1(Mat& _img, int _row1, int _row2, vector<Rect>& _vecRt);

	static void getTextLine(Mat& _img, vector<Rect>& _vecRt);


	static int getLinePixelSumH(Mat& _img, const int& _row, const int& _begin, const int& _end);
	static int getLinePixelSumV(Mat& _img, const int& _col, const int& _begin, const int& _end);


	static Size getBlankTL(Mat& _img, Rect& _rt);

	static Size getBlankBR(Mat& _img, Rect& _rt);
};