//ROS
#include <ros/ros.h>
#include <std_msgs/Time.h>
#include <std_msgs/Header.h>
#include <hannrs_msgs/People.h>
#include <hannrs_msgs/InteractionSets.h>
#include <tf/transform_listener.h>
#include "hannrs_msgs/VisionDetection.h"
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/TwistStamped.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Float64.h>
#include <hannrs_msgs/Get_Person.h>
//OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>

#include <fstream>

std::vector<hannrs_msgs::InteractionSet> it_sets;
std::vector<geometry_msgs::Pose> robot;
std::vector<hannrs_msgs::Person> people;
std::vector<std_msgs::Header> headers;
cv::KalmanFilter KF;

// std::ofstream pos_data;

double norm;
bool tv = false;

// void tvCallback(const std_msgs::Bool::ConstPtr& msg){
//   tv = msg->data;
// }

void robotCallback(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr& msg){
//   ROS_INFO("Time: %f", ros::Time::now().toSec());
  robot.push_back(msg->pose.pose);
}

void detectionsCallback(const hannrs_msgs::VisionDetection::ConstPtr& msg){  
  if(people.empty()){
    if(msg->positions.size() == 1){
      KF = cv::KalmanFilter(4,2,0,CV_64F);
      KF.transitionMatrix = *(cv::Mat_<double>(4, 4) << 1,0,1,0, 0,1,0,1, 0,0,1,0, 0,0,0,1);
      // init...
      KF.statePre.at<double>(0) = msg->positions[0].x;
      KF.statePre.at<double>(1) = msg->positions[0].y;
      KF.statePre.at<double>(2) = 0;
      KF.statePre.at<double>(3) = 0;
      KF.measurementMatrix = *(cv::Mat_<double>(2, 4) << 1,0,0,0, 0,1,0,0);
      KF.processNoiseCov = *(cv::Mat_<double>(4,4) << 0.1,0,0,0,  0,0.1,0,0,  0,0,0.25,0,  0,0,0,0.25);
      setIdentity(KF.measurementNoiseCov, cv::Scalar::all(0.1));
      KF.measurementNoiseCov = *(cv::Mat_<double>(2,2) << 0.1,0,  0,0.1);
      KF.errorCovPost = *(cv::Mat_<double>(4,4) << 0.1,0,0,0,  0,0.1,0,0,  0,0,.1,0,  0,0,0,.1);
	      
      hannrs_msgs::Person person;
      person.pose.position = msg->positions[0];
      person.pose.orientation.w = 1.0;
      person.status = "standing"; //person cannot enter the scene sitted
      person.name = "Andre";
      people.push_back(person);

    }
  }else{
    cv::Mat_<double> measurement(2,1); measurement.setTo(cv::Scalar(0));
    double dt = msg->header.stamp.toSec() - headers.back().stamp.toSec();
    
    KF.transitionMatrix = *(cv::Mat_<double>(4, 4) << 1,0,dt,0, 0,1,0,dt, 0,0,1,0, 0,0,0,1);

    double vel_module;
    hannrs_msgs::Person person;
    
    if(msg->positions.size() > 1){
      double dist = 10000;
      unsigned int ind;
      for(unsigned int i = 0; i < msg->positions.size(); i++){
	double d = sqrt((KF.statePost.at<double>(0) - msg->positions[i].x)*(KF.statePost.at<double>(0) - msg->positions[i].x) + 
	(KF.statePost.at<double>(1) - msg->positions[i].y)*(KF.statePost.at<double>(1) - msg->positions[i].y));
	if(d < dist){
	  dist = d;
	  ind = i;
	}
      }
      
      // First predict, to update the internal statePre variable
      cv::Mat prediction = KF.predict();
      
      // Get detection
      measurement(0) = msg->positions[ind].x;
      measurement(1) = msg->positions[ind].y;
      
      // The "correct" phase that is going to use the predicted value and our measurement
      cv::Mat estimated = KF.correct(measurement);
      
      person.pose.position.x = estimated.at<double>(0);
      person.pose.position.y = estimated.at<double>(1);
      if(msg->classifications[ind] == 2.0){
	person.pose.orientation = tf::createQuaternionMsgFromYaw(msg->positions[ind].z);
	person.velocity.linear.x = 0;
	person.velocity.linear.y = 0;
	vel_module = -1;
      }else{
	person.pose.orientation = tf::createQuaternionMsgFromYaw(atan2(estimated.at<double>(3), estimated.at<double>(2)));
	person.velocity.linear.x = estimated.at<double>(2);
	person.velocity.linear.y = estimated.at<double>(3);
	vel_module = sqrt(estimated.at<double>(2)*estimated.at<double>(2) + estimated.at<double>(3)*estimated.at<double>(3));
      }	
    }else if(msg->positions.size() == 1){
      // First predict, to update the internal statePre variable
      cv::Mat prediction = KF.predict();
      
      // Get detection
      measurement(0) = msg->positions[0].x;
      measurement(1) = msg->positions[0].y;
		  
      cv::Point measPt(measurement(0),measurement(1));
      
      // The "correct" phase that is going to use the predicted value and our measurement
      cv::Mat estimated = KF.correct(measurement);
      
      person.pose.position.x = estimated.at<double>(0);
      person.pose.position.y = estimated.at<double>(1);
      if(msg->classifications[0] == 2.0){
	person.pose.orientation = tf::createQuaternionMsgFromYaw(msg->positions[0].z);
	person.velocity.linear.x = 0;
	person.velocity.linear.y = 0;
	vel_module = -1;
      }else{
	person.pose.orientation = tf::createQuaternionMsgFromYaw(atan2(estimated.at<double>(3), estimated.at<double>(2)));
	person.velocity.linear.x = estimated.at<double>(2);
	person.velocity.linear.y = estimated.at<double>(3);
	vel_module = sqrt(estimated.at<double>(2)*estimated.at<double>(2) + estimated.at<double>(3)*estimated.at<double>(3));
      }
    }else{
      // First predict, to update the internal statePre variable
      cv::Mat prediction = KF.predict();
      
      person.pose.position.x = prediction.at<double>(0);
      person.pose.position.y = prediction.at<double>(1);
      if(people.back().status.compare("sitting") == 0){
	person.pose.orientation = people.back().pose.orientation;
	person.velocity.linear.x = 0;
	person.velocity.linear.y = 0;
	vel_module = -1;
      }else{
	person.pose.orientation = tf::createQuaternionMsgFromYaw(atan2(prediction.at<double>(3), prediction.at<double>(2)));
	person.velocity.linear.x = prediction.at<double>(2);
	person.velocity.linear.y = prediction.at<double>(3);
	vel_module = sqrt(prediction.at<double>(2)*prediction.at<double>(2) + prediction.at<double>(3)*prediction.at<double>(3));
      }
      
	KF.statePost = KF.statePre;
    }
    
    if(vel_module == -1){
      person.velocity.linear.x = 0;
      person.velocity.linear.y = 0;
      person.status = "sitting";
    }else if(vel_module < 0.15){
      person.pose.orientation = people.back().pose.orientation;
      person.velocity.linear.x = 0;
      person.velocity.linear.y = 0;
      person.status = "standing";
    }else{
      person.status = "walking";
    }
    
    person.name = "Andre";
    people.push_back(person);

  }
  
  headers.push_back(msg->header);
}

