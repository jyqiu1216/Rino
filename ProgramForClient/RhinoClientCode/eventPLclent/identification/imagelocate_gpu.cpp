#include "imagelocate_gpu.h"



ImageLocate_GPU::ImageLocate_GPU(Mat& _img)
{
	source_image = gpu::GpuMat(_img);

    int minHessian = 400;//SURF算法中的hessian阈值
    SurfFeatureDetector detector( minHessian );//定义一个SurfFeatureDetector（SURF） 特征检测类对象

    //【3】调用detect函数检测出SURF特征关键点，保存在vector容器中
    detector.detect(_img, keypoints_obj );

    //【4】计算描述符（特征向量）
    SurfDescriptorExtractor extractor;
    extractor.compute( _img, keypoints_obj, descriptors_cpu_src );

}

std::vector<DMatch> ImageLocate_GPU::flannBasedMatcher(void* _pMat)
{

    Mat& descriptors_scene = *(Mat*)_pMat;
    //【5】使用FLANN匹配算子进行匹配
    FlannBasedMatcher matcher;

    matcher.match( descriptors_cpu_src, descriptors_scene, matches );
    double max_dist = 0; double min_dist = 100;//最小距离和最大距离

    //【6】计算出关键点之间距离的最大值和最小值
    for( int i = 0; i < descriptors_cpu_src.rows; i++ )
    {
        double dist = matches[i].distance;
        if( dist < min_dist ) min_dist = dist;
        if( dist > max_dist ) max_dist = dist;
    }


    //【7】存下匹配距离小于3*min_dist的点对
    std::vector< DMatch > good_matches;
    for( int i = 0; i < descriptors_cpu_src.rows; i++ )
    {
        if( matches[i].distance < 3*min_dist )
        {
            good_matches.push_back( matches[i]);
        }
    }
    return good_matches;
}

/**
 * @brief getImageLocate 从指定图片中获取指定模板的位置信息
 * @param _pMat         灰色检测图
 * @return
 */
//#include <QDebug>
Rect ImageLocate_GPU::getImageLocate(void* _pMat)
{
	return Rect();
}
 Rect ImageLocate_GPU::getImageLocate1(void* _pMat)
 {
    return Rect();
 }
