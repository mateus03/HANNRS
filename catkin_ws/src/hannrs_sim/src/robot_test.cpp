#include <ros/ros.h>
#include <hannrs_msgs/People.h>
#include <hannrs_msgs/InteractionSets.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <nav_msgs/OccupancyGrid.h>
#include <sstream>
#include <geometry_msgs/Quaternion.h>
#include <tf/transform_listener.h>
#include <angles/angles.h>
#include <hannrs_msgs/Get_Person.h>
#include <std_msgs/Bool.h>

std::vector<hannrs_msgs::Person> people_vec;
nav_msgs::OccupancyGrid occup_grid;
std::string default_name("Ze");
unsigned int count = 0;
bool map_received = false;
hannrs_msgs::People msg;
std::vector<std::string> people_who_move;
std::vector<std::pair<double,double> > goals;
int last_flags;

std::vector<hannrs_msgs::InteractionSet> it_sets;
bool tv;

void tvCallback(const std_msgs::Bool::ConstPtr& msg){
  tv = msg->data;
}

void mapToWorld(unsigned int mx, unsigned int my, double& wx, double& wy){
  wx = occup_grid.info.origin.position.x + (mx + 0.5) * occup_grid.info.resolution;
  wy = occup_grid.info.origin.position.y + (my + 0.5) * occup_grid.info.resolution;
}

static void onMouse( int event, int x, int y, int flags, void* ){
  if(event == cv::EVENT_LBUTTONDOWN && map_received){
    
    hannrs_msgs::Person person;
    double pos_x, pos_y;
    mapToWorld(x, occup_grid.info.height - y, pos_x, pos_y);
    
    for(unsigned int i = 0; i < people_vec.size(); i++){
      double dist = sqrt(pow(people_vec[i].pose.position.x - pos_x, 2) + pow(people_vec[i].pose.position.y - pos_y, 2));
      if(dist < 0.5){
	people_who_move.push_back(people_vec[i].name);
	ROS_INFO("%s is going to move", people_vec[i].name.c_str());
	return;
      }
    }
    
    std::string name_;
    std::stringstream sstm;
    sstm << default_name << count;
    name_ = sstm.str();      
    person.name = name_;
    
    double yaw = 2.1611;
    geometry_msgs::Quaternion q = tf::createQuaternionMsgFromYaw(angles::normalize_angle(yaw));
    
    person.pose.position.x = pos_x;
    person.pose.position.y = pos_y;
    person.pose.orientation = q;
    
    count++;
    
    person.status = std::string("standing");
    ROS_INFO("Added %s, he is %s at %f %f", person.name.c_str(), person.status.c_str(), pos_x, pos_y);
    people_vec.push_back(person);
    
  }else if(event == cv::EVENT_RBUTTONDOWN && map_received){

    hannrs_msgs::Person person;
    double pos_x, pos_y;
    mapToWorld(x, occup_grid.info.height - y, pos_x, pos_y);
    
    std::string name_;
    std::stringstream sstm;
    sstm << default_name << count;
    name_ = sstm.str();      
    person.name = name_;
    
    double yaw = -1.154;
    geometry_msgs::Quaternion q = tf::createQuaternionMsgFromYaw(angles::normalize_angle(yaw));
    
    person.pose.position.x = pos_x;
    person.pose.position.y = pos_y;    
    person.pose.orientation = q;
    
    count++;
    
    person.status = std::string("sitting");
    ROS_INFO("Added %s, he is %s at %f %f", person.name.c_str(), person.status.c_str(), pos_x, pos_y);
    people_vec.push_back(person);
    
  }else if(event == cv::EVENT_MBUTTONDOWN && map_received){

    double pos_x, pos_y;
    mapToWorld(x, occup_grid.info.height - y, pos_x, pos_y);
    
    for(unsigned int i = 0; i < people_vec.size(); i++){
      for(unsigned int j = 0; j < people_who_move.size(); j++){
	if(people_who_move[j].compare(people_vec[i].name) == 0){
	  double angle = atan2(pos_y - people_vec[i].pose.position.y, pos_x - people_vec[i].pose.position.x);
	  angle = angles::normalize_angle(angle);
  // 	    ROS_INFO("%s Orientation: %f", people_who_move[j].c_str(), angle);
	  geometry_msgs::Quaternion q = tf::createQuaternionMsgFromYaw(angle);
	  people_vec[i].pose.orientation = q;
	  people_vec[i].velocity.linear.x = 0.55*cos(angle);
	  people_vec[i].velocity.linear.y = 0.55*sin(angle);
	  people_vec[i].status = std::string("walking");
	  goals.push_back(std::pair<double,double>(pos_x, pos_y));
	  ROS_INFO("Velocity: %f %f", people_vec[i].velocity.linear.x, people_vec[i].velocity.linear.y);
	}
      }
    }
    
  }
}

