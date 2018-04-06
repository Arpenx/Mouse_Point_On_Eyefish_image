#include <iostream>
#include "config.h"
//using namespace std;
//using namespace cv;

float R=300.5f;
float r=65.5f;
float thetaMax=89.0f*CV_PI/180.0f;
float f=R/thetaMax;
float thetaMin=r/f;

cv::Mat image;

int main()
{
    image=cv::imread("/Users/arpenx/Documents/project/eyeFish_images/camera.bmp");
    if(!image.data)
    {
        std::cout<<"open image file is false!"<<std::endl;
        exit(-1);
    }
    cv::namedWindow("srcImage",0);
    cv::imshow("srcImage",image);

    cv::setMouseCallback("srcImage",onMouse,0);
    cv::waitKey();
    return 0;
}
