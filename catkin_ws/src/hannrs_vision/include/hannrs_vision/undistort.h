#ifndef _UNDISTORT_H
#define _UNDISTORT_H

//ROS
#include <ros/ros.h>
//OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

typedef struct{
  std::vector<double> pol;    // the polynomial coefficients: pol[0] + x"pol[1] + x^2*pol[2] + ... + x^(N-1)*pol[N-1]
  int length_pol;                // length of polynomial
  std::vector<double> invpol; // the coefficients of the inverse polynomial
  int length_invpol;             // length of inverse polynomial
  double xc;         // row coordinate of the center
  double yc;         // column coordinate of the center
  double c;          // affine parameter
  double d;          // affine parameter
  double e;          // affine parameter
  double width;         // image width
  double height;        // image height
}ocam_model;

class Undistorter{
protected:
  ocam_model fisheye_model;
  cv::Mat mapx, mapy;
  
  void create_perspecive_undistortion_LUT( CvMat *mapx, CvMat *mapy);
  void create_undistort_map();
  
public:
  Undistorter();
  ~Undistorter();
  
  cv::Mat undistortIm(cv::Mat im);
  cv::Point2f image_center();
  cv::Size2f imageSize();
};

#endif
