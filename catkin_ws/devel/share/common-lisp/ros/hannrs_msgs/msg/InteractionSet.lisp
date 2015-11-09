; Auto-generated. Do not edit!


(cl:in-package hannrs_msgs-msg)


;//! \htmlinclude InteractionSet.msg.html

(cl:defclass <InteractionSet> (roslisp-msg-protocol:ros-message)
  ((center
    :reader center
    :initarg :center
    :type geometry_msgs-msg:Point
    :initform (cl:make-instance 'geometry_msgs-msg:Point))
   (radius
    :reader radius
    :initarg :radius
    :type std_msgs-msg:Float64
    :initform (cl:make-instance 'std_msgs-msg:Float64))
   (importance
    :reader importance
    :initarg :importance
    :type std_msgs-msg:Float64
    :initform (cl:make-instance 'std_msgs-msg:Float64)))
)

(cl:defclass InteractionSet (<InteractionSet>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <InteractionSet>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'InteractionSet)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name hannrs_msgs-msg:<InteractionSet> is deprecated: use hannrs_msgs-msg:InteractionSet instead.")))

(cl:ensure-generic-function 'center-val :lambda-list '(m))
(cl:defmethod center-val ((m <InteractionSet>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hannrs_msgs-msg:center-val is deprecated.  Use hannrs_msgs-msg:center instead.")
  (center m))

(cl:ensure-generic-function 'radius-val :lambda-list '(m))
(cl:defmethod radius-val ((m <InteractionSet>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hannrs_msgs-msg:radius-val is deprecated.  Use hannrs_msgs-msg:radius instead.")
  (radius m))

(cl:ensure-generic-function 'importance-val :lambda-list '(m))
(cl:defmethod importance-val ((m <InteractionSet>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hannrs_msgs-msg:importance-val is deprecated.  Use hannrs_msgs-msg:importance instead.")
  (importance m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <InteractionSet>) ostream)
  "Serializes a message object of type '<InteractionSet>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'center) ostream)
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'radius) ostream)
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'importance) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <InteractionSet>) istream)
  "Deserializes a message object of type '<InteractionSet>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'center) istream)
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'radius) istream)
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'importance) istream)
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<InteractionSet>)))
  "Returns string type for a message object of type '<InteractionSet>"
  "hannrs_msgs/InteractionSet")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'InteractionSet)))
  "Returns string type for a message object of type 'InteractionSet"
  "hannrs_msgs/InteractionSet")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<InteractionSet>)))
  "Returns md5sum for a message object of type '<InteractionSet>"
  "bb76bfc545446f6e2a45fcee2a9e6bc8")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'InteractionSet)))
  "Returns md5sum for a message object of type 'InteractionSet"
  "bb76bfc545446f6e2a45fcee2a9e6bc8")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<InteractionSet>)))
  "Returns full string definition for message of type '<InteractionSet>"
  (cl:format cl:nil "geometry_msgs/Point center~%std_msgs/Float64 radius~%std_msgs/Float64 importance~%~%================================================================================~%MSG: geometry_msgs/Point~%# This contains the position of a point in free space~%float64 x~%float64 y~%float64 z~%~%================================================================================~%MSG: std_msgs/Float64~%float64 data~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'InteractionSet)))
  "Returns full string definition for message of type 'InteractionSet"
  (cl:format cl:nil "geometry_msgs/Point center~%std_msgs/Float64 radius~%std_msgs/Float64 importance~%~%================================================================================~%MSG: geometry_msgs/Point~%# This contains the position of a point in free space~%float64 x~%float64 y~%float64 z~%~%================================================================================~%MSG: std_msgs/Float64~%float64 data~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <InteractionSet>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'center))
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'radius))
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'importance))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <InteractionSet>))
  "Converts a ROS message object to a list"
  (cl:list 'InteractionSet
    (cl:cons ':center (center msg))
    (cl:cons ':radius (radius msg))
    (cl:cons ':importance (importance msg))
))
