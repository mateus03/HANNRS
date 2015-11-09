cmake_minimum_required(VERSION 2.8.3)
project(hannrs_sim)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  angles
  geometry_msgs
  hannrs_msgs
  nav_msgs
  roscpp
  tf
  cv_bridge
)

#find_package( OpenCV REQUIRED )

catkin_package(
 INCLUDE_DIRS include
 LIBRARIES hannrs_sim
 CATKIN_DEPENDS angles geometry_msgs hannrs_msgs nav_msgs roscpp tf cv_bridge
 DEPENDS system_lib
)

###########
## Build ##
###########
include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)

add_executable(test_hanp src/robot_test.cpp)
add_executable(exp1 src/exp1.cpp)
add_executable(vel_module src/vel_module.cpp)

add_dependencies(test_hanp hannrs_msgs_generate_messages_cpp)
add_dependencies(exp1 hannrs_msgs_generate_messages_cpp)
add_dependencies(vel_module hannrs_msgs_generate_messages_cpp)

target_link_libraries(test_hanp
  ${catkin_LIBRARIES}
  ${OpenCV_LIBRARIES} 
)

target_link_libraries(exp1
  ${catkin_LIBRARIES}
)

target_link_libraries(vel_module
  ${catkin_LIBRARIES}
)

