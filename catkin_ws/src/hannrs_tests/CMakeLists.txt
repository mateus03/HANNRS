cmake_minimum_required(VERSION 2.8.3)
project(hannrs_tests)

# set(OpenCV_DIR /opt/ros/hydro/share/OpenCV)

# find_package(catkin REQUIRED COMPONENTS
#   geometry_msgs
#   nav_msgs
#   roscpp
#   std_msgs
#   OpenCV
# )

find_package(catkin REQUIRED COMPONENTS
  geometry_msgs
  nav_msgs
  roscpp
  std_msgs
  cv_bridge
  hannrs_msgs
)

catkin_package(
 INCLUDE_DIRS include
 LIBRARIES hannrs_tests
 CATKIN_DEPENDS geometry_msgs nav_msgs roscpp std_msgs cv_bridge hannrs_msgs
 DEPENDS system_lib
)

###########
## Build ##
###########
include_directories(
  ${catkin_INCLUDE_DIRS}
)

#set(ALL_LIBS ${catkin_LIBRARIES}  ${OpenCV_LIBS})

## Declare a cpp executable
add_executable(paths src/paths.cpp)
add_executable(paths2 src/paths2.cpp)
add_executable(vel_test src/vel_test.cpp)
add_executable(dist_test src/dist_test.cpp)

add_dependencies(paths hannrs_msgs_generate_messages_cpp)
add_dependencies(paths2 hannrs_msgs_generate_messages_cpp)
add_dependencies(vel_test hannrs_msgs_generate_messages_cpp)
add_dependencies(dist_test hannrs_msgs_generate_messages_cpp)

# target_link_libraries(paths
#   ${ALL_LIBS}
# )

# target_link_libraries(paths2
#   ${ALL_LIBS}
# )

# target_link_libraries(vel_test
#   ${ALL_LIBS}
# )

# target_link_libraries(dist_test
#   ${ALL_LIBS}
# )

target_link_libraries(paths
  ${catkin_LIBRARIES}
)

target_link_libraries(paths2
  ${catkin_LIBRARIES}
)

target_link_libraries(vel_test
  ${catkin_LIBRARIES}
)

target_link_libraries(dist_test
  ${catkin_LIBRARIES}
)
