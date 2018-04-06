#include "config.h"

void meshgrid(const cv::Range &xgv,const cv::Range &ygv,cv::Mat &X,cv::Mat &Y)
{
    std::vector<float> v_x,v_y;
    for(int i=xgv.start;i<=xgv.end;i++)  v_x.push_back(float(i));
    for(int j=ygv.start;j<=ygv.end;j++)  v_y.push_back(float(j));
    cv::repeat(cv::Mat(v_x).t(),v_y.size(),1,X);
    cv::repeat(cv::Mat(v_y),1,v_x.size(),Y);
}

void sph2cart(cv::Mat theta,cv::Mat phi,cv::Mat &r_x,cv::Mat &r_y,cv::Mat &r_z)
{
    int r=1;
    for(int i=0;i<theta.rows;i++)
        for(int j=0;j<theta.cols;j++)
        {
            if(!theta.at<float>(i,j)==0 && !phi.at<float>(i,j)==0)
            {
            r_y.at<float>(i,j)=r*sin(theta.at<float>(i,j))*sin(phi.at<float>(i,j));
            r_x.at<float>(i,j)=r*sin(theta.at<float>(i,j))*cos(phi.at<float>(i,j));
            r_z.at<float>(i,j)=r*cos(theta.at<float>(i,j));
            }
        }
}
void rotateAnglexyz(const int col,const int row,cv::Point2f point,float &angle_x,float &angle_z)
{
     float p_x=point.x-col/2;
     float p_y=point.y-row/2;
     if(p_x>0)
     {
         if(p_y>0)
         {
             angle_z=CV_PI/2-atan2(p_y,p_x);
             angle_x=sqrt(p_x*p_x+p_y*p_y)/f;
         }
         else
         {
             angle_z=-(CV_PI/2-atan2(-p_y,p_x));
             angle_x=-sqrt(p_x*p_x+p_y*p_y)/f;
         }
     }
     else
     {
         if(p_y>0)
         {
             angle_z=-(CV_PI/2-atan2(p_y,-p_x));
             angle_x=sqrt(p_x*p_x+p_y*p_y)/f;
         }
         else{
             angle_z=-(CV_PI/2-atan2(-p_y,-p_x));
             angle_x=-sqrt(p_x*p_x+p_y*p_y)/f;
         }
     }
}

void rotate3D(float alpha,float beta,float gamma,cv::Mat &Rx,cv::Mat &Ry,cv::Mat &Rz)
{
    Rx.at<float>(0, 0) = 1; Rx.at<float>(0, 1) = 0;			  Rx.at<float>(0, 2) = 0;
    Rx.at<float>(1, 0) = 0; Rx.at<float>(1, 1) = cos(alpha);  Rx.at<float>(1, 2) = sin(alpha);
    Rx.at<float>(2, 0) = 0; Rx.at<float>(2, 1) = -sin(alpha); Rx.at<float>(2, 2) = cos(alpha);

    Ry.at<float>(0, 0) = cos(beta); Ry.at<float>(0, 1) = 0;	Ry.at<float>(0, 2) = -sin(beta);
    Ry.at<float>(1, 0) = 0;			Ry.at
        <float>(1, 1) = 1; Ry.at<float>(1, 2) = 0;
    Ry.at<float>(2, 0) = sin(beta); Ry.at<float>(2, 1) = 0; Ry.at<float>(2, 2) = cos(beta);

    Rz.at<float>(0, 0) = cos(gamma);  Rz.at<float>(0, 1) = sin(gamma); Rz.at<float>(0, 2) = 0;
    Rz.at<float>(1, 0) = -sin(gamma); Rz.at<float>(1, 1) = cos(gamma); Rz.at<float>(1, 2) = 0;
    Rz.at<float>(2, 0) = 0;			  Rz.at<float>(2, 1) = 0;		   Rz.at<float>(2, 2) = 1;
}

