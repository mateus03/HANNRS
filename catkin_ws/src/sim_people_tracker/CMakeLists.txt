cmake_minimum_required(VERSION 2.8.3)
project(sim_people_tracker)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  gazebo_msgs
  hannrs_msgs
)

catkin_package(
 INCLUDE_DIRS include
 LIBRARIES sim_people_tracker
 CATKIN_DEPENDS roscpp std_msgs gazebo_msgs hannrs_msgs
 DEPENDS system_lib
)

###########
## Build ##
###########
include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(sim_people_tracker_node src/people_tracker.cpp)

add_dependencies(sim_people_tracker_node hannrs_msgs_generate_messages_cpp)

target_link_libraries(sim_people_tracker_node
  ${catkin_LIBRARIES}
)
