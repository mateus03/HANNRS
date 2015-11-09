#include <ros/ros.h>
#include <nav_msgs/Path.h>
#include "hannrs_msgs/People.h"
#include <rosgraph_msgs/Clock.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <fstream>


std::vector<double> t;
geometry_msgs::Pose person;
geometry_msgs::Pose robot;

std::ofstream person_data;
std::ofstream robot_data;

double t1, t2;

void clockCallback(const rosgraph_msgs::Clock::ConstPtr& msg){
  if(t.empty()){
    t.push_back(msg->clock.toSec());
  }
}

void poseCallback(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr& msg){
  if(!t.empty()){
    robot = msg->pose.pose;
    t1 = msg->header.stamp.toSec() - t[0];
  }
}

void peopleCallback(const hannrs_msgs::People::ConstPtr& msg){

  if(!t.empty()){
    if(!msg->people.empty()){
      if(msg->people[0].status.compare("standing") == 0){
	t2 = msg->header.stamp.toSec() - t[0];
	person = msg->people[0].pose;
      }
    }
  }
  
}


int main(int argc, char* argv[]){
  ros::init(argc, argv, "paths");
  
  ros::NodeHandle nh;
  ros::Subscriber clock_sub = nh.subscribe("/clock", 1, clockCallback); 
  ros::Subscriber pose_sub = nh.subscribe("/mbot05/amcl_pose", 1, poseCallback);
  ros::Subscriber people_sub = nh.subscribe("/hannrs/people", 1, peopleCallback);
  
  person_data.open("/home/mateus/person_pose.txt");
  robot_data.open("/home/mateus/robot_pose.txt");
  
  ros::Rate loop(100);
  while(ros::ok()){
    
    if(person.position.x != 0 && person.position.y != 0){
      double dist = sqrt((person.position.x - robot.position.x)*(person.position.x - robot.position.x) + (person.position.y - robot.position.y)*(person.position.y - robot.position.y));
      robot_data << t1 << " " << dist << std::endl;
    }
    
//     person_data << t2 << " " << person.position.x << " " << person.position.y << std::endl;
//     robot_data << t1 << " " << robot.position.x << " " << robot.position.y << std::endl; 
    ros::spinOnce();
    loop.sleep();
  }
  
  
  person_data.close();
  robot_data.close();
  return 0;
}
