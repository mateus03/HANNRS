cmake_minimum_required(VERSION 2.8.3)
project(sim_interaction_detector)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  gazebo_msgs
  geometry_msgs
  hannrs_msgs
  roscpp
)

catkin_package(
 INCLUDE_DIRS include
 LIBRARIES sim_interaction_detector
 CATKIN_DEPENDS gazebo_msgs geometry_msgs hannrs_msgs roscpp
 DEPENDS system_lib
)

###########
## Build ##
###########
include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(sim_interaction_detector_node src/interaction_detector.cpp)

add_dependencies(sim_interaction_detector_node hannrs_msgs_generate_messages_cpp)

target_link_libraries(sim_interaction_detector_node
  ${catkin_LIBRARIES}
)

#############
## Install ##
#############
