cmake_minimum_required(VERSION 2.8.3)
project(hannrs_send_goals)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  actionlib
  angles
  geometry_msgs
  hannrs_msgs
  move_base_msgs
  roscpp
  tf
)

catkin_package(
 INCLUDE_DIRS include
 LIBRARIES hannrs_send_goals
 CATKIN_DEPENDS actionlib angles geometry_msgs hannrs_msgs move_base_msgs roscpp tf
 DEPENDS system_lib
)

###########
## Build ##
###########
include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(${PROJECT_NAME} src/send_goal.cpp)
add_dependencies(${PROJECT_NAME} hannrs_msgs_generate_messages_cpp)
target_link_libraries(${PROJECT_NAME} ${catkin_LIBRARIES})

add_executable(simple_goal src/simple_goal.cpp)
add_dependencies(simple_goal hannrs_msgs_generate_messages_cpp)
target_link_libraries(simple_goal ${catkin_LIBRARIES})
