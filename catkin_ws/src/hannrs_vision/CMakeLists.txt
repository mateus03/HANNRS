cmake_minimum_required(VERSION 2.8.3)
project(hannrs_vision)

#set(OpenCV_DIR /opt/ros/hydro/share/OpenCV)

find_package(catkin REQUIRED COMPONENTS
  cv_bridge
  dynamic_reconfigure
  hannrs_msgs
  image_transport
  roscpp
  sensor_msgs
  std_msgs
  geometry_msgs
)

generate_dynamic_reconfigure_options(
  cfg/Undistort.cfg
)
catkin_package(
 INCLUDE_DIRS include
 LIBRARIES hannrs_vision
 CATKIN_DEPENDS cv_bridge dynamic_reconfigure hannrs_msgs image_transport roscpp sensor_msgs std_msgs geometry_msgs
 DEPENDS system_lib
)

###########
## Build ##
###########

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

#set(ALL_LIBS ${catkin_LIBRARIES}  ${OpenCV_LIBS} ${image_transport_LIBS} ${cv_bridge_LIBS})

add_executable(detect_person src/detect_people.cpp)
add_executable(omnidetect src/omnidetect.cpp)
add_executable(test_svm src/test_svm.cpp)
add_executable(vision_node src/vision_node.cpp src/people_detector.cpp src/undistort.cpp)
add_executable(track src/track.cpp)
add_executable(track2 src/track2.cpp)
add_executable(tvchecker src/tv_check.cpp)

add_dependencies(detect_person hannrs_msgs_generate_messages_cpp)
add_dependencies(omnidetect hannrs_msgs_generate_messages_cpp)
add_dependencies(test_svm hannrs_msgs_generate_messages_cpp)
add_dependencies(vision_node hannrs_msgs_generate_messages_cpp)
add_dependencies(track hannrs_msgs_generate_messages_cpp)
add_dependencies(track2 hannrs_msgs_generate_messages_cpp)
add_dependencies(tvchecker hannrs_msgs_generate_messages_cpp)

# target_link_libraries(detect_person
#   ${ALL_LIBS}
# )

# target_link_libraries(omnidetect
#   ${ALL_LIBS}
# )

# target_link_libraries(test_svm
#   ${ALL_LIBS}
# )

# target_link_libraries(vision_node
#   ${ALL_LIBS}
# )

target_link_libraries(track
  ${catkin_LIBRARIES}
)

# target_link_libraries(track2
#   ${ALL_LIBS}
# )

target_link_libraries(tvchecker
  ${catkin_LIBRARIES} ${image_transport_LIBS} ${cv_bridge_LIBS}
)

target_link_libraries(detect_person
  ${catkin_LIBRARIES} ${image_transport_LIBS} ${cv_bridge_LIBS}
)

target_link_libraries(omnidetect
  ${catkin_LIBRARIES}  ${image_transport_LIBS} ${cv_bridge_LIBS}
)

target_link_libraries(test_svm
  ${catkin_LIBRARIES} ${image_transport_LIBS} ${cv_bridge_LIBS}
)

target_link_libraries(vision_node
  ${catkin_LIBRARIES} ${image_transport_LIBS} ${cv_bridge_LIBS}
)

target_link_libraries(track2
  ${catkin_LIBRARIES} ${image_transport_LIBS} ${cv_bridge_LIBS}
)
