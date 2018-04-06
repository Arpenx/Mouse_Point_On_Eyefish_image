#ifndef CONFIG_H
#define CONFIG_H
#include <opencv2/opencv.hpp>
extern cv::Mat image;
extern float f;
void cart2sph(cv::Mat d_x,cv::Mat d_y,cv::Mat d_z,cv::Mat &d_theta,cv::Mat &d_phi);
void rotate3D(float angle_x,float angle_y,float angle_z,cv::Mat &xx,cv::Mat &yy,cv::Mat &zz);
void rotateAnglexyz(const int col,const int row,cv::Point2f point,float &angle_x,float &angle_z);
void sph2cart(cv::Mat theta,cv::Mat phi,cv::Mat &r_x,cv::Mat &r_y,cv::Mat &r_z);
void meshgrid(const cv::Range &xgv,const cv::Range &ygv,cv::Mat &X,cv::Mat &Y);
void cal_mapping(int row,int col,int (&parals)[2],cv::Mat &mapx,cv::Mat &mapy,cv::Point point);
void onMouse(int event,int x,int y,int flags,void* ustc);

#endif // CONFIG_H
