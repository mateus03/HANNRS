; Auto-generated. Do not edit!


(cl:in-package hannrs_msgs-srv)


;//! \htmlinclude Get_Person-request.msg.html

(cl:defclass <Get_Person-request> (roslisp-msg-protocol:ros-message)
  ((name
    :reader name
    :initarg :name
    :type std_msgs-msg:String
    :initform (cl:make-instance 'std_msgs-msg:String)))
)

(cl:defclass Get_Person-request (<Get_Person-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <Get_Person-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'Get_Person-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name hannrs_msgs-srv:<Get_Person-request> is deprecated: use hannrs_msgs-srv:Get_Person-request instead.")))

(cl:ensure-generic-function 'name-val :lambda-list '(m))
(cl:defmethod name-val ((m <Get_Person-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hannrs_msgs-srv:name-val is deprecated.  Use hannrs_msgs-srv:name instead.")
  (name m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <Get_Person-request>) ostream)
  "Serializes a message object of type '<Get_Person-request>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'name) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <Get_Person-request>) istream)
  "Deserializes a message object of type '<Get_Person-request>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'name) istream)
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<Get_Person-request>)))
  "Returns string type for a service object of type '<Get_Person-request>"
  "hannrs_msgs/Get_PersonRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'Get_Person-request)))
  "Returns string type for a service object of type 'Get_Person-request"
  "hannrs_msgs/Get_PersonRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<Get_Person-request>)))
  "Returns md5sum for a message object of type '<Get_Person-request>"
  "26f0c8a5e492d0bf40eb98ffffc050e6")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'Get_Person-request)))
  "Returns md5sum for a message object of type 'Get_Person-request"
  "26f0c8a5e492d0bf40eb98ffffc050e6")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<Get_Person-request>)))
  "Returns full string definition for message of type '<Get_Person-request>"
  (cl:format cl:nil "std_msgs/String name~%~%================================================================================~%MSG: std_msgs/String~%string data~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'Get_Person-request)))
  "Returns full string definition for message of type 'Get_Person-request"
  (cl:format cl:nil "std_msgs/String name~%~%================================================================================~%MSG: std_msgs/String~%string data~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <Get_Person-request>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'name))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <Get_Person-request>))
  "Converts a ROS message object to a list"
  (cl:list 'Get_Person-request
    (cl:cons ':name (name msg))
))
;//! \htmlinclude Get_Person-response.msg.html

(cl:defclass <Get_Person-response> (roslisp-msg-protocol:ros-message)
  ((pose
    :reader pose
    :initarg :pose
    :type geometry_msgs-msg:Pose
    :initform (cl:make-instance 'geometry_msgs-msg:Pose))
   (status
    :reader status
    :initarg :status
    :type std_msgs-msg:String
    :initform (cl:make-instance 'std_msgs-msg:String)))
)

(cl:defclass Get_Person-response (<Get_Person-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <Get_Person-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'Get_Person-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name hannrs_msgs-srv:<Get_Person-response> is deprecated: use hannrs_msgs-srv:Get_Person-response instead.")))

(cl:ensure-generic-function 'pose-val :lambda-list '(m))
(cl:defmethod pose-val ((m <Get_Person-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hannrs_msgs-srv:pose-val is deprecated.  Use hannrs_msgs-srv:pose instead.")
  (pose m))

(cl:ensure-generic-function 'status-val :lambda-list '(m))
(cl:defmethod status-val ((m <Get_Person-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hannrs_msgs-srv:status-val is deprecated.  Use hannrs_msgs-srv:status instead.")
  (status m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <Get_Person-response>) ostream)
  "Serializes a message object of type '<Get_Person-response>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'pose) ostream)
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'status) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <Get_Person-response>) istream)
  "Deserializes a message object of type '<Get_Person-response>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'pose) istream)
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'status) istream)
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<Get_Person-response>)))
  "Returns string type for a service object of type '<Get_Person-response>"
  "hannrs_msgs/Get_PersonResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'Get_Person-response)))
  "Returns string type for a service object of type 'Get_Person-response"
  "hannrs_msgs/Get_PersonResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<Get_Person-response>)))
  "Returns md5sum for a message object of type '<Get_Person-response>"
  "26f0c8a5e492d0bf40eb98ffffc050e6")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'Get_Person-response)))
  "Returns md5sum for a message object of type 'Get_Person-response"
  "26f0c8a5e492d0bf40eb98ffffc050e6")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<Get_Person-response>)))
  "Returns full string definition for message of type '<Get_Person-response>"
  (cl:format cl:nil "geometry_msgs/Pose pose~%std_msgs/String status~%~%================================================================================~%MSG: geometry_msgs/Pose~%# A representation of pose in free space, composed of postion and orientation. ~%Point position~%Quaternion orientation~%~%================================================================================~%MSG: geometry_msgs/Point~%# This contains the position of a point in free space~%float64 x~%float64 y~%float64 z~%~%================================================================================~%MSG: geometry_msgs/Quaternion~%# This represents an orientation in free space in quaternion form.~%~%float64 x~%float64 y~%float64 z~%float64 w~%~%================================================================================~%MSG: std_msgs/String~%string data~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'Get_Person-response)))
  "Returns full string definition for message of type 'Get_Person-response"
  (cl:format cl:nil "geometry_msgs/Pose pose~%std_msgs/String status~%~%================================================================================~%MSG: geometry_msgs/Pose~%# A representation of pose in free space, composed of postion and orientation. ~%Point position~%Quaternion orientation~%~%================================================================================~%MSG: geometry_msgs/Point~%# This contains the position of a point in free space~%float64 x~%float64 y~%float64 z~%~%================================================================================~%MSG: geometry_msgs/Quaternion~%# This represents an orientation in free space in quaternion form.~%~%float64 x~%float64 y~%float64 z~%float64 w~%~%================================================================================~%MSG: std_msgs/String~%string data~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <Get_Person-response>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'pose))
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'status))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <Get_Person-response>))
  "Converts a ROS message object to a list"
  (cl:list 'Get_Person-response
    (cl:cons ':pose (pose msg))
    (cl:cons ':status (status msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'Get_Person)))
  'Get_Person-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'Get_Person)))
  'Get_Person-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'Get_Person)))
  "Returns string type for a service object of type '<Get_Person>"
  "hannrs_msgs/Get_Person")