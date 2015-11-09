#include <ros/ros.h>
#include <hannrs_msgs/People.h>
#include <gazebo_msgs/ModelStates.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/Twist.h>
#include <hannrs_msgs/Get_Person.h>

//people names
const std::string people[] = {"Ze","Manel","Quim"};

std::vector<std::string> people_names(people, people + sizeof(people)/sizeof(std::string));
std::vector<geometry_msgs::Pose> poses;
std::vector<geometry_msgs::Twist> velocities;
std::vector<std::string> names;
hannrs_msgs::People people_list;

void gazeboCallBack(const gazebo_msgs::ModelStates::ConstPtr& msg){
  std::vector<int> inds;
  
  for(int i = 0; i < (int)people_names.size(); i++){
    for(int j = 0; j < (int)msg->name.size(); j++){
      if(msg->name[j].compare(people_names[i]) == 0){
	inds.push_back(j);
      }
    }
  }
  
  names.clear();
  poses.clear();
  velocities.clear();
  
  for(int i = 0; i < (int)inds.size(); i++){
    names.push_back(msg->name[inds[i]]);
    poses.push_back(msg->pose[inds[i]]);
    velocities.push_back(msg->twist[inds[i]]);
  }
}

hannrs_msgs::People fill_msg(){
  hannrs_msgs::People msg;
  hannrs_msgs::Person person;
  std::vector<hannrs_msgs::Person> stuffing;
  std_msgs::Header head;
  
  for(int i = 0; i < (int)names.size(); i++){
    person.name = names[i];
    person.pose = poses[i];
    person.velocity = velocities[i];
    double vel_module = sqrt(pow(velocities[i].linear.x,2) + pow(velocities[i].linear.y,2));
    
    if(names[i].compare("Quim")==0){
      person.status = "sitting";
    }else if(vel_module < 0.2){
      person.status = "standing";
    }else{
      person.status = "walking";
    }
    
    stuffing.push_back(person);
  }
  
  head.stamp = ros::Time::now();
  head.frame_id = "/map";
  
  msg.header = head;
  msg.people = stuffing;
  
  people_list = msg;
  
  return msg;
  
}

bool get_person(hannrs_msgs::Get_Person::Request& req, hannrs_msgs::Get_Person::Response& res){
  for(unsigned int i = 0; i < people_list.people.size(); i++){
    if(req.name.data.compare(people_list.people[i].name) == 0){
      res.pose = people_list.people[i].pose;
      res.status.data = people_list.people[i].status;
      return true;
    }
  }
  return false;
}

int main(int argc, char* argv[]){
  
  ros::init(argc, argv, "people_tracker");
  ros::NodeHandle n; 
  ros::Subscriber sub = n.subscribe("gazebo/model_states", 1, &gazeboCallBack);
  ros::Publisher people_pub = n.advertise<hannrs_msgs::People>("hannrs/people", 1);
  ros::ServiceServer service = n.advertiseService("/get_person", get_person);
  hannrs_msgs::People pub_msg;
  
  ros::Rate loop_rate(5);
  
  while(ros::ok()){
    pub_msg = fill_msg();
    people_pub.publish(pub_msg);
    ros::spinOnce();
    loop_rate.sleep();
  }
  
  return 0; 
}