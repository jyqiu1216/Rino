// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//
#pragma once

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
//#include "opencv2/features2d/features2d.hpp"
//#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/nonfree/nonfree.hpp"
//#include "opencv2/legacy/legacy.hpp"
//#include "opencv2/calib3d/calib3d.hpp"
//#include "opencv2/nonfree/nonfree.hpp"
//#include "opencv2/nonfree/features2d.hpp"

#include "caffe/proto/caffe.pb.h"
#include "caffe/caffe.hpp"
#include "caffe/common.hpp"  
#include "caffe/layers/input_layer.hpp"  
#include "caffe/layers/inner_product_layer.hpp"  
#include "caffe/layers/dropout_layer.hpp"  
#include "caffe/layers/conv_layer.hpp"  
#include "caffe/layers/relu_layer.hpp"  
#include "caffe/layers/pooling_layer.hpp"  
#include "caffe/layers/lrn_layer.hpp"  
#include "caffe/layers/softmax_layer.hpp"  

#include <algorithm>
#include <iosfwd>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#ifdef _DEBUG
#pragma comment(lib, "opencv_calib3d2410d.lib")
#pragma comment(lib, "opencv_contrib2410d.lib")
#pragma comment(lib, "opencv_core2410d.lib")
#pragma comment(lib, "opencv_features2d2410d.lib")
#pragma comment(lib, "opencv_flann2410d.lib")
#pragma comment(lib, "opencv_gpu2410d.lib")
#pragma comment(lib, "opencv_highgui2410d.lib")
#pragma comment(lib, "opencv_imgproc2410d.lib")
#pragma comment(lib, "opencv_ml2410d.lib")
#pragma comment(lib, "opencv_legacy2410d.lib")
#pragma comment(lib, "opencv_nonfree2410d.lib")
#pragma comment(lib, "opencv_objdetect2410d.lib")
#pragma comment(lib, "gflagsd.lib")
#else
#pragma comment(lib, "opencv_calib3d2410.lib")
#pragma comment(lib, "opencv_contrib2410.lib")
#pragma comment(lib, "opencv_core2410.lib")
#pragma comment(lib, "opencv_features2d2410.lib")
#pragma comment(lib, "opencv_flann2410.lib")
#pragma comment(lib, "opencv_gpu2410.lib")
#pragma comment(lib, "opencv_highgui2410.lib")
#pragma comment(lib, "opencv_imgproc2410.lib")
#pragma comment(lib, "opencv_ml2410.lib")
#pragma comment(lib, "opencv_legacy2410.lib")
#pragma comment(lib, "opencv_nonfree2410.lib")
#pragma comment(lib, "opencv_objdetect2410.lib")
#pragma comment(lib, "gflags.lib")
#endif

#pragma comment(lib, "libprotobuf.lib")
#pragma comment(lib, "libglog.lib")
#pragma comment(lib, "hdf5.lib")
#pragma comment(lib, "hdf5_hl.lib")
#pragma comment(lib, "hdf5_tools.lib")
#pragma comment(lib, "hdf5_cpp.lib")
#pragma comment(lib, "libopenblas.dll.a")
#pragma comment(lib, "libcaffe.lib")

//#pragma comment(lib, "cuda/lib/x64/cudnn.lib")
//#pragma comment(lib, "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v8.0/lib/x64/cuda.lib")
//#pragma comment(lib, "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v8.0/lib/x64/cublas.lib")
//#pragma comment(lib, "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v8.0/lib/x64/curand.lib")
//#pragma comment(lib, "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v8.0/lib/x64/cudart.lib")

//#include "LinkAllSymbol.h"

using namespace caffe;  // NOLINT(build/namespaces)

namespace caffe
{
	extern INSTANTIATE_CLASS(InputLayer);
	extern INSTANTIATE_CLASS(InnerProductLayer);
	extern INSTANTIATE_CLASS(DropoutLayer);
	extern INSTANTIATE_CLASS(ConvolutionLayer);
	//REGISTER_LAYER_CLASS(Convolution);
	extern INSTANTIATE_CLASS(ReLULayer);
	//REGISTER_LAYER_CLASS(ReLU);
	extern INSTANTIATE_CLASS(PoolingLayer);
	//REGISTER_LAYER_CLASS(Pooling);
	extern INSTANTIATE_CLASS(LRNLayer);
	//REGISTER_LAYER_CLASS(LRN);
	extern INSTANTIATE_CLASS(SoftmaxLayer);
	//REGISTER_LAYER_CLASS(Softmax);
}
// TODO:  在此处引用程序需要的其他头文件
