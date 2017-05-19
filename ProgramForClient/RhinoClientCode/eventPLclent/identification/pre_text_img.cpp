#include "pre_text_img.h"




void heartStone_friendList_color(Mat& srcImage,Mat& resultImage)
{
    Mat hsvmat;
    cvtColor(srcImage,hsvmat, CV_BGR2HSV);
    Mat temp = hsvmat.clone();

    int width = srcImage.cols;
    int height = srcImage.rows;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            char* pix_hsv = (char*)hsvmat.data+ (i*width+j)*3;
            //CvScalar s_hsv = cvGet2D(hsvmat, i, j);//获取像素点为（j, i）点的HSV的值
            /*
                opencv 的H范围是0~180，红色的H范围大概是(0~8)∪(160,180)
                S是饱和度，一般是大于一个值,S过低就是灰色（参考值S>80)，
                V是亮度，过低就是黑色，过高就是白色(参考值220>V>50)。
            */

            char* pix_hsv_out = (char*)temp.data+ (i*width+j)*3;
            //if(!(((pix_hsv[0]>0)&&(pix_hsv[0]<8))||(pix_hsv[0]>120)&&(pix_hsv[0]<180)))
            if((pix_hsv[0]>90)&&(pix_hsv[0]<100) )
            {
                pix_hsv_out[0] = pix_hsv[0];
                pix_hsv_out[1] = pix_hsv[1];
                pix_hsv_out[2] = pix_hsv[2];

            }
            else
            {
                pix_hsv_out[0] = 0;
                pix_hsv_out[1] = 0;
                pix_hsv_out[2] = 0;

            }
        }
    }
    cvtColor(temp,resultImage, CV_HSV2BGR);
}

vector<Rect> get_heartStone_friendName_rect(Mat& _image)
{
    //1- 进行模糊处理，去除噪音 高斯模糊
    Mat gaussImg;
    cv::GaussianBlur(_image,gaussImg,cv::Size(7,7),1.5);

    //2- 轮廓查询 canny
    //gaussImg
    Mat edgeImg;
    int canny_1 = 150;
    int canny_2 = 100;
    Canny(gaussImg,edgeImg,canny_1,canny_2,3);


    //3- 文字轮廓较细，顾 采用两次 腐蚀
    Mat close_dilate_img;
    Mat element = getStructuringElement(CV_SHAPE_RECT,Size(16,4));
    dilate(edgeImg,close_dilate_img,element);

    //4- 查找有效轮廓
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;

    //找出轮廓--
    findContours(close_dilate_img,contours,hierarchy,RETR_LIST,CHAIN_APPROX_SIMPLE,Point(0,0));

    vector<vector<Point>> contours_poly(contours.size());
    vector<Rect> boundRect(contours.size());
    for(int i=0;i<contours.size();i++)
    {
        approxPolyDP(Mat(contours[i]),contours_poly[i],3,true);
        //juxing
        boundRect[i] = boundingRect(Mat(contours_poly[i]));
    }


    vector<Rect> resultVector;
   // Mat drawing = Mat::zeros(thresholdImg.size(),CV_8UC3);
    for(int i=0;i<contours.size();i++)
    {
        Scalar color = Scalar(128,0,255);
        //绘制轮廓
        //drawContours(srcImage,contours_poly,i,color,1,8,vector<Vec4i>(),0,Point());
        //绘制矩形
        Rect& _rect = boundRect[i];
        if(_rect.width<35/*||_rect.width>200*/)
            continue;
        if(_rect.height > 55)
            continue;
		
		_rect.x += 0;
		_rect.y -= 1;
        _rect.width = _image.cols*0.5+10;
        _rect.height= _image.rows*0.1;
        resultVector.push_back(_rect);

		//rectangle(srcImage,boundRect[i].tl(),boundRect[i].br(),color,2,8,0);
    }

    return resultVector;

}

