#pragma once

#include "caffe/caffe_header.h"
#include "caffe/caffe.hpp"
#include "caffe/CaffeRecognizer.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <algorithm>
#include <iosfwd>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <fstream>



class ResourceHandler
{
public:
	static void test1(CaffeRecognizer *_recognizer, const std::string& _destDir)
	{
		sprit1(_recognizer, "E:/TEMP/16.jpg", Size(16, 16), _destDir);
		sprit1(_recognizer, "E:/TEMP/20.jpg", Size(20, 20), _destDir);
		sprit1(_recognizer, "E:/TEMP/24.jpg", Size(24, 24), _destDir);
	}

	static void sprit1(CaffeRecognizer *_recognizer, const string& _src, Size& _size, const string& _destDir)
	{
		Mat imgSrc = imread(_src, 0);
		Mat imgShow = imgSrc.clone();
		Mat imgDest = imgSrc.clone();
	
		////二值化
		cv::threshold(imgDest, imgDest, 50, 255, CV_THRESH_OTSU);

		vector<Rect> rtTextLine;
		_recognizer->splitTextLine(imgSrc, rtTextLine);

		int num = 0;

		for (size_t row = 0; row + _size.height < imgDest.rows; row += _size.height)
		{
			for (size_t col = 0; col + _size.width < imgDest.cols; col += _size.width)
			{
				vector<Rect> vecBound;
				ImageProcessor::splitVetrical1(imgDest(Rect(col, row, _size.width, _size.height)), 0, _size.height, vecBound);

				if (vecBound.size() <= 1)
					continue;

				
				Rect rtTemp = vecBound[0];

				for (size_t i = 0; i < vecBound.size()-1; i++)
				{
					rtTemp.width = vecBound[i].width + vecBound[i].x - rtTemp.x;

					int h = 0;
					if (rtTemp.y + rtTemp.height < vecBound[i].y + vecBound[i].height )
					{
						h = vecBound[i].y + vecBound[i].height;
					}
					else
					{
						h = rtTemp.y + rtTemp.height;
					}

					if (vecBound[i].y < rtTemp.y)
					{
						rtTemp.y = vecBound[i].y;
					}
					rtTemp.height = h - rtTemp.y;
			

					Rect rt(rtTemp.x + col, rtTemp.y + row, rtTemp.width, rtTemp.height);
					char buf[128] = { 0 };
					sprintf(buf, "/99999_%d_%dx%d.jpg", num, _size.width, _size.height);

					Mat img = imgSrc(rt);
					setBorder(img);
					imwrite(_destDir + buf, img);
					++num;
					cv::rectangle(imgShow, rt.tl(), rt.br(), cv::Scalar(0, 0, 255));
				}
			}
		}

		//imshow("切割", imgShow);
		//waitKey(0);
	}


	static void handleResource(const std::string& _destDir)
	{
		//std::string destDir = "F:/01-Development/lixySample/bin/samples/TEST/data_train";
		//std::string destDir = "F:/01-Development/lixySample/bin/samples/TEST/data_test";

		cout << "\n ===> 训练数据 \n";
		spritImage("E:/TEMP/16.jpg", Size(16, 16), _destDir + "/data_train");
		spritImage("E:/TEMP/20.jpg", Size(20, 20), _destDir + "/data_train");
		spritImage("E:/TEMP/24.jpg", Size(24, 24), _destDir + "/data_train");
		spritImage("E:/TEMP/28.jpg", Size(28, 28), _destDir + "/data_train");
		spritImage("E:/TEMP/32.jpg", Size(32, 32), _destDir + "/data_train");


		cout << "\n ===> 测试数据 \n";
		spritImage("E:/TEMP/20.jpg", Size(20, 20), _destDir + "/data_test");
		spritImage("E:/TEMP/28.jpg", Size(28, 28), _destDir + "/data_test");

		cout << "\n ===> END \n";
	}

	static void setBorder(Mat &_img)
	{
		// 黑色边框
		Mat imgBorder = Mat::zeros(Size(_img.cols + 2, _img.rows + 2), _img.type());

		// 定义一个Mat类型并给其设定ROI区域  
		Mat imageROI = imgBorder(Rect(1, 1, _img.cols, _img.rows));

		// 拷贝到ROI  
		_img.copyTo(imageROI);
		_img = imgBorder;
	}

	static void spritImage(const string& _src, Size& _size, const string& _destDir)
	{
		cout << " ===> " << _src.c_str() << endl;;
		Mat imgSrc = imread(_src, 0);
		Mat imgGray = imgSrc.clone();
		cv::threshold(imgGray, imgGray, 50, 255, CV_THRESH_OTSU);

		//Mat imgT = imgGray > 50;
		Mat imgT = imgGray.clone();

		//imshow("二值化", imgT);
		//waitKey(0);

		char buf[128] = { 0 };
		CreateDirectoryA(_destDir.c_str(), 0);

		int count = 0;
		for (size_t row = 0; row + _size.height <= imgT.rows; row += _size.height)
		{
			for (size_t col = 0; col + _size.width <= imgT.cols; col += _size.width)
			{
				sprintf(buf, "%s/%d", _destDir.c_str(), count);
				CreateDirectoryA(buf, 0);

				Rect rt(col, row, _size.width, _size.height);
				

				Size tl = ImageProcessor::getBlankTL(imgT, rt);
				Size br = ImageProcessor::getBlankBR(imgT, rt);

				if (tl.width >= _size.width)
					continue;

				rt.x += tl.width;
				rt.y += tl.height;
				rt.width -= (tl.width + br.width);
				rt.height -= (tl.height + br.height);

				


				//Mat imgU = imgT(Rect(col, row, _size.width, _size.height));
				////【4】定义轮廓和层次结构
				//vector<vector<Point>> contours;
				//vector<Vec4i> hierarchy;

				////////【5】查找轮廓
				//findContours(imgU, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
				//if (contours.size() <= 0)
				//	continue;

				//Point pt1(0, 0);
				//Point pt2(0, 0);

				//for (int i = 0; i < contours.size(); ++i)
				//{
				//	Rect rt = cv::boundingRect(contours[i]);
				//	rt.x += col;
				//	rt.y += row;

				//	if (0 == i)
				//	{
				//		pt1.x = rt.x;
				//		pt1.y = rt.y;
				//		pt2.x = rt.x + rt.width;
				//		pt2.y = rt.y + rt.height;
				//	}
				//	else
				//	{
				//		if (pt1.x > rt.x) pt1.x = rt.x;
				//		if (pt1.y > rt.y) pt1.y = rt.y;
				//		if (pt2.x < rt.x + rt.width) pt2.x = rt.x + rt.width;
				//		if (pt2.y < rt.y + rt.height) pt2.y = rt.y + rt.height;
				//	}
				//}

				//Rect rt(pt1.x, pt1.y, pt2.x - pt1.x, pt2.y - pt1.y);
				//if (rt.x > 0) rt.x -= 1;
				//if (rt.y > 0) rt.y -= 1;
				//if (rt.x + rt.width < imgSrc.cols - 1) rt.width += 2;
				//if (rt.y + rt.height < imgSrc.rows - 1) rt.height += 2;

				sprintf(buf, "/%d/%d_%dx%d.jpg", count, count, _size.width, _size.height);
				Mat img = imgSrc(rt);
				setBorder(img);
				imwrite(_destDir + buf, img);

				//sprintf(buf, "/%d/%d_%dx%d_1.jpg", count, count, _size.width, _size.height);
				//imwrite(_destDir + buf, imgGray(rt));
				count++;
			}
		}
	}
};