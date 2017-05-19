

#include "CaffeRecognizer.h"


#include <list>
using namespace std;


//CaffeClassifier* CaffeRecognizer::ms_classifier = nullptr;

CaffeRecognizer::CaffeRecognizer()
	: m_classifier(nullptr)
{
}

CaffeRecognizer::CaffeRecognizer(ModelInfo& _modelInfo)
	: m_classifier(nullptr)
{
	initModel(_modelInfo);
}

void CaffeRecognizer::initModel(ModelInfo& _modelInfo)
{
	if (m_classifier)
		delete m_classifier;

	m_classifier = new CaffeClassifier(_modelInfo.modelFile, _modelInfo.deployFile, _modelInfo.meanFile, _modelInfo.labelFile);
}


void CaffeRecognizer::splitTextLine(Mat& _imgSrc, vector<Rect>& _vecRt)
{
	Mat img = _imgSrc.clone();// = Mat::zeros(_imgSrc.rows, _imgSrc.cols, CV_8UC3);

	//imgDest = imgDest > 50;
	////二值化
	cv::threshold(img, img, 50, 255, CV_THRESH_OTSU);
	imwrite("C:/Users/lixiaoyuan/Documents/文字识别/0SRC.png", img);

	int begin = -1;
	int end = -1;

	Rect rt(0, 0, img.cols, 0);

	for (int row = 0; row < img.rows; row++)
	{
		int pixelSum = ImageProcessor::getLinePixelSumH(img, row, 0, img.cols - 1);
		if (begin < 0)
		{
			if (pixelSum > 0)
			{
				begin = row;
			}
		}
		else
		{
			bool flag = false;
			if (pixelSum <= 0)
			{
				flag = true;
				end = row;
				rt.y = begin;
				rt.height = end - begin;
			}
			else if (end < 0 && row == img.rows - 1)
			{
				flag = true;
				end = row;
				rt.y = begin;
				rt.height = end - begin + 1;
			}

			if (flag)
			{
				_vecRt.push_back(rt);

				begin = -1;

				char buf[128] = { 0 };
				sprintf(buf, "C:/Users/lixiaoyuan/Documents/文字识别/LINE-%d_1.png", row);
				imwrite(buf, _imgSrc(rt));
			}
		}

	}
}



void adjustBoundBlank(Mat& _img, Rect& _rt)
{
	Size tl = ImageProcessor::getBlankTL(_img, _rt);
	Size br = ImageProcessor::getBlankBR(_img, _rt);


	_rt.y += tl.height;
	_rt.height -= tl.height;


	_rt.height -= br.height;
}


std::string CaffeRecognizer::getText(Mat& _imgSrc)
{
	std::string text;

	Mat img = _imgSrc.clone();
	cv::threshold(img, img, 30, 255, CV_THRESH_OTSU);
	////二值化
	//cv::threshold(img, img, 45, 255, CV_THRESH_OTSU);

	/**
	* status 用于标识当前操作状态: 0：识别起始非空白行, 1: 识别结束空白行
	*/
	int status = 0;

	// 保存当前切割的单个文字的矩形区域
	Rect rtCur(-1, 0, 0, img.rows);
	Rect rtLast(-1, 0, 0, img.rows);

	cout << " ===> ";

	bool flag = false;

	for (int col = 0; col < img.cols; ++col)
	{
		int pixelSum = ImageProcessor::getLinePixelSumV(img, col, 0, img.rows - 1);
		if (0 == status)
		{
			if (pixelSum > 0)
			{
				rtCur.x = col;
				status = 1;
			}
		}
		else if (1 == status)
		{
			if (pixelSum <= 255 * 0 || (img.cols - 1 == col))
			{
				int end = col;
				rtCur.width = end - rtCur.x;

				adjustBoundBlank(img, rtCur);
				//rtCur.height -= (tl.height + br.height);

				if (flag)
				{
					rtLast.width = rtCur.width + rtCur.x - rtLast.x;

					if (rtLast.y > rtCur.y)
					{
						rtLast.y = rtCur.y;
					}

					if (rtLast.y + rtLast.height < rtCur.y + rtCur.height)
					{
						rtLast.height = rtCur.y + rtCur.height - rtLast.y;
					}
					rtCur = rtLast;
				}

				//if (pixelSum > 0)
				//{
				//	rtCur.width++;
				//}

				std::string str;
				getTextSingle(_imgSrc(rtCur), str);

				if (0 == str.compare("*"))
				{
					flag = true;
					rtLast = rtCur;
				}
				else
				{
					flag = false;
					text += str;
				}

				status = 0;
				rtCur.y = 0;
				rtCur.height = _imgSrc.rows;
			}
		}
	} // $$ end of for

	cout << text.c_str() << endl;
	return text;
}

void setBorder(Mat &_img)
{
	// 黑色边框
	Mat imgBorder = Mat::zeros(Size(_img.cols + 2, _img.rows + 2), _img.type());

	// 定义一个Mat类型并给其设定ROI区域  
	Mat imageROI = imgBorder(Rect(1, 1, _img.cols, _img.rows));

	// 拷贝到ROI  
	_img.copyTo(imageROI);
	_img = imgBorder;
}

int CaffeRecognizer::getTextSingle(Mat& _img, std::string& _text)
{
	setBorder(_img);

	static int num = 0;
	++num;
	char buf[128] = { 0 };
	sprintf(buf, "C:/Users/lixiaoyuan/Documents/文字识别/TEXT-%d.png", num);
	imwrite(buf, _img);

	int classId = -1;
	if (m_classifier)
	{
		std::vector<PredictionRes> predictions = m_classifier->classify(_img);
		for (size_t j = 0; j < 1; ++j)
		{
			PredictionRes &p = predictions[j];
			//std::cout << std::fixed << std::setprecision(4) << p.second << " - \"" << p.first << "\"" << std::endl;
			_text += p.name;
			classId = p.classId;
		}
	}
	return classId;
}

bool CaffeRecognizer::getClass(Mat& _img, PredictionRes &_res)
{
	if (m_classifier)
	{
		std::vector<PredictionRes> predictions = m_classifier->classify(_img);
		for (size_t j = 0; j < 1; ++j)
		{
			_res = predictions[j];
			return true;
		}
	}
	return false;
}