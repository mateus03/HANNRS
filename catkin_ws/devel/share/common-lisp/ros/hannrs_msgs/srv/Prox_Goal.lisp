; Auto-generated. Do not edit!


(cl:in-package hannrs_msgs-srv)


;//! \htmlinclude Prox_Goal-request.msg.html

(cl:defclass <Prox_Goal-request> (roslisp-msg-protocol:ros-message)
  ((action
    :reader action
    :initarg :action
    :type std_msgs-msg:String
    :initform (cl:make-instance 'std_msgs-msg:String))
   (person
    :reader person
    :initarg :person
    :type std_msgs-msg:String
    :initform (cl:make-instance 'std_msgs-msg:String)))
)

(cl:defclass Prox_Goal-request (<Prox_Goal-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <Prox_Goal-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'Prox_Goal-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name hannrs_msgs-srv:<Prox_Goal-request> is deprecated: use hannrs_msgs-srv:Prox_Goal-request instead.")))

(cl:ensure-generic-function 'action-val :lambda-list '(m))
(cl:defmethod action-val ((m <Prox_Goal-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hannrs_msgs-srv:action-val is deprecated.  Use hannrs_msgs-srv:action instead.")
  (action m))

(cl:ensure-generic-function 'person-val :lambda-list '(m))
(cl:defmethod person-val ((m <Prox_Goal-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hannrs_msgs-srv:person-val is deprecated.  Use hannrs_msgs-srv:person instead.")
  (person m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <Prox_Goal-request>) ostream)
  "Serializes a message object of type '<Prox_Goal-request>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'action) ostream)
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'person) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <Prox_Goal-request>) istream)
  "Deserializes a message object of type '<Prox_Goal-request>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'action) istream)
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'person) istream)
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<Prox_Goal-request>)))
  "Returns string type for a service object of type '<Prox_Goal-request>"
  "hannrs_msgs/Prox_GoalRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'Prox_Goal-request)))
  "Returns string type for a service object of type 'Prox_Goal-request"
  "hannrs_msgs/Prox_GoalRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<Prox_Goal-request>)))
  "Returns md5sum for a message object of type '<Prox_Goal-request>"
  "8e39ad254c03e98e247f822ee5a87824")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'Prox_Goal-request)))
  "Returns md5sum for a message object of type 'Prox_Goal-request"
  "8e39ad254c03e98e247f822ee5a87824")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<Prox_Goal-request>)))
  "Returns full string definition for message of type '<Prox_Goal-request>"
  (cl:format cl:nil "std_msgs/String action~%std_msgs/String person~%~%================================================================================~%MSG: std_msgs/String~%string data~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'Prox_Goal-request)))
  "Returns full string definition for message of type 'Prox_Goal-request"
  (cl:format cl:nil "std_msgs/String action~%std_msgs/String person~%~%================================================================================~%MSG: std_msgs/String~%string data~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <Prox_Goal-request>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'action))
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'person))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <Prox_Goal-request>))
  "Converts a ROS message object to a list"
  (cl:list 'Prox_Goal-request
    (cl:cons ':action (action msg))
    (cl:cons ':person (person msg))
))
;//! \htmlinclude Prox_Goal-response.msg.html

(cl:defclass <Prox_Goal-response> (roslisp-msg-protocol:ros-message)
  ((wait_time
    :reader wait_time
    :initarg :wait_time
    :type std_msgs-msg:Float64
    :initform (cl:make-instance 'std_msgs-msg:Float64)))
)

(cl:defclass Prox_Goal-response (<Prox_Goal-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <Prox_Goal-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'Prox_Goal-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name hannrs_msgs-srv:<Prox_Goal-response> is deprecated: use hannrs_msgs-srv:Prox_Goal-response instead.")))

(cl:ensure-generic-function 'wait_time-val :lambda-list '(m))
(cl:defmethod wait_time-val ((m <Prox_Goal-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hannrs_msgs-srv:wait_time-val is deprecated.  Use hannrs_msgs-srv:wait_time instead.")
  (wait_time m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <Prox_Goal-response>) ostream)
  "Serializes a message object of type '<Prox_Goal-response>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'wait_time) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <Prox_Goal-response>) istream)
  "Deserializes a message object of type '<Prox_Goal-response>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'wait_time) istream)
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<Prox_Goal-response>)))
  "Returns string type for a service object of type '<Prox_Goal-response>"
  "hannrs_msgs/Prox_GoalResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'Prox_Goal-response)))
  "Returns string type for a service object of type 'Prox_Goal-response"
  "hannrs_msgs/Prox_GoalResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<Prox_Goal-response>)))
  "Returns md5sum for a message object of type '<Prox_Goal-response>"
  "8e39ad254c03e98e247f822ee5a87824")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'Prox_Goal-response)))
  "Returns md5sum for a message object of type 'Prox_Goal-response"
  "8e39ad254c03e98e247f822ee5a87824")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<Prox_Goal-response>)))
  "Returns full string definition for message of type '<Prox_Goal-response>"
  (cl:format cl:nil "std_msgs/Float64 wait_time~%~%================================================================================~%MSG: std_msgs/Float64~%float64 data~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'Prox_Goal-response)))
  "Returns full string definition for message of type 'Prox_Goal-response"
  (cl:format cl:nil "std_msgs/Float64 wait_time~%~%================================================================================~%MSG: std_msgs/Float64~%float64 data~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <Prox_Goal-response>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'wait_time))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <Prox_Goal-response>))
  "Converts a ROS message object to a list"
  (cl:list 'Prox_Goal-response
    (cl:cons ':wait_time (wait_time msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'Prox_Goal)))
  'Prox_Goal-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'Prox_Goal)))
  'Prox_Goal-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'Prox_Goal)))
  "Returns string type for a service object of type '<Prox_Goal>"
  "hannrs_msgs/Prox_Goal")