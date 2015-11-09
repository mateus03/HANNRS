
(cl:in-package :asdf)

(defsystem "hannrs_msgs-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :geometry_msgs-msg
               :std_msgs-msg
)
  :components ((:file "_package")
    (:file "InteractionSet" :depends-on ("_package_InteractionSet"))
    (:file "_package_InteractionSet" :depends-on ("_package"))
    (:file "Goal" :depends-on ("_package_Goal"))
    (:file "_package_Goal" :depends-on ("_package"))
    (:file "InteractionSets" :depends-on ("_package_InteractionSets"))
    (:file "_package_InteractionSets" :depends-on ("_package"))
    (:file "People" :depends-on ("_package_People"))
    (:file "_package_People" :depends-on ("_package"))
    (:file "VisionDetection" :depends-on ("_package_VisionDetection"))
    (:file "_package_VisionDetection" :depends-on ("_package"))
    (:file "Person" :depends-on ("_package_Person"))
    (:file "_package_Person" :depends-on ("_package"))
  ))