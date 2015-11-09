# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "hannrs_msgs: 6 messages, 2 services")

set(MSG_I_FLAGS "-Ihannrs_msgs:/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/msg;-Igeometry_msgs:/opt/ros/indigo/share/geometry_msgs/cmake/../msg;-Istd_msgs:/opt/ros/indigo/share/std_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(genlisp REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(hannrs_msgs_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/srv/Prox_Goal.srv" NAME_WE)
add_custom_target(_hannrs_msgs_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "hannrs_msgs" "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/srv/Prox_Goal.srv" "std_msgs/Float64:std_msgs/String"
)

get_filename_component(_filename "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/msg/Goal.msg" NAME_WE)
add_custom_target(_hannrs_msgs_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "hannrs_msgs" "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/msg/Goal.msg" "geometry_msgs/Point:geometry_msgs/Quaternion:std_msgs/Header:geometry_msgs/Pose"
)

get_filename_component(_filename "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/msg/People.msg" NAME_WE)
add_custom_target(_hannrs_msgs_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "hannrs_msgs" "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/msg/People.msg" "geometry_msgs/Point:hannrs_msgs/Person:geometry_msgs/Vector3:std_msgs/Header:geometry_msgs/Quaternion:geometry_msgs/Pose:geometry_msgs/Twist"
)

get_filename_component(_filename "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/srv/Get_Person.srv" NAME_WE)
add_custom_target(_hannrs_msgs_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "hannrs_msgs" "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/srv/Get_Person.srv" "geometry_msgs/Point:geometry_msgs/Quaternion:std_msgs/String:geometry_msgs/Pose"
)

get_filename_component(_filename "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/msg/InteractionSet.msg" NAME_WE)
add_custom_target(_hannrs_msgs_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "hannrs_msgs" "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/msg/InteractionSet.msg" "std_msgs/Float64:geometry_msgs/Point"
)

get_filename_component(_filename "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/msg/VisionDetection.msg" NAME_WE)
add_custom_target(_hannrs_msgs_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "hannrs_msgs" "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/msg/VisionDetection.msg" "std_msgs/Header:geometry_msgs/Point"
)

get_filename_component(_filename "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/msg/Person.msg" NAME_WE)
add_custom_target(_hannrs_msgs_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "hannrs_msgs" "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/msg/Person.msg" "geometry_msgs/Quaternion:geometry_msgs/Vector3:geometry_msgs/Twist:geometry_msgs/Point:geometry_msgs/Pose"
)

get_filename_component(_filename "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/msg/InteractionSets.msg" NAME_WE)
add_custom_target(_hannrs_msgs_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "hannrs_msgs" "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/msg/InteractionSets.msg" "std_msgs/Float64:hannrs_msgs/InteractionSet:std_msgs/Header:geometry_msgs/Point"
)

#
#  langs = gencpp;genlisp;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(hannrs_msgs
  "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/msg/Goal.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Point.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/indigo/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Pose.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/hannrs_msgs
)
_generate_msg_cpp(hannrs_msgs
  "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/msg/People.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Point.msg;/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/msg/Person.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Vector3.msg;/opt/ros/indigo/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Twist.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/hannrs_msgs
)
_generate_msg_cpp(hannrs_msgs
  "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/msg/InteractionSet.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/indigo/share/std_msgs/cmake/../msg/Float64.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/hannrs_msgs
)
_generate_msg_cpp(hannrs_msgs
  "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/msg/VisionDetection.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/indigo/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/hannrs_msgs
)
_generate_msg_cpp(hannrs_msgs
  "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/msg/Person.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Vector3.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Twist.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Point.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Pose.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/hannrs_msgs
)
_generate_msg_cpp(hannrs_msgs
  "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/msg/InteractionSets.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/indigo/share/std_msgs/cmake/../msg/Float64.msg;/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/msg/InteractionSet.msg;/opt/ros/indigo/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/hannrs_msgs
)

### Generating Services
_generate_srv_cpp(hannrs_msgs
  "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/srv/Get_Person.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Point.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/indigo/share/std_msgs/cmake/../msg/String.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Pose.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/hannrs_msgs
)
_generate_srv_cpp(hannrs_msgs
  "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/srv/Prox_Goal.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/indigo/share/std_msgs/cmake/../msg/Float64.msg;/opt/ros/indigo/share/std_msgs/cmake/../msg/String.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/hannrs_msgs
)