void mserPro(Mat& sourceImg)
{
    //创建MSER类
    MSER ms;
    //用于组块区域的像素点集
    vector<vector<Point>> regions;
    ms(sourceImg, regions, Mat());
    //在灰度图像中用椭圆形绘制组块
    for (int i = 0; i < regions.size(); i++)
    {
        ellipse(sourceImg, fitEllipse(regions[i]), Scalar(255));
    }
    //imshow("mser", sourceImg);
}

void Preprocess_canny( Mat& sourceImg,Mat &edgeImg,int canny_1 )
{

    Canny(sourceImg,edgeImg,canny_1,canny_1*3,3);
    //imwrite("result/4_Canny_image.jpg", edgeImg);
    //upImage(edgeImg);
}

void colorFilter(Mat& inputImage, Mat& outputImage)
{
    Mat hsvmat;
    cvtColor(inputImage,hsvmat, CV_BGR2HSV);
    Mat temp = hsvmat.clone();

    int width = inputImage.cols;
    int height = inputImage.rows;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            char* pix_hsv = (char*)hsvmat.data+ (i*width+j)*3;
            //CvScalar s_hsv = cvGet2D(hsvmat, i, j);//获取像素点为（j, i）点的HSV的值
            /*
                opencv 的H范围是0~180，红色的H范围大概是(0~8)∪(160,180)
                S是饱和度，一般是大于一个值,S过低就是灰色（参考值S>80)，
                V是亮度，过低就是黑色，过高就是白色(参考值220>V>50)。
            */

            char* pix_hsv_out = (char*)temp.data+ (i*width+j)*3;
            //if(!(((pix_hsv[0]>0)&&(pix_hsv[0]<8))||(pix_hsv[0]>120)&&(pix_hsv[0]<180)))
            if((pix_hsv[0]>70)&&(pix_hsv[0]<120) )
            {
                pix_hsv_out[0] = pix_hsv[0];
                pix_hsv_out[1] = pix_hsv[1];
                pix_hsv_out[2] = pix_hsv[2];

            }
            else
            {
                pix_hsv_out[0] = 0;
                pix_hsv_out[1] = 0;
                pix_hsv_out[2] = 0;

            }
        }
    }
    cvtColor(temp,outputImage, CV_HSV2BGR);
}


void colorFilterGay(Mat& inputImage, Mat& outputImage,int grayLevel1,int grayLevel2)
{

    outputImage = inputImage.clone();
    int width = inputImage.cols;
    int height = inputImage.rows;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            char* pix_hsv = (char*)inputImage.data+ (i*width+j)*1;
            //CvScalar s_hsv = cvGet2D(hsvmat, i, j);//获取像素点为（j, i）点的HSV的值
            /*
                opencv 的H范围是0~180，红色的H范围大概是(0~8)∪(160,180)
                S是饱和度，一般是大于一个值,S过低就是灰色（参考值S>80)，
                V是亮度，过低就是黑色，过高就是白色(参考值220>V>50)。
            */

            char* pix_hsv_out = (char*)outputImage.data+ (i*width+j)*1;
            //if(!(((pix_hsv[0]>0)&&(pix_hsv[0]<8))||(pix_hsv[0]>120)&&(pix_hsv[0]<180)))
            //if(pix_hsv[0] > grayLevel1 && pix_hsv[0] < grayLevel2)
            if(pix_hsv[0] < 30)
            {
                pix_hsv_out[0] = pix_hsv[0];

            }
            else
            {
                pix_hsv_out[0] = 0;
            }
        }
    }

}

void findTextRegion(Mat srcMat,vector<Rect>& rect)
{

}
void constrastAndBright(Mat& srcImag,Mat& desImag,int constrast,int bright)
{
    if(srcImag.data)
    {

        for(int y=0;y<srcImag.rows;y++ )
        {
            for(int x = 0; x< srcImag.cols;x++)
            {
                for(int c=0;c<3;c++)
                {
                      desImag.at<Vec3b>(y,x)[c]=saturate_cast<uchar>((constrast*0.01)*(srcImag.at<Vec3b>(y,x)[c])+bright);
                }

            }
        }
    }
}