bool get_person(hannrs_msgs::Get_Person::Request& req, hannrs_msgs::Get_Person::Response& res){
  if(people.empty()){
    return false;
  }else{
    if(req.name.data.compare(people.back().name) == 0){
      res.pose = people.back().pose;
      res.status.data = people.back().status;
      return true;
    }
  }
}

int main(int argc, char* argv[]){
  ros::init(argc, argv, "tracker");
  
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("/hannrs/visiondetections", 1, detectionsCallback);
//   ros::Subscriber tv_sub = n.subscribe("/hannrs/TV", 1, tvCallback);
//   ros::Subscriber robot_sub = n.subscribe("/mbot05/amcl_pose", 1, robotCallback);
  ros::Publisher people_pub = n.advertise<hannrs_msgs::People>("/hannrs/people", 1);
  ros::Publisher itset_pub = n.advertise<hannrs_msgs::InteractionSets>("/hannrs/interaction_sets", 1);
  ros::Publisher pose_pub = n.advertise<geometry_msgs::PoseStamped>("/people_pose", 1);
  ros::Publisher vel_pub = n.advertise<geometry_msgs::TwistStamped>("/people_vel", 1);
  ros::ServiceServer service = n.advertiseService("/get_person", get_person);
//   ros::Publisher norm_pub = n.advertise<std_msgs::Float64>("/norm",1);
  
//   pos_data.open("/home/mateus/track_data.txt");
  
  ros::Rate loop(5);
  while(ros::ok()){
//         ros::Time t = ros::Time::now();
    
    if(!people.empty()){
      hannrs_msgs::People pub_msg;
      pub_msg.header.stamp = ros::Time::now();
      pub_msg.header.frame_id = "/map";
      pub_msg.people.push_back(people.back());
      people_pub.publish(pub_msg);
//       pos_data << people.back().pose.position.x << " " << people.back().pose.position.y << std::endl;
      geometry_msgs::PoseStamped pose_msg;
      pose_msg.header.stamp = ros::Time::now();
      pose_msg.header.frame_id = "/map";
      pose_msg.pose = people.back().pose;
      pose_pub.publish(pose_msg);
      
      geometry_msgs::TwistStamped vel_msg;
      vel_msg.header.stamp = ros::Time::now();
      vel_msg.header.frame_id = "/map";
      vel_msg.twist = people.back().velocity;
      vel_pub.publish(vel_msg);
      
//       it_sets.clear();
//       if(people.back().status.compare("sitting") == 0 && tv ){
// 	hannrs_msgs::InteractionSet it_set;
// 	it_set.importance.data = 1.0;
// 	it_set.center.x = (people.back().pose.position.x + 6.768)/2;
// 	it_set.center.y = (people.back().pose.position.y + -1.230)/2;
// 	it_set.radius.data = sqrt((people.back().pose.position.x - 6.768)*(people.back().pose.position.x - 6.768) + 
// 	(people.back().pose.position.y - 1.230)*(people.back().pose.position.y - 1.230)/2);
// 	it_sets.push_back(it_set);
//       }      
//       hannrs_msgs::InteractionSets it_msg;
//       it_msg.header.stamp = ros::Time::now();
//       it_msg.header.frame_id = "/map";
//       it_msg.interaction_sets = it_sets;
//       itset_pub.publish(it_msg);
//       
//       norm = cv::norm(KF.errorCovPost, cv::NORM_L2);
//       std_msgs::Float64 norm_msg;
//       norm_msg.data = norm;
//       norm_pub.publish(norm_msg);

//       norm = cv::norm(KF.errorCovPost, cv::NORM_L2);
//       std::cout << ros::Time::now().toSec() - headers.front().stamp.toSec() << " " << norm << std::endl;
//       std::cout << KF.errorCovPost << std::endl;
    }
    
    
    
    ros::spinOnce();
    loop.sleep();
    
//     double elapsed = ros::Time::now().toSec() - t.toSec();
//     ROS_INFO("Elapsed: %f", elapsed);
  }
  
//   pos_data.close();
  
  return 0;
}
