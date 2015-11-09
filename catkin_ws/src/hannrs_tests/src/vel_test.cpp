#include "ros/ros.h"
#include <nav_msgs/Odometry.h>
#include <std_msgs/Time.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <rosgraph_msgs/Clock.h>
#include <tf/transform_listener.h>

#include <fstream>

std::vector<double> t;
std::ofstream pos_data;
std::ofstream vel_data;

// void clockCallback(const rosgraph_msgs::Clock::ConstPtr& msg){
//   if(t.empty()){
//     t.push_back(msg->clock.toSec());
//   }
// }

void poseCallback(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr& msg){
  
//   double dist = sqrt((msg->pose.pose.position.x + 7)*(msg->pose.pose.position.x + 7) + (msg->pose.pose.position.y - 11)*(msg->pose.pose.position.y - 11));
//   pos_data << msg->header.stamp.toSec() - t[0] << " " << dist << std::endl;

  pos_data << msg->pose.pose.position.x << " " << msg->pose.pose.position.y << " " << tf::getYaw(msg->pose.pose.orientation) << std::endl;
}

void odomCallback(const nav_msgs::Odometry::ConstPtr& msg){
  
//   double vel_module = sqrt(msg->twist.twist.linear.x*msg->twist.twist.linear.x + msg->twist.twist.linear.y*msg->twist.twist.linear.y);
//   vel_data << msg->header.stamp.toSec() - t[0] << " " << vel_module << std::endl;
}

int main(int argc, char* argv[]){
  ros::init(argc, argv, "listener");

  ros::NodeHandle n;

//   ros::Subscriber clock_sub = n.subscribe("/clock", 1, clockCallback); 
  ros::Subscriber odom_sub = n.subscribe("/mbot05/odom", 1, odomCallback);
  ros::Subscriber pose_sub = n.subscribe("/mbot05/amcl_pose", 1, poseCallback);
  
  vel_data.open("/home/mateus/vel_module.txt");
  pos_data.open("/home/mateus/dist_goal.txt");
  
  ros::spin();
  
  pos_data.close();
  vel_data.close();
  return 0;
}