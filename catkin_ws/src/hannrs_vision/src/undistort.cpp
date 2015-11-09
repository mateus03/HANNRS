#include "hannrs_vision/undistort.h"

Undistorter::Undistorter(){
  std::string calib_file;
  
  ros::NodeHandle pnh("~");
  pnh.getParam("calib_file", calib_file);
  
  //load calib
  cv::FileStorage fileS(calib_file.c_str(), cv::FileStorage::READ);
  fileS["ss"] >> fisheye_model.pol;
  fileS["invpol"] >> fisheye_model.invpol;
  fileS["center_row"] >> fisheye_model.xc;
  fileS["center_column"] >> fisheye_model.yc;
  fileS["c"] >> fisheye_model.c;
  fileS["d"] >> fisheye_model.d;
  fileS["e"] >> fisheye_model.e;
  fileS["height"] >> fisheye_model.height;
  fileS["width"] >> fisheye_model.width;
  fisheye_model.length_pol = fisheye_model.pol.size();
  fisheye_model.length_invpol = fisheye_model.invpol.size();
  fileS.release();
  
  create_undistort_map();
  
}
  Undistorter::~Undistorter(){}
  
void Undistorter::create_undistort_map(){
  CvMat* mapx_persp = cvCreateMat(fisheye_model.height, fisheye_model.width, CV_32FC1);
  CvMat* mapy_persp = cvCreateMat(fisheye_model.height, fisheye_model.width, CV_32FC1);
  create_perspecive_undistortion_LUT( mapx_persp, mapy_persp);
  mapx = cv::Mat(mapx_persp);
  mapy = cv::Mat(mapy_persp);
}
  
void Undistorter::create_perspecive_undistortion_LUT( CvMat *mapx, CvMat *mapy){
  float Nxc = mapx->rows/2.0;
  float Nyc = mapx->cols/2.0;
  float Nz  = -mapx->cols/10;
  double M[3];
  double m[2];
  double norm;
  double theta;
  double t, t_i;
  double rho, x, y;
  double invnorm;
  int j,k;
  
  for (int i = 0; i < mapx->rows; i++)
    for (j = 0; j < mapx->cols; j++){   
      M[0] = (i - Nxc);
      M[1] = (j - Nyc);
      M[2] = Nz;
      
      norm = sqrt(M[0]*M[0] + M[1]*M[1]);
      theta = atan(M[2]/norm);
      
      if (norm != 0){
	invnorm = 1/norm;
	t  = theta;
	rho = fisheye_model.invpol[0];
	t_i = 1;

	for (k = 1; k < fisheye_model.length_invpol; k++){
	  t_i *= t;
	  rho += t_i*fisheye_model.invpol[k];
	}
	
	x = M[0]*invnorm*rho;
	y = M[1]*invnorm*rho;
      
	m[0] = x*fisheye_model.c + y*fisheye_model.d + fisheye_model.xc;
	m[1] = x*fisheye_model.e + y   + fisheye_model.yc;
      }else{
	m[0] = fisheye_model.xc;
	m[1] = fisheye_model.yc;
      }
      
      *( mapx->data.fl + i*mapx->cols+j ) = (float) m[1];
      *( mapy->data.fl + i*mapx->cols+j ) = (float) m[0];
    }
}

cv::Mat Undistorter::undistortIm(cv::Mat im){
  cv::Mat undistorted_im;
  cv::remap(im, undistorted_im, mapx, mapy, cv::INTER_LINEAR, cv::BORDER_TRANSPARENT, cvScalarAll(0));
  
  return undistorted_im;
  
}

cv::Point2f Undistorter::image_center(){
  return cv::Point2f(fisheye_model.width/2.0, fisheye_model.height/2.0);
}

cv::Size2f Undistorter::imageSize(){
  return cv::Size2f(fisheye_model.width, fisheye_model.height);
}