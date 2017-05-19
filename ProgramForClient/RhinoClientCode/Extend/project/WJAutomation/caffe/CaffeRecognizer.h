#pragma once

#include "CaffeClassifier.h"
#include "ImageProcessor.h"


#include <list>
using namespace std;


struct ModelInfo
{
	std::string modelFile;
	std::string deployFile;
	std::string meanFile;
	std::string labelFile;
};


class CaffeRecognizer
{
public:
	CaffeRecognizer();
	CaffeRecognizer(ModelInfo& _modelInfo);

	void initModel(ModelInfo& _modelInfo);
	//void initCaffeClassifier();

	void splitTextLine(Mat& _img, vector<Rect>& _vecRt);
	
	
	std::string getText(Mat& _img);

	void adjustBound(Mat& _img, Rect& _rtBound);

	int getTextSingle(Mat& _img, std::string& _text);

	bool getClass(Mat& _img, PredictionRes &_res);

private:
	CaffeClassifier* m_classifier;
};