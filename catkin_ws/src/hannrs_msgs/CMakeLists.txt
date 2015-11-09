cmake_minimum_required(VERSION 2.8.3)
project(hannrs_msgs)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  message_generation
  geometry_msgs
  std_msgs
)
add_message_files(
  FILES
  Person.msg
  People.msg
  InteractionSet.msg
  InteractionSets.msg
  Goal.msg
  VisionDetection.msg
)

## Generate services in the 'srv' folder
add_service_files(
  FILES
  Prox_Goal.srv
  Get_Person.srv
  
)

## Generate added messages and services with any dependencies listed here
generate_messages(
  DEPENDENCIES
  geometry_msgs  std_msgs
)

catkin_package(
 CATKIN_DEPENDS message_runtime geometry_msgs std_msgs
)

###########
## Build ##
###########
include_directories(
  ${catkin_INCLUDE_DIRS}
)
