#include <ros/ros.h>
#include <nav_msgs/Path.h>
#include "hannrs_msgs/People.h"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <fstream>

// cv::Mat img_map = cv::imread("/home/mateus/hannrs_catkin/src/hannrs_maps/maps/hannrs_LRM-nav.pgm", CV_LOAD_IMAGE_COLOR);
cv::Mat img_map = cv::imread("/home/mateus/map0.png", CV_LOAD_IMAGE_COLOR);
cv::Mat original = img_map.clone();

cv::Point2f om(-21.2, -10.0);
double res = 0.05;

std::vector<cv::Point2f> robot_path;
std::vector<cv::Point2f> person_path;
bool fst_path = true, snd_path = false;
bool plot_person = true;

int c = 0;
int e = 0;

void pathCallback(const nav_msgs::Path::ConstPtr& msg){
  
  if(fst_path || snd_path){
    robot_path.clear();
    for(unsigned int i = 0; i < msg->poses.size(); i++){
      cv::Point2f p;
      p.x = (msg->poses[i].pose.position.x - om.x)/0.05;
      p.y = img_map.rows - (msg->poses[i].pose.position.y - om.y)/0.05;
      robot_path.push_back(p);
    }

    for(int i = 0; i < (int)robot_path.size() - 1; i++){
      if(c == 0){
	cv::line(img_map, robot_path[i], robot_path[i+1], cv::Scalar(0,255,0));
      }else{
	cv::line(img_map, robot_path[i], robot_path[i+1], cv::Scalar(255,0,0));
      }
    }
    
    fst_path = false;
    snd_path = false;
  }
  
  c++;

}

void peopleCallback(const hannrs_msgs::People::ConstPtr& msg){

  if(!fst_path && plot_person){
    for(unsigned int i = 0; i < msg->people.size(); i++){
      cv::Point2f p;
      p.x = (msg->people[i].pose.position.x - om.x)/0.05;
      p.y = img_map.rows - (msg->people[i].pose.position.y - om.y)/0.05;
      person_path.push_back(p);
    }

    for(int i = 0; i < (int)person_path.size() - 1; i++){
      cv::line(img_map, person_path[i], person_path[i+1], cv::Scalar(0,0,255));
    }
    
    
//     if(c < 10){
//       snd_path = true;
//     }
    e++;
    
//     cv::circle(img_map, person_path[0], 5, cv::Scalar(0,0,255), 1, 8,0);
//     plot_person = false;
  }
  
}


static void onMouse( int event, int x, int y, int flags, void* ){
  
  if(event == cv::EVENT_LBUTTONDOWN){
    imwrite("/home/mateus/map0.png", img_map);
  }
  
}

int main(int argc, char* argv[]){
  ros::init(argc, argv, "paths");
  
  ros::NodeHandle nh;
  ros::Subscriber path_sub = nh.subscribe("/mbot05/move_base/Astar_planner/plan", 1, pathCallback);
  ros::Subscriber people_sub = nh.subscribe("/hannrs/people", 1, peopleCallback);
  
  cv::namedWindow("map", cv::WINDOW_NORMAL);
  
  cv::setMouseCallback( "map", onMouse, 0 );
  
  ros::Rate loop(5);
  while(ros::ok()){
    cv::imshow("map", img_map);
    cv::waitKey(3);
    ros::spinOnce();
    loop.sleep();
  }
  
  cv::destroyAllWindows();
  return 0;
}
