#ifndef _CaffeClassifier_H_
#define _CaffeClassifier_H_

#include "caffe_header.h"

/**
* \brief:	识别结果预测信息
*/
struct PredictionRes
{
	std::string name;	/** 类别名称 */
	int classId;		/** 类别ID */
	float percentage;	/** 相似度(百分比) */

	PredictionRes() : name(""), classId(0), percentage(0.0f)
	{}

	PredictionRes(std::string& _name, int _id, float _percentage) 
		: name(_name), classId(_id), percentage(_percentage)
	{}
};


/**
* \brief:	Caffe类别识别管理类	
*/
class CaffeClassifier 
{
public:
	/**
	* \brief:	CaffeClassifier构造函数
	* \param [model_file]:	模型配置文件(deploy.prototxt)
	* \param [trained_file]:	训练结果集caffemodel文件
	* \param [mean_file]:	均值文件(mean.binaryproto)
	* \param [trained_file]:	类别说明文件(label.txt)
	*/
	CaffeClassifier(const string& model_file, const string& trained_file, const string& mean_file, const string& label_file);

	std::vector<PredictionRes> classify(const cv::Mat& img, int N = 5);

private:
	void setMean(const string& mean_file);

	/** 预测 */
	std::vector<float> predict(const cv::Mat& img);

	void wrapInputLayer(std::vector<cv::Mat>* input_channels);

	void preprocess(const cv::Mat& img, std::vector<cv::Mat>* input_channels);

private:
	boost::shared_ptr<Net<float> > m_net;
	cv::Size m_inputGeometry;
	int num_channels_;
	cv::Mat m_mean;
	std::vector<string> m_labels;
};

#endif