### Generating Module File
_generate_module_cpp(hannrs_msgs
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/hannrs_msgs
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(hannrs_msgs_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(hannrs_msgs_generate_messages hannrs_msgs_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/srv/Prox_Goal.srv" NAME_WE)
add_dependencies(hannrs_msgs_generate_messages_cpp _hannrs_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/msg/Goal.msg" NAME_WE)
add_dependencies(hannrs_msgs_generate_messages_cpp _hannrs_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/msg/People.msg" NAME_WE)
add_dependencies(hannrs_msgs_generate_messages_cpp _hannrs_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/srv/Get_Person.srv" NAME_WE)
add_dependencies(hannrs_msgs_generate_messages_cpp _hannrs_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/msg/InteractionSet.msg" NAME_WE)
add_dependencies(hannrs_msgs_generate_messages_cpp _hannrs_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/msg/VisionDetection.msg" NAME_WE)
add_dependencies(hannrs_msgs_generate_messages_cpp _hannrs_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/msg/Person.msg" NAME_WE)
add_dependencies(hannrs_msgs_generate_messages_cpp _hannrs_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/msg/InteractionSets.msg" NAME_WE)
add_dependencies(hannrs_msgs_generate_messages_cpp _hannrs_msgs_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(hannrs_msgs_gencpp)
add_dependencies(hannrs_msgs_gencpp hannrs_msgs_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS hannrs_msgs_generate_messages_cpp)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(hannrs_msgs
  "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/msg/Goal.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Point.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/indigo/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Pose.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/hannrs_msgs
)
_generate_msg_lisp(hannrs_msgs
  "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/msg/People.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Point.msg;/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/msg/Person.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Vector3.msg;/opt/ros/indigo/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Twist.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/hannrs_msgs
)
_generate_msg_lisp(hannrs_msgs
  "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/msg/InteractionSet.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/indigo/share/std_msgs/cmake/../msg/Float64.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/hannrs_msgs
)
_generate_msg_lisp(hannrs_msgs
  "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/msg/VisionDetection.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/indigo/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/hannrs_msgs
)
_generate_msg_lisp(hannrs_msgs
  "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/msg/Person.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Vector3.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Twist.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Point.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Pose.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/hannrs_msgs
)
_generate_msg_lisp(hannrs_msgs
  "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/msg/InteractionSets.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/indigo/share/std_msgs/cmake/../msg/Float64.msg;/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/msg/InteractionSet.msg;/opt/ros/indigo/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/hannrs_msgs
)

### Generating Services
_generate_srv_lisp(hannrs_msgs
  "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/srv/Get_Person.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Point.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/indigo/share/std_msgs/cmake/../msg/String.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Pose.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/hannrs_msgs
)
_generate_srv_lisp(hannrs_msgs
  "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/srv/Prox_Goal.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/indigo/share/std_msgs/cmake/../msg/Float64.msg;/opt/ros/indigo/share/std_msgs/cmake/../msg/String.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/hannrs_msgs
)

### Generating Module File
_generate_module_lisp(hannrs_msgs
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/hannrs_msgs
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(hannrs_msgs_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(hannrs_msgs_generate_messages hannrs_msgs_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/srv/Prox_Goal.srv" NAME_WE)
add_dependencies(hannrs_msgs_generate_messages_lisp _hannrs_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/msg/Goal.msg" NAME_WE)
add_dependencies(hannrs_msgs_generate_messages_lisp _hannrs_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/msg/People.msg" NAME_WE)
add_dependencies(hannrs_msgs_generate_messages_lisp _hannrs_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/srv/Get_Person.srv" NAME_WE)
add_dependencies(hannrs_msgs_generate_messages_lisp _hannrs_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/msg/InteractionSet.msg" NAME_WE)
add_dependencies(hannrs_msgs_generate_messages_lisp _hannrs_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/msg/VisionDetection.msg" NAME_WE)
add_dependencies(hannrs_msgs_generate_messages_lisp _hannrs_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/msg/Person.msg" NAME_WE)
add_dependencies(hannrs_msgs_generate_messages_lisp _hannrs_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/msg/InteractionSets.msg" NAME_WE)
add_dependencies(hannrs_msgs_generate_messages_lisp _hannrs_msgs_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(hannrs_msgs_genlisp)
add_dependencies(hannrs_msgs_genlisp hannrs_msgs_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS hannrs_msgs_generate_messages_lisp)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(hannrs_msgs
  "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/msg/Goal.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Point.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/indigo/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Pose.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/hannrs_msgs
)
_generate_msg_py(hannrs_msgs
  "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/msg/People.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Point.msg;/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/msg/Person.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Vector3.msg;/opt/ros/indigo/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Twist.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/hannrs_msgs
)
_generate_msg_py(hannrs_msgs
  "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/msg/InteractionSet.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/indigo/share/std_msgs/cmake/../msg/Float64.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/hannrs_msgs
)
_generate_msg_py(hannrs_msgs
  "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/msg/VisionDetection.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/indigo/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/hannrs_msgs
)
_generate_msg_py(hannrs_msgs
  "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/msg/Person.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Vector3.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Twist.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Point.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Pose.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/hannrs_msgs
)
_generate_msg_py(hannrs_msgs
  "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/msg/InteractionSets.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/indigo/share/std_msgs/cmake/../msg/Float64.msg;/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/msg/InteractionSet.msg;/opt/ros/indigo/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/hannrs_msgs
)

### Generating Services
_generate_srv_py(hannrs_msgs
  "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/srv/Get_Person.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Point.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/indigo/share/std_msgs/cmake/../msg/String.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Pose.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/hannrs_msgs
)
_generate_srv_py(hannrs_msgs
  "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/srv/Prox_Goal.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/indigo/share/std_msgs/cmake/../msg/Float64.msg;/opt/ros/indigo/share/std_msgs/cmake/../msg/String.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/hannrs_msgs
)

### Generating Module File
_generate_module_py(hannrs_msgs
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/hannrs_msgs
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(hannrs_msgs_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(hannrs_msgs_generate_messages hannrs_msgs_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/srv/Prox_Goal.srv" NAME_WE)
add_dependencies(hannrs_msgs_generate_messages_py _hannrs_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/msg/Goal.msg" NAME_WE)
add_dependencies(hannrs_msgs_generate_messages_py _hannrs_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/msg/People.msg" NAME_WE)
add_dependencies(hannrs_msgs_generate_messages_py _hannrs_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/srv/Get_Person.srv" NAME_WE)
add_dependencies(hannrs_msgs_generate_messages_py _hannrs_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/msg/InteractionSet.msg" NAME_WE)
add_dependencies(hannrs_msgs_generate_messages_py _hannrs_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/msg/VisionDetection.msg" NAME_WE)
add_dependencies(hannrs_msgs_generate_messages_py _hannrs_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/msg/Person.msg" NAME_WE)
add_dependencies(hannrs_msgs_generate_messages_py _hannrs_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/amateus/HANNRS/catkin_ws/src/hannrs_msgs/msg/InteractionSets.msg" NAME_WE)
add_dependencies(hannrs_msgs_generate_messages_py _hannrs_msgs_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(hannrs_msgs_genpy)
add_dependencies(hannrs_msgs_genpy hannrs_msgs_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS hannrs_msgs_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/hannrs_msgs)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/hannrs_msgs
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
add_dependencies(hannrs_msgs_generate_messages_cpp geometry_msgs_generate_messages_cpp)
add_dependencies(hannrs_msgs_generate_messages_cpp std_msgs_generate_messages_cpp)

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/hannrs_msgs)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/hannrs_msgs
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
add_dependencies(hannrs_msgs_generate_messages_lisp geometry_msgs_generate_messages_lisp)
add_dependencies(hannrs_msgs_generate_messages_lisp std_msgs_generate_messages_lisp)

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/hannrs_msgs)
  install(CODE "execute_process(COMMAND \"/usr/bin/python\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/hannrs_msgs\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/hannrs_msgs
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
add_dependencies(hannrs_msgs_generate_messages_py geometry_msgs_generate_messages_py)
add_dependencies(hannrs_msgs_generate_messages_py std_msgs_generate_messages_py)
