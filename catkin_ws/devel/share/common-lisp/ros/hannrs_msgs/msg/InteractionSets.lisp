; Auto-generated. Do not edit!


(cl:in-package hannrs_msgs-msg)


;//! \htmlinclude InteractionSets.msg.html

(cl:defclass <InteractionSets> (roslisp-msg-protocol:ros-message)
  ((header
    :reader header
    :initarg :header
    :type std_msgs-msg:Header
    :initform (cl:make-instance 'std_msgs-msg:Header))
   (interaction_sets
    :reader interaction_sets
    :initarg :interaction_sets
    :type (cl:vector hannrs_msgs-msg:InteractionSet)
   :initform (cl:make-array 0 :element-type 'hannrs_msgs-msg:InteractionSet :initial-element (cl:make-instance 'hannrs_msgs-msg:InteractionSet))))
)

(cl:defclass InteractionSets (<InteractionSets>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <InteractionSets>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'InteractionSets)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name hannrs_msgs-msg:<InteractionSets> is deprecated: use hannrs_msgs-msg:InteractionSets instead.")))

(cl:ensure-generic-function 'header-val :lambda-list '(m))
(cl:defmethod header-val ((m <InteractionSets>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hannrs_msgs-msg:header-val is deprecated.  Use hannrs_msgs-msg:header instead.")
  (header m))

(cl:ensure-generic-function 'interaction_sets-val :lambda-list '(m))
(cl:defmethod interaction_sets-val ((m <InteractionSets>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hannrs_msgs-msg:interaction_sets-val is deprecated.  Use hannrs_msgs-msg:interaction_sets instead.")
  (interaction_sets m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <InteractionSets>) ostream)
  "Serializes a message object of type '<InteractionSets>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'header) ostream)
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'interaction_sets))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (roslisp-msg-protocol:serialize ele ostream))
   (cl:slot-value msg 'interaction_sets))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <InteractionSets>) istream)
  "Deserializes a message object of type '<InteractionSets>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'header) istream)
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'interaction_sets) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'interaction_sets)))
    (cl:dotimes (i __ros_arr_len)
    (cl:setf (cl:aref vals i) (cl:make-instance 'hannrs_msgs-msg:InteractionSet))
  (roslisp-msg-protocol:deserialize (cl:aref vals i) istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<InteractionSets>)))
  "Returns string type for a message object of type '<InteractionSets>"
  "hannrs_msgs/InteractionSets")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'InteractionSets)))
  "Returns string type for a message object of type 'InteractionSets"
  "hannrs_msgs/InteractionSets")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<InteractionSets>)))
  "Returns md5sum for a message object of type '<InteractionSets>"
  "5ded484679810030225d5056ea06c63e")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'InteractionSets)))
  "Returns md5sum for a message object of type 'InteractionSets"
  "5ded484679810030225d5056ea06c63e")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<InteractionSets>)))
  "Returns full string definition for message of type '<InteractionSets>"
  (cl:format cl:nil "Header header~%hannrs_msgs/InteractionSet[] interaction_sets~%~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%# 0: no frame~%# 1: global frame~%string frame_id~%~%================================================================================~%MSG: hannrs_msgs/InteractionSet~%geometry_msgs/Point center~%std_msgs/Float64 radius~%std_msgs/Float64 importance~%~%================================================================================~%MSG: geometry_msgs/Point~%# This contains the position of a point in free space~%float64 x~%float64 y~%float64 z~%~%================================================================================~%MSG: std_msgs/Float64~%float64 data~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'InteractionSets)))
  "Returns full string definition for message of type 'InteractionSets"
  (cl:format cl:nil "Header header~%hannrs_msgs/InteractionSet[] interaction_sets~%~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%# 0: no frame~%# 1: global frame~%string frame_id~%~%================================================================================~%MSG: hannrs_msgs/InteractionSet~%geometry_msgs/Point center~%std_msgs/Float64 radius~%std_msgs/Float64 importance~%~%================================================================================~%MSG: geometry_msgs/Point~%# This contains the position of a point in free space~%float64 x~%float64 y~%float64 z~%~%================================================================================~%MSG: std_msgs/Float64~%float64 data~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <InteractionSets>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'header))
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'interaction_sets) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ (roslisp-msg-protocol:serialization-length ele))))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <InteractionSets>))
  "Converts a ROS message object to a list"
  (cl:list 'InteractionSets
    (cl:cons ':header (header msg))
    (cl:cons ':interaction_sets (interaction_sets msg))
))
