cmake_minimum_required(VERSION 2.8.3)
project(hannrs_costmap_layers)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  costmap_2d
  dynamic_reconfigure
  hannrs_msgs
  pluginlib
  roscpp
)

# generate_dynamic_reconfigure_options(
generate_dynamic_reconfigure_options(
  cfg/ProxemicsLayer.cfg
  cfg/VisibilityLayer.cfg
  cfg/Pass_On_LeftLayer.cfg
  cfg/InteractionLayer.cfg
)

catkin_package(
 INCLUDE_DIRS include
 LIBRARIES hannrs_costmap_layers
 CATKIN_DEPENDS costmap_2d dynamic_reconfigure hannrs_msgs pluginlib roscpp
)

###########
## Build ##
###########
include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

## Declare a cpp library
add_library(${PROJECT_NAME}
  src/proxemics_layer.cpp
  src/visibility_layer.cpp
  src/pass_on_left_layer.cpp
  src/interaction_layer.cpp
)

add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}_gencfg hannrs_msgs_generate_messages_cpp)

## Specify libraries to link a library or executable target against
target_link_libraries(${PROJECT_NAME}
  ${catkin_LIBRARIES}
)

#############
## Install ##
#############
install(FILES hannrs_costmap_layers_plugin.xml
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})
