#include <ros/ros.h>
#include <hannrs_msgs/Goal.h>
#include <geometry_msgs/Pose.h>
#include <tf/transform_listener.h>
#include <tf/transform_datatypes.h>
#include <angles/angles.h>

int main(int argc, char* argv[]){
  ros::init(argc, argv, "goal_talker");
  ros::NodeHandle nh;
  
  ros::Publisher goal_pub = nh.advertise<hannrs_msgs::Goal>("hannrs/goal", 1);
  hannrs_msgs::Goal goal_msg;

  goal_msg.header.stamp = ros::Time::now();
  goal_msg.action = std::string("go to");
  goal_msg.person = std::string("noone");
  goal_msg.object = std::string("nothing");
  goal_msg.pose.position.x = 1.0;
  goal_msg.pose.orientation.w = 1.0;
  
  ros::Rate loop_rate(1);
//   int count = 0;
//   while(ros::ok()){
//     if(count == 10){
//       goal_pub.publish(goal_msg);
//       goal_msg.action = std::string("go to");
//     }
//     ros::spinOnce();
//     loop_rate.sleep();
//     ++count;
//   }
  
  while(goal_pub.getNumSubscribers() == 0)
    loop_rate.sleep();
  
  goal_pub.publish(goal_msg);

  return 0;
}