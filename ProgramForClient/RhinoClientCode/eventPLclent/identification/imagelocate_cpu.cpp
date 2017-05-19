#include "imagelocate_cpu.h"



ImageLocate_CPU::ImageLocate_CPU(Mat& _img)
{
    source_image = _img;
    int minHessian = 400;//SURF算法中的hessian阈值
    SurfFeatureDetector detector( minHessian );//定义一个SurfFeatureDetector（SURF） 特征检测类对象

    //【3】调用detect函数检测出SURF特征关键点，保存在vector容器中
    detector.detect(_img, keypoints_obj );

    //【4】计算描述符（特征向量）
    SurfDescriptorExtractor extractor;
    extractor.compute( _img, keypoints_obj, descriptors_cpu_src );

}

std::vector<DMatch> ImageLocate_CPU::flannBasedMatcher(void* _pMat)
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


Rect ImageLocate_CPU::getImageLocate(void* _pMat)
{
//【2】使用SURF算子检测关键点
    Mat& screne_image = *(Mat*)_pMat; CTimeManager::getInstance()->time_update();
    int minHessian = 400;//SURF算法中的hessian阈值
    SurfFeatureDetector detector( minHessian );//定义一个SurfFeatureDetector（SURF） 特征检测类对象

 cout<<"lastTime 1" << CTimeManager::getInstance()->time_update();

    std::vector<KeyPoint> keypoints_scene;
    //【3】调用detect函数检测出SURF特征关键点，保存在vector容器中
    detector.detect( screne_image, keypoints_scene );

    //【4】计算描述符（特征向量）
    SurfDescriptorExtractor extractor;
    cv::Mat descriptors_scene;
    extractor.compute( screne_image, keypoints_scene, descriptors_scene );

cout<<"lastTime 2" << CTimeManager::getInstance()->time_update();
//    //【5】使用FLANN匹配算子进行匹配
//        FlannBasedMatcher matcher;
//        vector< DMatch > matches;
//        matcher.match( descriptors_object, descriptors_scene, matches );
//        double max_dist = 0; double min_dist = 100;//最小距离和最大距离
    //【7】存下匹配距离小于3*min_dist的点对
     std::vector< DMatch > good_matches = flannBasedMatcher(&descriptors_scene);
 cout<<"lastTime 3" << CTimeManager::getInstance()->time_update();
    //定义两个局部变量
    vector<Point2f> obj;
    vector<Point2f> scene;

    //从匹配成功的匹配对中获取关键点
    for( unsigned int i = 0; i < good_matches.size(); i++ )
    {
        obj.push_back( keypoints_obj[ good_matches[i].queryIdx ].pt );
        scene.push_back( keypoints_scene[ good_matches[i].trainIdx ].pt );
    }
 cout<<"lastTime 4" << CTimeManager::getInstance()->time_update();
    Mat H = findHomography( obj, scene, CV_RANSAC );//计算透视变换

    //从待测图片中获取角点
    vector<Point2f> obj_corners(4);
    obj_corners[0] = cvPoint(0,0);
    obj_corners[1] = cvPoint( source_image.cols, 0 );
    obj_corners[2] = cvPoint( source_image.cols, source_image.rows );
    obj_corners[3] = cvPoint( 0, source_image.rows );

    vector<Point2f> scene_corners(4);
    //进行透视变换
    perspectiveTransform( obj_corners, scene_corners, H);

 cout<<"lastTime 5" << CTimeManager::getInstance()->time_update();
    int x = scene_corners[0].x;
    int y = scene_corners[0].y;

    int width = scene_corners[1].x - scene_corners[0].x;
    int height = scene_corners[2].y - scene_corners[1].y;

    //
    cout<<"lastTime " << CTimeManager::getInstance()->time_update();

    Rect rect(x,y,width,height);

  // //绘制出角点之间的直线
  // line(screne_image, scene_corners[0], scene_corners[1] , Scalar(255, 0, 123), 4 );
  // line(screne_image, scene_corners[1], scene_corners[2] , Scalar( 255, 0, 123), 4 );
  // line(screne_image, scene_corners[2], scene_corners[3] , Scalar( 255, 0, 123), 4 );
  // line(screne_image, scene_corners[3], scene_corners[0] , Scalar( 255, 0, 123), 4 );
 ////绘制出角点之间的直线
 //line( screne_image, scene_corners[0] + Point2f( static_cast<float>(source_image.cols), 0), scene_corners[1] + Point2f( static_cast<float>(source_image.cols), 0), Scalar(255, 0, 123), 4 );
 //line( screne_image, scene_corners[1] + Point2f( static_cast<float>(source_image.cols), 0), scene_corners[2] + Point2f( static_cast<float>(source_image.cols), 0), Scalar( 255, 0, 123), 4 );
 //line( screne_image, scene_corners[2] + Point2f( static_cast<float>(source_image.cols), 0), scene_corners[3] + Point2f( static_cast<float>(source_image.cols), 0), Scalar( 255, 0, 123), 4 );
 //line( screne_image, scene_corners[3] + Point2f( static_cast<float>(source_image.cols), 0), scene_corners[0] + Point2f( static_cast<float>(source_image.cols), 0), Scalar( 255, 0, 123), 4 );
 //imshow("gg",screne_image);
    return Rect(x,y,width,height);
}
 Rect ImageLocate_CPU::getImageLocate1(void* _pMat)
 {
    return Rect();
 }
