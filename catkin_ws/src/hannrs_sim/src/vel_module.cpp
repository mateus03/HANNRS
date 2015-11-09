#include "ros/ros.h"
#include <nav_msgs/Odometry.h>
#include <std_msgs/Time.h>

#include <fstream>

std::ofstream pos_data;
std::vector<double> t;

void odomCallback(const nav_msgs::Odometry::ConstPtr& msg){
  if(t.empty()){
    t.push_back( msg->header.stamp.toSec());
  }
  
  double vel_module = sqrt(msg->twist.twist.linear.x*msg->twist.twist.linear.x + msg->twist.twist.linear.y*msg->twist.twist.linear.y);
  pos_data << msg->header.stamp.toSec() - t[0] << " " << vel_module << std::endl;
}

int main(int argc, char* argv[]){
  ros::init(argc, argv, "listener");

  ros::NodeHandle n;

  ros::Subscriber sub = n.subscribe("/mbot05/odom", 1, odomCallback);
  
  pos_data.open("/home/mateus/vel_module.txt");
  
  ros::spin();
  
  pos_data.close();
  
  return 0;
}