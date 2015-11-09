#include <termios.h>
#include <signal.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>

#define KEYCODE_A 0x61
#define KEYCODE_D 0x64
#define KEYCODE_S 0x73
#define KEYCODE_W 0x77
#define KEYCODE_Q 0x71
#define KEYCODE_E 0x65
#define KEYCODE_R 0x72
#define KEYCODE_F 0x66

#define KEYCODE_A_CAP 0x41
#define KEYCODE_D_CAP 0x44
#define KEYCODE_S_CAP 0x53
#define KEYCODE_W_CAP 0x57
#define KEYCODE_Q_CAP 0x51
#define KEYCODE_E_CAP 0x45
#define KEYCODE_R_CAP 0x52
#define KEYCODE_F_CAP 0x46

#define KEYCODE_1 0x31
#define KEYCODE_2 0x32
#define KEYCODE_3 0x33

class HannrsKbop
{
  private:
    bool ROBOT;
    bool ZE;
    bool MANEL;
    geometry_msgs::Twist cmd;
    geometry_msgs::Twist cmd_null;
    double walk_vel, run_vel, yaw_rate, yaw_rate_run;
    ros::NodeHandle nh;
    ros::Publisher robot_cmd, Ze_cmd, Manel_cmd;

  public:
  void init(){ 
    ROBOT = true;
    ZE = false;
    MANEL = false;
    
    robot_cmd = nh.advertise<geometry_msgs::Twist>("/cmd_vel",1);
    Ze_cmd = nh.advertise<geometry_msgs::Twist>("/cmd_vel_Ze",1);
    Manel_cmd = nh.advertise<geometry_msgs::Twist>("/cmd_vel_Manel",1);
    
    cmd.linear.x = cmd.linear.y = cmd.linear.z = cmd.angular.x = cmd.angular.y = cmd.angular.z =  0;
    cmd_null.linear.x = cmd_null.linear.y = cmd_null.linear.z = cmd_null.angular.x = cmd_null.angular.y = cmd_null.angular.z =  0;
    
    ros::NodeHandle private_nh("~");
    
    private_nh.param("walk_vel", walk_vel, 0.55);
    private_nh.param("run_vel", run_vel, 1.0);
    private_nh.param("yaw_rate", yaw_rate, 0.5);
    private_nh.param("yaw_run_rate", yaw_rate_run, 1.0);

  }
  
  ~HannrsKbop(){}
  void keyboardLoop();

};

int kfd = 0;
struct termios cooked, raw;

void quit(int sig){
  tcsetattr(kfd, TCSANOW, &cooked);
  exit(0);
}

int main(int argc, char* argv[]){
  ros::init(argc, argv, "hanp_keyboardop");

  HannrsKbop op;
  op.init();

  signal(SIGINT,quit);

  op.keyboardLoop();

  return(0);
}

void HannrsKbop::keyboardLoop()
{
  char c;
  bool dirty=false;

  // get the console in raw mode
  tcgetattr(kfd, &cooked);
  memcpy(&raw, &cooked, sizeof(struct termios));
  raw.c_lflag &=~ (ICANON | ECHO);
  // Setting a new line, then end of file
  raw.c_cc[VEOL] = 1;
  raw.c_cc[VEOF] = 2;
  tcsetattr(kfd, TCSANOW, &raw);

  puts("\n--- QR-SIM 2012 ---");
  puts("Reading from keyboard");
  puts("---------------------------");
  puts("Use 'WASD' to translate");
  puts("Use 'QE' to yaw and 'RF' to scale");
  puts("Use any other key to cancel movement");
  puts("Press 'CAPS' to run");


  for(;;){
    
    // get the next event from the keyboard
    if(read(kfd, &c, 1) < 0){
      perror("read():");
      exit(-1);
    }

    cmd.linear.x = cmd.linear.y = cmd.linear.z = cmd.angular.z = 0;

    switch(c){
      // Walking
    case KEYCODE_W:
      cmd.linear.x = walk_vel;
      dirty = true;
      break;
    case KEYCODE_S:
      cmd.linear.x = - walk_vel;
      dirty = true;
      break;
    case KEYCODE_A:
      cmd.linear.y = walk_vel;
      dirty = true;
      break;
    case KEYCODE_D:
      cmd.linear.y = - walk_vel;
      dirty = true;
      break;
    case KEYCODE_Q:
      cmd.angular.z = yaw_rate;
      dirty = true;
      break;
    case KEYCODE_E:
      cmd.angular.z = - yaw_rate;
      dirty = true;
      break;     
    case KEYCODE_R:
      cmd.linear.z = walk_vel;
      dirty = true;
      break;
    case KEYCODE_F:
      cmd.linear.z = - walk_vel;
      dirty = true;
      break;

      // Running 
    case KEYCODE_W_CAP:
      cmd.linear.x = run_vel;
      dirty = true;
      break;
    case KEYCODE_S_CAP:
      cmd.linear.x = - run_vel;
      dirty = true;
      break;
    case KEYCODE_A_CAP:
      cmd.linear.y = run_vel;
      dirty = true;
      break;
    case KEYCODE_D_CAP:
      cmd.linear.y = - run_vel;
      dirty = true;
      break;
    case KEYCODE_Q_CAP:
      cmd.angular.z = yaw_rate;
      dirty = true;
      break;
    case KEYCODE_E_CAP:
      cmd.angular.z = - yaw_rate;
      dirty = true;
      break;     
    case KEYCODE_R_CAP:
      cmd.linear.z = run_vel;
      dirty = true;
      break;
    case KEYCODE_F_CAP:
      cmd.linear.z = - run_vel;
      dirty = true;
      break;
      
    //switch mode
    case KEYCODE_1:
      ROBOT = true;
      ZE = false;
      MANEL = false;
      break;
    case KEYCODE_2:
      ROBOT = false;
      ZE = true;
      MANEL = false;
      break;
    case KEYCODE_3:
      ROBOT = false;
      ZE = false;
      MANEL = true;
      break;
    }
    
    if (dirty){
      if(ROBOT){
	robot_cmd.publish(cmd);
	Ze_cmd.publish(cmd_null);
	Manel_cmd.publish(cmd_null);
      }else if(ZE){
	Ze_cmd.publish(cmd);
	robot_cmd.publish(cmd_null);
	Manel_cmd.publish(cmd_null);
      }else if(MANEL){
	Manel_cmd.publish(cmd);
	robot_cmd.publish(cmd_null);
	Ze_cmd.publish(cmd_null);
      }
    }
  }
}
