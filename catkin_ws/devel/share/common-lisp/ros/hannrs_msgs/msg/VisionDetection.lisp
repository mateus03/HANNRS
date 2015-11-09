; Auto-generated. Do not edit!


(cl:in-package hannrs_msgs-msg)


;//! \htmlinclude VisionDetection.msg.html

(cl:defclass <VisionDetection> (roslisp-msg-protocol:ros-message)
  ((header
    :reader header
    :initarg :header
    :type std_msgs-msg:Header
    :initform (cl:make-instance 'std_msgs-msg:Header))
   (positions
    :reader positions
    :initarg :positions
    :type (cl:vector geometry_msgs-msg:Point)
   :initform (cl:make-array 0 :element-type 'geometry_msgs-msg:Point :initial-element (cl:make-instance 'geometry_msgs-msg:Point)))
   (classifications
    :reader classifications
    :initarg :classifications
    :type (cl:vector cl:float)
   :initform (cl:make-array 0 :element-type 'cl:float :initial-element 0.0))
   (camera
    :reader camera
    :initarg :camera
    :type cl:string
    :initform ""))
)

(cl:defclass VisionDetection (<VisionDetection>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <VisionDetection>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'VisionDetection)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name hannrs_msgs-msg:<VisionDetection> is deprecated: use hannrs_msgs-msg:VisionDetection instead.")))

(cl:ensure-generic-function 'header-val :lambda-list '(m))
(cl:defmethod header-val ((m <VisionDetection>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hannrs_msgs-msg:header-val is deprecated.  Use hannrs_msgs-msg:header instead.")
  (header m))

(cl:ensure-generic-function 'positions-val :lambda-list '(m))
(cl:defmethod positions-val ((m <VisionDetection>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hannrs_msgs-msg:positions-val is deprecated.  Use hannrs_msgs-msg:positions instead.")
  (positions m))

(cl:ensure-generic-function 'classifications-val :lambda-list '(m))
(cl:defmethod classifications-val ((m <VisionDetection>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hannrs_msgs-msg:classifications-val is deprecated.  Use hannrs_msgs-msg:classifications instead.")
  (classifications m))

(cl:ensure-generic-function 'camera-val :lambda-list '(m))
(cl:defmethod camera-val ((m <VisionDetection>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hannrs_msgs-msg:camera-val is deprecated.  Use hannrs_msgs-msg:camera instead.")
  (camera m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <VisionDetection>) ostream)
  "Serializes a message object of type '<VisionDetection>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'header) ostream)
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'positions))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (roslisp-msg-protocol:serialize ele ostream))
   (cl:slot-value msg 'positions))
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'classifications))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (cl:let ((bits (roslisp-utils:encode-single-float-bits ele)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)))
   (cl:slot-value msg 'classifications))
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'camera))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'camera))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <VisionDetection>) istream)
  "Deserializes a message object of type '<VisionDetection>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'header) istream)
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'positions) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'positions)))
    (cl:dotimes (i __ros_arr_len)
    (cl:setf (cl:aref vals i) (cl:make-instance 'geometry_msgs-msg:Point))
  (roslisp-msg-protocol:deserialize (cl:aref vals i) istream))))
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'classifications) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'classifications)))
    (cl:dotimes (i __ros_arr_len)
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:aref vals i) (roslisp-utils:decode-single-float-bits bits))))))
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'camera) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'camera) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<VisionDetection>)))
  "Returns string type for a message object of type '<VisionDetection>"
  "hannrs_msgs/VisionDetection")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'VisionDetection)))
  "Returns string type for a message object of type 'VisionDetection"
  "hannrs_msgs/VisionDetection")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<VisionDetection>)))
  "Returns md5sum for a message object of type '<VisionDetection>"
  "14445a7b0a15418cdcb6155f2f316869")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'VisionDetection)))
  "Returns md5sum for a message object of type 'VisionDetection"
  "14445a7b0a15418cdcb6155f2f316869")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<VisionDetection>)))
  "Returns full string definition for message of type '<VisionDetection>"
  (cl:format cl:nil "Header header~%geometry_msgs/Point[] positions~%float32[] classifications~%string camera~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%# 0: no frame~%# 1: global frame~%string frame_id~%~%================================================================================~%MSG: geometry_msgs/Point~%# This contains the position of a point in free space~%float64 x~%float64 y~%float64 z~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'VisionDetection)))
  "Returns full string definition for message of type 'VisionDetection"
  (cl:format cl:nil "Header header~%geometry_msgs/Point[] positions~%float32[] classifications~%string camera~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%# 0: no frame~%# 1: global frame~%string frame_id~%~%================================================================================~%MSG: geometry_msgs/Point~%# This contains the position of a point in free space~%float64 x~%float64 y~%float64 z~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <VisionDetection>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'header))
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'positions) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ (roslisp-msg-protocol:serialization-length ele))))
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'classifications) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ 4)))
     4 (cl:length (cl:slot-value msg 'camera))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <VisionDetection>))
  "Converts a ROS message object to a list"
  (cl:list 'VisionDetection
    (cl:cons ':header (header msg))
    (cl:cons ':positions (positions msg))
    (cl:cons ':classifications (classifications msg))
    (cl:cons ':camera (camera msg))
))
