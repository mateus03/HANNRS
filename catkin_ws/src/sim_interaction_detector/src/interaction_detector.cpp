#include <ros/ros.h>
#include <hannrs_msgs/InteractionSet.h>
#include <hannrs_msgs/InteractionSets.h>
#include <gazebo_msgs/ModelStates.h>
#include <geometry_msgs/Point.h>
#include <std_msgs/Float64.h>
#include <std_msgs/Time.h>
#include <cmath>
#include <std_msgs/Bool.h>

geometry_msgs::Pose person_pose;
geometry_msgs::Pose object_pose;
std_msgs::Time elapsed;
bool tv;

void tvCallback(const std_msgs::Bool::ConstPtr& msg){
  tv = msg->data;
}

void gazeboCallBack(const gazebo_msgs::ModelStates::ConstPtr& msg){
  
  if(msg->name.size() == msg->pose.size() && msg->name.size() == msg->twist.size()){
    for(int i = 0; i < (int)msg->name.size(); i++){
      if(msg->name[i].compare("Quim") == 0){
	person_pose = msg->pose[i];
      }else if(msg->name[i].compare("tv") == 0){
	object_pose = msg->pose[i];
      }
    }
  }
}

hannrs_msgs::InteractionSets fill_msg(){
  hannrs_msgs::InteractionSet itset;
  std::vector<hannrs_msgs::InteractionSet> itset_list;
  hannrs_msgs::InteractionSets msg;
  geometry_msgs::Point center;
  std_msgs::Float64 radius;
  std_msgs::Float64 importance;
  std_msgs::Header head;
 
//   double dt;
//   dt = ros::Time::now().toSec() - elapsed.data.toSec();
  
  if(tv){
    center.x = (person_pose.position.x + object_pose.position.x)/2;
    center.y = (person_pose.position.y + object_pose.position.y)/2;
    center.z = 0;
    
    radius.data = sqrt(pow(person_pose.position.x - object_pose.position.x, 2) + pow(person_pose.position.y - object_pose.position.y, 2))/2;
    
    importance.data = 1.0;
    
    head.stamp = ros::Time::now();
    head.frame_id = "/map";
    
    itset.center = center;
    itset.radius = radius;
    itset.importance = importance;
    
    itset_list.push_back(itset);
    
    msg.header = head;
    msg.interaction_sets = itset_list;
    
    return msg;
//   }else{
//     head.stamp = ros::Time::now();
//     head.frame_id = "/map";
//     
//     itset.center = center;
//     itset.radius = radius;
//     itset.importance = importance;
//     
//     itset_list.clear();
//     
//     msg.header = head;
//     msg.interaction_sets = itset_list;
    
//     return msg;
  }
}

int main(int argc, char* argv[]){
  
  ros::init(argc, argv, "interaction_detector");
  ros::NodeHandle n; 
  ros::Subscriber sub = n.subscribe("/gazebo/model_states", 1, &gazeboCallBack);
  ros::Subscriber tv_sub = n.subscribe("/hannrs/TV", 1, &tvCallback);
  ros::Publisher itset_pub = n.advertise<hannrs_msgs::InteractionSets>("hannrs/interaction_sets", 1); 
  hannrs_msgs::InteractionSets pub_msg;
  
  ros::Rate loop_rate(5);
  
//   elapsed.data = ros::Time::now();
  
  while(ros::ok()){
    pub_msg = fill_msg();
    itset_pub.publish(pub_msg);
    ros::spinOnce();
    loop_rate.sleep();
  }
  
  return 0; 
}