void cart2sph(cv::Mat d_x,cv::Mat d_y,cv::Mat d_z,cv::Mat &d_theta,cv::Mat &d_phi)
{
    for(int i=0;i<d_x.rows;i++)
        for(int j=0;j<d_x.cols;j++)
        {

            if(d_x.at<float>(i,j) || d_y.at<float>(i,j) || d_z.at<float>(i,j) )
            {
                 d_phi.at<float>(i,j)=atan2(d_y.at<float>(i,j),d_x.at<float>(i,j));
                 d_theta.at<float>(i,j)=atan2(sqrt(d_x.at<float>(i,j)*d_x.at<float>(i,j)
                                                   +d_y.at<float>(i,j)*d_y.at<float>(i,j)),
                                                   d_z.at<float>(i,j));
            }

        }
}
//void cart2sph(const cv::Mat x, const cv::Mat y, const cv::Mat z, cv::Mat &theta,cv::Mat &phi)/*由3D空间直角坐标转3D的两夹角*/
//{
//    for (int j = 0; j < x.cols; j++)
//    for (int i = 0; i < x.rows; i++)
//    {
//        float r = sqrt(x.at<float>(i, j)*x.at<float>(i, j) + y.at<float>(i, j)*y.at<float>(i, j)
//                        + z.at<float>(i, j)*z.at<float>(i, j));
//        if (r != 0)
//        {
//            phi.at<float>(i, j) = atan2(y.at<float>(i, j), x.at<float>(i, j));/*2D上与x轴的夹角*/
//            theta.at<float>(i, j) = acos(z.at<float>(i, j) / r);/*球面上点与球心连线与z轴的夹角*/
//        }
//    }
//}
void cal_mapping(int row,int col,int (&parals)[2],cv::Mat &mapx,cv::Mat &mapy,cv::Point point)
{
     int width=parals[0];
     int height=parals[1];
     cv::Mat X(row,col,CV_32FC1,cv::Scalar(0));
     cv::Mat Y(row,col,CV_32FC1,cv::Scalar(0));

     meshgrid(cv::Range(1,col),cv::Range(1,row),X,Y);
     //center point
     cv::Mat c_x=X-col/2;
     cv::Mat c_y=Y-row/2;

     cv::Mat temp(row,col,CV_32FC1,cv::Scalar(0));
     for(int i=0;i<row;i++)
         for(int j=0;j<col;j++)
         {
             if(c_x.at<float>(i,j) < width/2 && c_x.at<float>(i,j) > -width/2 &&
                     c_y.at<float>(i,j) < height/2 && c_y.at<float>(i,j) > -height/2)
             {
                 temp.at<float>(i,j)=1;
//                 std::cout<<"("<<i<<","<<j<<")"<<std::endl;
             }
         }
     //the source image (rou,phi) Polar coordinates
     cv::Mat rou(row,col,CV_32FC1,cv::Scalar(0));
     cv::Mat phi(row,col,CV_32FC1,cv::Scalar(0));
     //Cartesian coordinate system to Polar coordinates
     cv::cartToPolar(c_x,c_y,rou,phi);

     cv::Mat theta(row,col,CV_32FC1,cv::Scalar(0));

     for(int i=0;i<row;i++)
         for(int j=0;j<col;j++)
         {
//             theta.at<float>(i,j)=rou.at<float>(i,j)/f;
             theta.at<float>(i,j)=atan2(rou.at<float>(i,j),f);
         }
     cv::Mat r_x(row,col,CV_32FC1,cv::Scalar(0));
     cv::Mat r_y(row,col,CV_32FC1,cv::Scalar(0));
     cv::Mat r_z(row,col,CV_32FC1,cv::Scalar(0));
     //Polar coordinates to Cartesian coordinate system
     sph2cart(theta,phi,r_x,r_y,r_z);

     float angle_x,angle_z;
     //rotate angle by ox and oz
     rotateAnglexyz(col,row,point,angle_x,angle_z);
     std::cout<<"angle_x:"<<angle_x<<std::endl;
     std::cout<<"angle_z:"<<angle_z<<std::endl;

     cv::Mat xx(3,3,CV_32FC1);
     cv::Mat yy(3,3,CV_32FC1);
     cv::Mat zz(3,3,CV_32FC1);

     rotate3D(-angle_x,0,-angle_z,xx,yy,zz);

     std::cout<<"xx"<<xx<<std::endl;
     std::cout<<"yy"<<yy<<std::endl;
     std::cout<<"zz"<<zz<<std::endl;

     r_x=r_x.mul(temp);
     r_y=r_y.mul(temp);
     r_z=r_z.mul(temp);

     cv::Mat xyz(row*col,3,r_x.type());
     cv::Mat r_xx=r_x.reshape(0,row*col);
     cv::Mat r_yy=r_y.reshape(0,row*col);
     cv::Mat r_zz=r_z.reshape(0,row*col);

     r_xx.copyTo(xyz.col(0));
     r_yy.copyTo(xyz.col(1));
     r_zz.copyTo(xyz.col(2));

     xyz=xyz*xx*zz;

     xyz.col(0).copyTo(r_xx);
     xyz.col(1).copyTo(r_yy);
     xyz.col(2).copyTo(r_zz);
     r_xx.reshape(0,row).copyTo(r_x);
     r_yy.reshape(0,row).copyTo(r_y);
     r_zz.reshape(0,row).copyTo(r_z);

//     cv::Mat d_x=d_xx.reshape(0,row);
//     cv::Mat d_y=d_yy.reshape(0,row);
//     cv::Mat d_z=d_zz.reshape(0,row);

     cv::Mat r_rou(row,col,CV_32FC1,cv::Scalar(0));
     cv::Mat r_phi(row,col,CV_32FC1,cv::Scalar(0));
     cv::Mat r_theta(row,col,CV_32FC1,cv::Scalar(0));

     cart2sph(r_x,r_y,r_z,r_theta,r_phi);

     for(int i=0;i<row;i++)
         for(int j=0;j<col;j++)
         {
           r_rou.at<float>(i,j)=f*r_theta.at<float>(i,j);
         }
//     cv::Mat nx,ny;
     cv::polarToCart(r_rou,r_phi,mapx,mapy);
//     mapx+=nx;
//     mapy+=ny;
//     std::cout<<r_rou<<std::endl;
//     std::cout<<r_phi<<std::endl;

}

void onMouse(int event,int x,int y,int flags,void* ustc)
{
    //width and higth
    int parals[]={image.cols/4,image.rows/4};
    int row=image.rows;
    int col=image.cols;
    cv::Mat mapx(row,col,CV_32FC1,cv::Scalar(0,0,0));
    cv::Mat mapy(row,col,CV_32FC1,cv::Scalar(0,0,0));
    if(event == CV_EVENT_LBUTTONDOWN)
    {
        cv::Point2f point(x,y);
        cal_mapping(row,col,parals,mapx,mapy,point);
        mapx+=col/2;
        mapy+=row/2;
        cv::Mat dstImage;
//        std::cout<<image<<std::endl;
        cv::remap(image,dstImage,mapx,mapy,CV_INTER_LINEAR,cv::BORDER_CONSTANT,cv::Scalar(0));
//        std::cout<<dstImage<<std::endl;
        cv::imshow("dstImage",dstImage);
    }
}
