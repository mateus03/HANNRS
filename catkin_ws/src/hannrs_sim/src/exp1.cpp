#include "ros/ros.h"
#include "std_msgs/String.h"
#include <nav_msgs/Odometry.h>
#include <hannrs_msgs/People.h>
#include <std_msgs/Time.h>
#include <fstream>
#include <tf/transform_listener.h>
#include <geometry_msgs/Pose.h>

using namespace std;

std_msgs::Time begin;
geometry_msgs::Pose initial_pose;

ofstream myfile("det_test1.txt", ios::out | ios::binary);

int c = 0;

void chatterCallback(const hannrs_msgs::People::ConstPtr& msg){
  
  if(!msg->people.empty()){
  
    if(c == 0){
      initial_pose.position.x = msg->people[0].pose.position.x;
      initial_pose.position.y = msg->people[0].pose.position.y;
    }
    
    myfile << msg->header.stamp.toSec() - begin.data.toSec() << " " << msg->people[0].pose.position.x - initial_pose.position.x << 
    " " << msg->people[0].pose.position.y - initial_pose.position.y;
    myfile << "\n";
    
    c++;
  }
}

int main(int argc, char **argv)
{

  ros::init(argc, argv, "listener");

  ros::NodeHandle n;

  ros::Subscriber sub = n.subscribe("/hanp/people", 1, chatterCallback);

  begin.data = ros::Time::now();
  
  tf::StampedTransform transform; 
  tf::TransformListener listener;

  try{
    ros::Time now = ros::Time::now();
    listener.waitForTransform("/map", "/base_link", ros::Time(0), ros::Duration(0.5));
    listener.lookupTransform("/map", "/base_link", ros::Time(0), transform);
  }
  catch (tf::TransformException &ex) {
    ROS_ERROR("%s",ex.what());
    ros::Duration(1.0).sleep();
  }
  
  initial_pose.position.x = transform.getOrigin().x();
  initial_pose.position.y = transform.getOrigin().y();
  
  ros::spin();

  myfile.close();
  return 0;
}