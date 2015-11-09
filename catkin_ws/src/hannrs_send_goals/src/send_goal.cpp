#include <ros/ros.h>
#include <hannrs_msgs/Prox_Goal.h>
#include <hannrs_msgs/Get_Person.h>
#include <hannrs_msgs/People.h>
#include <geometry_msgs/Quaternion.h>
#include <cstdlib>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <tf/transform_listener.h>
#include <angles/angles.h>

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

int main(int argc, char* argv[]){

  ros::init(argc, argv, "send_goals");
  ros::NodeHandle nh;
  ros::ServiceClient global_client = nh.serviceClient<hannrs_msgs::Prox_Goal>("/mbot05/move_base/global_costmap/proxemics/prox_goal");
  ros::ServiceClient local_client = nh.serviceClient<hannrs_msgs::Prox_Goal>("/mbot05/move_base/local_costmap/proxemics/prox_goal");
  ros::ServiceClient person_client = nh.serviceClient<hannrs_msgs::Get_Person>("/get_person");
  
  hannrs_msgs::Prox_Goal srv_global, srv_local;
  hannrs_msgs::Get_Person srv_person;
  
  double x_,y_,yaw_;
  
  std::string action_("hand over");
  std::string person_("Ze0");
  
  ros::NodeHandle private_nh("~");
  private_nh.param("action", action_, action_);
  private_nh.param("person", person_, person_);
  private_nh.param("x", x_, 0.0);
  private_nh.param("y", y_, 0.0);
  private_nh.param("yaw", yaw_, 0.0);
  
  std::cout << x_ << " " << y_ << std::endl;
  ROS_INFO("Action: %s Person: %s", action_.c_str(), person_.c_str());
  
  srv_global.request.action.data = action_;
  srv_global.request.person.data = person_;
  srv_local = srv_global;
  srv_person.request.name.data = person_;
  
  move_base_msgs::MoveBaseGoal goal;
  //tell the action client that we want to spin a thread by default
  MoveBaseClient ac("/mbot05/move_base", true);

  //wait for the action server to come up
  while(!ac.waitForServer(ros::Duration(5.0))){
    ROS_INFO("Waiting for the move_base action server to come up");
  }
  
  if(action_.compare("go to") == 0){
    goal.target_pose.header.frame_id = "/map";
    goal.target_pose.header.stamp = ros::Time::now();

    goal.target_pose.pose.position.x = x_;
    goal.target_pose.pose.position.y = y_;
    
    goal.target_pose.pose.orientation = tf::createQuaternionMsgFromYaw(yaw_);
	
  }else{

    if(person_client.call(srv_person)){
      geometry_msgs::Pose pose = srv_person.response.pose;
      std::string status = srv_person.response.status.data;
      
      if(status.compare("walking") == 0){
	ROS_ERROR("Cannot perform action %s on person %s", action_.c_str(), person_.c_str());
	return 4;
      }
      
      if(global_client.call(srv_global) && local_client.call(srv_local)){
	//wait for costmap to update
	std::cout << srv_global.response.wait_time.data << std::endl;
	ros::Rate wait(std::max(srv_global.response.wait_time.data, srv_local.response.wait_time.data));
	wait.sleep();

	if(action_.compare("speak to") == 0){
	  double yaw = tf::getYaw(pose.orientation);
	  geometry_msgs::Quaternion q = tf::createQuaternionMsgFromYaw(angles::normalize_angle(yaw - M_PI));
	  
	  goal.target_pose.header.frame_id = "map";
	  goal.target_pose.header.stamp = ros::Time::now();
	  
	  goal.target_pose.pose.position.x = pose.position.x + 1.2*cos(yaw);
	  goal.target_pose.pose.position.y = pose.position.y + 1.2*sin(yaw);
	  
	  goal.target_pose.pose.orientation = q;
	  
	}else if(action_.compare("hand over") == 0){
	  double yaw = tf::getYaw(pose.orientation);
	  geometry_msgs::Quaternion q = tf::createQuaternionMsgFromYaw(angles::normalize_angle(yaw - M_PI));
	  
	  goal.target_pose.header.frame_id = "map";
	  goal.target_pose.header.stamp = ros::Time::now();
	  
	  goal.target_pose.pose.position.x = pose.position.x + 0.95*cos(yaw);
	  goal.target_pose.pose.position.y = pose.position.y + 0.95*sin(yaw);
	  
	  goal.target_pose.pose.orientation = q;
	  
	}else{
	  ROS_ERROR("Unkonown action!");
	  return 2;
	}
	
      }else{
	ROS_ERROR("Failed to call service prox_goal");
	return 3;
      }
    }else{
      ROS_ERROR("Failed to call service get_person");
      return 1;
    }
  }
      
  ROS_INFO("Sending goal");
  ac.sendGoal(goal);

  ac.waitForResult();

  if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    ROS_INFO("Hooray, the base moved");
  else
    ROS_INFO("The base failed to move for some reason");


  return 0;
}