void costmapCallBack(const nav_msgs::OccupancyGrid::ConstPtr& msg_map){
  occup_grid = *msg_map;
  map_received = true;
  ROS_INFO("Map received!");
}

void move_people(){
  if(!people_who_move.empty()){
    if(people_who_move.size() == goals.size()){
      for(unsigned int i = 0; i < people_who_move.size(); i++){
	for(unsigned int j = 0; j < people_vec.size(); j++){
	  if(people_who_move[i].compare(people_vec[j].name) == 0){
	    double dist = sqrt(pow(people_vec[j].pose.position.x - goals[i].first, 2) + pow(people_vec[j].pose.position.y - goals[i].second, 2));
	    if(dist < 0.2){
	      people_vec[j].status = std::string("standing");
	      people_vec[j].velocity.linear.x = 0.0;
	      people_vec[j].velocity.linear.y = 0.0;
	      people_who_move.erase(people_who_move.begin() + i);
	      goals.erase(goals.begin() + i);
	      ROS_INFO("People who move: %d Goals: %d", (int)people_who_move.size(), (int)goals.size());
	    }else{
	      people_vec[j].pose.position.x += people_vec[j].velocity.linear.x * 0.1; 
	      people_vec[j].pose.position.y += people_vec[j].velocity.linear.y * 0.1;
	    }
	  }
	}
      }
    }
  }
}


bool get_person(hannrs_msgs::Get_Person::Request& req, hannrs_msgs::Get_Person::Response& res){
  for(unsigned int i = 0; i < msg.people.size(); i++){
    if(req.name.data.compare(msg.people[i].name) == 0){
      res.pose = msg.people[i].pose;
      res.status.data = msg.people[i].status;
      return true;
    }
  }
  return false;
}

int main(int argc, char* argv[]){
  ros::init(argc, argv, "test_hanp");
  ros::NodeHandle nh;
  ros::Publisher people_pub = nh.advertise<hannrs_msgs::People>("hannrs/people", 1);
  ros::Subscriber tv_sub = nh.subscribe("/hannrs/TV", 1, &tvCallback);
  ros::ServiceServer service = nh.advertiseService("/get_person", get_person);
  ros::Publisher itset_pub = nh.advertise<hannrs_msgs::InteractionSets>("hannrs/interaction_sets", 1); 
  
  ros::NodeHandle private_nh("~");
  std::string map_("/home/mateus/hannrs_ws/src/hannrs_maps/maps/hannrs_LRM-nav.pgm");
  private_nh.param("map", map_, map_);
  
  ros::Subscriber costmap_sub = nh.subscribe("/mbot05/move_base/global_costmap/costmap", 1, &costmapCallBack);
  
  cv::namedWindow("map", cv::WINDOW_NORMAL);
  cv::Mat img_map = cv::imread(map_, CV_LOAD_IMAGE_UNCHANGED);
  
  cv::setMouseCallback( "map", onMouse, 0 );
  
  ros::Rate loop_rate(10);
  ROS_INFO("Entering the Loop!");
  while(ros::ok()){
    cv::imshow("map", img_map);
    cv::waitKey(3);
    msg.header.stamp = ros::Time::now();
    msg.header.frame_id = "/map";
    move_people();
    msg.people = people_vec;
    people_pub.publish(msg);
    
    it_sets.clear();
    if(!people_vec.empty()){
      if(people_vec.back().status.compare("sitting") == 0 && tv ){
	hannrs_msgs::InteractionSet it_set;
	it_set.importance.data = 1.0;
	it_set.center.x = (people_vec.back().pose.position.x + 6.768)/2;
	it_set.center.y = (people_vec.back().pose.position.y + -1.230)/2;
	it_set.radius.data = sqrt((people_vec.back().pose.position.x - 6.768)*(people_vec.back().pose.position.x - 6.768) + 
	(people_vec.back().pose.position.y - 1.230)*(people_vec.back().pose.position.y - 1.230)/2);
	it_sets.push_back(it_set);
      }      
    }
    hannrs_msgs::InteractionSets it_msg;
    it_msg.header.stamp = ros::Time::now();
    it_msg.header.frame_id = "/map";
    it_msg.interaction_sets = it_sets;
    itset_pub.publish(it_msg);
    
    ros::spinOnce();
    loop_rate.sleep();
  }
}
