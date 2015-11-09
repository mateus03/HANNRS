#include <gazebo/common/Plugin.hh>
#include <gazebo/gazebo.hh>
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <gazebo/math/Vector3.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/physics/PhysicsIface.hh>
#include <hannrs_msgs/People.h>
#include <tf/transform_datatypes.h>

namespace gazebo{

  class ZeControl : public ModelPlugin{
    
    public: 
      void Load(physics::ModelPtr _parent, sdf::ElementPtr){
	// Make sure the ROS node for Gazebo has already been initialized                                                                                    
	if (!ros::isInitialized()){
	  ROS_FATAL_STREAM("A ROS node for Gazebo has not been initialized, unable to load plugin. "
	    << "Load the Gazebo system plugin 'libgazebo_ros_api_plugin.so' in the gazebo_ros package)");
	  return;
	}

	// Store the pointer to the model
	this->model = _parent;
	
	// Listen to the update event. This event is broadcast every
	// simulation iteration.
	this->updateConnection = event::Events::ConnectWorldUpdateBegin(
	    boost::bind(&ZeControl::UpdateChild, this));
	
	cmd_sub = nh.subscribe("/cmd_vel_Ze",1, &ZeControl::cmdCallBack, this);
	people_sub = nh.subscribe("hannrs/people", 1, &ZeControl::peopleCallBack, this);
	
	fst_cmd = true;
	cmd.linear.x = cmd.linear.y = cmd.linear.z = cmd.angular.x = cmd.angular.y = cmd.angular.z =  0;
      }
      
      void cmdCallBack(const geometry_msgs::Twist::ConstPtr& msg){
	cmd = *msg;
	fst_cmd = false;
      }
      
      void peopleCallBack(const hannrs_msgs::People::ConstPtr& msg){
	  people_list = *msg;
	  
	  for(unsigned int i = 0; i < people_list.people.size(); i++){
	    hannrs_msgs::Person& person = people_list.people[i];
	    
	    if(person.name.compare("Ze") == 0){
	      pose = person.pose;
	    }
	  }
      }
      
      void UpdateChild(){
	if(!fst_cmd){
	  double theta = tf::getYaw(pose.orientation);
	  double lin_vel_x = cmd.linear.x*cos(theta) - cmd.linear.y*sin(theta);
	  double lin_vel_y = cmd.linear.x*sin(theta) + cmd.linear.y*cos(theta);
	  this->model->SetAngularVel(math::Vector3(cmd.angular.x, cmd.angular.y, cmd.angular.z));
	  this->model->SetLinearVel(math::Vector3(lin_vel_x, lin_vel_y, cmd.linear.z));
	}
      }
      
      private:
	// Pointer to the model
	physics::ModelPtr model;

	// Pointer to the update event connection
	event::ConnectionPtr updateConnection;
	
	ros::NodeHandle nh;
	ros::Subscriber cmd_sub;
	ros::Subscriber people_sub;
	geometry_msgs::Twist cmd;
	hannrs_msgs::People people_list;
	geometry_msgs::Pose pose;
	bool fst_cmd;
    
  };

  // Register this plugin with the simulator
  GZ_REGISTER_MODEL_PLUGIN(ZeControl);
}
