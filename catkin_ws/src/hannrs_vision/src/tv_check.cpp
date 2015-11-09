//ROS
#include <ros/ros.h>
#include <std_msgs/Bool.h> 

int main(int argc, char* argv[]){
  
  ros::init(argc, argv, "tvchecker");
  
  ros::NodeHandle n;
  ros::Publisher tv_pub = n.advertise<std_msgs::Bool>("/hannrs/TV", 1);
  
  std_msgs::Bool msg;
  ros::Rate loop(5);
  while(ros::ok()){
    int tv_status = system("ping -c 1 10.0.29.101");
    if(tv_status == 0){
      msg.data = true;
    }else{
      msg.data = false;
    }
    tv_pub.publish(msg);
    ros::spinOnce();
    loop.sleep();
  }
  
}