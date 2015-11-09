//ROS
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
//OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "hannrs_vision/people_detector.h"

// This is a function, not a class method
void wrappedCallBackFunc(int event, int x, int y, int flags, void* ptr){
    PeopleDetector* mcPtr = (PeopleDetector*)ptr;
    if(mcPtr != NULL)
        mcPtr->CallBackFunc(event, x, y, flags);
}

int main(int argc, char* argv[]){
  ros::init(argc, argv, "vision_node");
  
  PeopleDetector* pd = new PeopleDetector();
  
  //set the callback function for any mouse event
  cv::setMouseCallback("undistorted image", wrappedCallBackFunc, (void*)pd);
  
  ros::Time t;
  double elapsed;
  ros::Rate loop(12);
  while(ros::ok()){ 
//     t = ros::Time::now();
    
    pd->detect();
    
    pd->showImage();

    pd->spinOnce();
    loop.sleep();
/*    
    elapsed = ros::Time::now().toSec() - t.toSec();
    std::cout << elapsed << std::endl;*/

  }
  
  delete pd;
  return 0;
}
