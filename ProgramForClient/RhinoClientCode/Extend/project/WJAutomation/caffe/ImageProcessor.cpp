#include "ImageProcessor.h"

// 水平分割

void ImageProcessor::splitHorizontal(Mat& _img, vector<Rect>& _rt)
{
	Point pt(0, 0);

	int step = _img.step1() / sizeof(uchar);
	int channels = _img.channels();

	for (int i = 0; i < _img.rows; i++)
	{
		int status = 0;
		for (int j = 0; j < _img.cols; j++)
		{
			status += (_img.data[i*step + j*channels + 0] + _img.data[i*step + j*channels + 1] + _img.data[i*step + j*channels + 2]);
		}

		if (status > 0 && 0 == pt.x)
		{
			pt.x = i;
		}
		else if (status <= 0 && pt.x != 0)
		{
			pt.y = i;
			_rt.push_back(Rect(0, pt.x, _img.cols, pt.y - pt.x));
			char buf[32] = { 0 };
			sprintf(buf, "E:\\TEMP\\水平切割_%d-%d.jpg", pt.x, pt.y);
			imwrite(buf, _img(_rt.back()));

			pt.x = 0;
			pt.y = 0;
		}
	}
}

struct ColorRGB
{
	int r;
	int g;
	int b;
};


Rect ImageProcessor::getBound(Mat& _img)
{
	Rect rtBound;

	//【4】定义轮廓和层次结构
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	//////【5】查找轮廓
	findContours(_img, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	if (contours.size() <= 0)
		return Rect(0, 0, 0, 0);

	Point pt1(0, 0);
	Point pt2(0, 0);

	for (int i = 0; i < contours.size(); ++i)
	{
		rtBound = cv::boundingRect(contours[i]);

		if (0 == i)
		{
			pt1.x = rtBound.x;
			pt1.y = rtBound.y;
			pt2.x = rtBound.x + rtBound.width;
			pt2.y = rtBound.y + rtBound.height;
		}
		else
		{
			if (pt1.x > rtBound.x) pt1.x = rtBound.x;
			if (pt1.y > rtBound.y) pt1.y = rtBound.y;
			if (pt2.x < rtBound.x + rtBound.width) pt2.x = rtBound.x + rtBound.width;
			if (pt2.y < rtBound.y + rtBound.height) pt2.y = rtBound.y + rtBound.height;
		}
	}
	return Rect(pt1.x, pt1.y, pt2.x - pt1.x, pt2.y - pt1.y);
}




void ImageProcessor::saveBound(Mat& _imgSrc, vector<Rect>& _vecBound, Rect rtBound)
{
	for (int i = rtBound.y; i < rtBound.y + rtBound.height; ++i)
	{
		if (getLinePixelSumH(_imgSrc, i, rtBound.x, rtBound.x + rtBound.width - 1) > 0)
		{
			break;
		}

		++rtBound.y;
		--rtBound.height;
	}

	for (int i = rtBound.y + rtBound.height - 1; i >= rtBound.y; --i)
	{

		if (getLinePixelSumH(_imgSrc, i, rtBound.x, rtBound.x + rtBound.width - 1) > 0)
		{
			break;
		}
		--rtBound.height;
	}

	_vecBound.push_back(rtBound);

	//static int num = 0;
	//++num;
	//char buf[32] = { 0 };
	//sprintf(buf, "E:\\AAAAA\\SAVE_%d.jpg", num);
	//imwrite(buf, _imgSrc(rtBound));
}

int ImageProcessor::splitVetrical1(Mat& _imgSrc, int _row1, int _row2, vector<Rect>& _vecRt)
{
	int maxWidth = 0;

	// 切割一行
	Mat _img = _imgSrc(Rect(0, _row1, _imgSrc.cols, _row2 - _row1));

	/**
	* status 用于标识当前操作状态:
	*	0：识别起始非空白行, 1: 识别结束空白行, 2: , 3:保存矩形边框
	*/
	int status = 0;

	// 保存当前切割的单个文字的矩形区域
	Rect rtCur(-1, _row1, -1, _img.rows);

	for (int col = 0; col < _img.cols; ++col)
	{
		if (ImageProcessor::getLinePixelSumV(_img, col, 0, _img.rows - 1) > 255 * 0)
		{
			if (0 == status)
			{
				rtCur.x = col;
				status = 1;
			}
		}
		else
		{
			if (1 == status)
			{
				int end = col;
				rtCur.width = end - rtCur.x;
				saveBound(_imgSrc, _vecRt, rtCur);

				if (maxWidth < rtCur.width)
					maxWidth = rtCur.width;

				status = 0;
			}
		}
	} // $$ end of for
	return maxWidth;
}

void ImageProcessor::splitVetrical(Mat& _imgSrc, int _row1, int _row2, vector<Rect>& _vecRt)
{
	// 切割一行
	Mat _img = _imgSrc(Rect(0, _row1, _imgSrc.cols, _row2 - _row1));

	/**
	* status 用于标识当前操作状态:
	*	0：识别起始非空白行, 1: 识别结束空白行, 2: , 3:保存矩形边框
	*/
	int status = 0;

	// 保存当前切割的单个文字的矩形区域
	Rect rtCur(-1, _row1, -1, _img.rows);
	Rect rtTemp(-1, _row1, 0, _img.rows);

	for (int col = 0; col < _img.cols; ++col)
	{
		if (ImageProcessor::getLinePixelSumV(_img, col, 0, _img.rows - 1) > 255 * 0)
		{
			if (0 == status)
			{
				rtCur.x = (col > 0) ? (col - 1) : col;
				status = 1;
			}
		}
		else
		{
			if (1 == status)
			{
				int end = (col < (_img.cols - 1)) ? (col + 1) : col;
				rtCur.width = end - rtCur.x;

				int width = rtTemp.width > 0 ? (end - rtTemp.x) : (end - rtCur.x);
				if (width < rtCur.height*0.8)
				{
					if (rtTemp.width <= 0)
						rtTemp.x = rtCur.x;

					rtTemp.width = width;
				}
				else if (width > rtCur.height * 1.5)
				{
					if (rtTemp.x >= 0 && rtTemp.width > 0)
					{
						saveBound(_imgSrc, _vecRt, rtTemp);
						rtTemp = rtCur;
					}
					else
					{
						saveBound(_imgSrc, _vecRt, rtCur);
					}
				}
				else
				{
					if (rtTemp.width > 0)
					{
						rtCur.x = rtTemp.x;
						rtTemp.x = -1;
						rtTemp.width = 0;
					}
					rtCur.width = width;
					saveBound(_imgSrc, _vecRt, rtCur);
				}
				status = 0;
			}
		}
	} // $$ end of for

	if (rtTemp.width > 0)
	{
		saveBound(_imgSrc, _vecRt, rtCur);
	}
}

void ImageProcessor::getBoundRect(Mat& _img, vector<Rect>& _rtBound)
{
	int rowNum = 0;

	int begin = 0;
	int end = 0;

	for (int row = 0; row < _img.rows; row++)
	{
		if (getLinePixelSumH(_img, row, 0, _img.cols - 1) > 255)
		{
			if (0 == begin)
			{
				begin = row;

				if (begin > 0)
					begin -= 1;
			}
		}
		else
		{
			if (begin != 0 && 0 == end)
			{
				end = row;

				if (end < _img.rows)
					end += 1;

				splitVetrical(_img, begin, end, _rtBound);

				//char buf[128] = { 0 };
				//sprintf(buf, "C:/Users/lixiaoyuan/Documents/文字识别/LINE-%d.jpg", rowNum);
				//imwrite(buf, _img(Rect(0, begin, _img.cols, end - begin)));

				begin = 0;
				end = 0;
				++rowNum;
			}
		}
	}
}


void ImageProcessor::getTextLine(Mat& _img, vector<Rect>& _vecRt)
{
	int begin = -1;
	int end = -1;

	Rect rt(0, 0, _img.cols, 0);

	for (int row = 0; row < _img.rows; row++)
	{
		if (getLinePixelSumH(_img, row, 0, _img.cols - 1) > 0)
		{
			if (begin < 0)
			{
				begin = row;

				if (begin > 0)
					begin -= 1;
			}
		}
		else
		{
			if (begin >= 0 && end < 0)
			{
				end = row;

				if (end < _img.rows)
					end += 1;

				rt.y = begin;
				rt.height = end - begin;
				_vecRt.push_back(rt);

				begin = -1;
				end = -1;
			}
		}
	}
}

int ImageProcessor::getLinePixelSumH(Mat& _img, const int& _row, const int& _begin, const int& _end)
{
	if (_row >= _img.rows || _begin < 0 || _end > _img.cols - 1)
		return -1;

	int pixelSum = 0;
	int _channels = _img.channels();

	uchar* data = _img.ptr<uchar>(_row);
	if (!data)
		return -1;

	for (int col = _begin; col <= _end; ++col)
	{
		for (int i = 0; i < _channels; ++i)
		{
			pixelSum += (data[col*_channels + i]);
		}
	}

	return pixelSum;
}


int ImageProcessor::getLinePixelSumV(Mat& _img, const int& _col, const int& _begin, const int& _end)
{
	if (_col >= _img.cols || _begin < 0 || _end > _img.rows - 1)
		return -1;

	int pixelSum = 0;
	int _channels = _img.channels();

	for (int row = _begin; row <= _end; ++row)
	{
		uchar* data = _img.ptr<uchar>(row);
		if (!data)
			return -1;

		for (int i = 0; i < _channels; ++i)
		{
			pixelSum += (data[_col*_channels + i]);
		}
	}
	return pixelSum;
}



Size ImageProcessor::getBlankTL(Mat& _img, Rect& _rt)
{
	Size size;
	for (int row = _rt.y; row < _rt.y + _rt.height; ++row)
	{
		if (getLinePixelSumH(_img, row, _rt.x, _rt.x + _rt.width - 1) > 0)
			break;

		++size.height;
	}

	for (int col = _rt.x; col < _rt.x + _rt.width; ++col)
	{
		if (getLinePixelSumV(_img, col, _rt.y, _rt.y + _rt.height - 1) > 0)
			break;

		++size.width;
	}
	return size;
}

Size ImageProcessor::getBlankBR(Mat& _img, Rect& _rt)
{
	Size size;
	for (int row = _rt.y + _rt.height - 1; row >= _rt.y; --row)
	{
		if (getLinePixelSumH(_img, row, _rt.x, _rt.x + _rt.width - 1) > 0)
			break;

		++size.height;
	}

	for (int col = _rt.x + _rt.width - 1; col >= _rt.x; --col)
	{
		if (getLinePixelSumV(_img, col, _rt.y, _rt.y + _rt.height - 1) > 0)
			break;

		++size.width;
	}
	return size;
}