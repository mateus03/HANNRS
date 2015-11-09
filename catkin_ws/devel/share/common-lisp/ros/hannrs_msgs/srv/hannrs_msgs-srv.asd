
(cl:in-package :asdf)

(defsystem "hannrs_msgs-srv"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :geometry_msgs-msg
               :std_msgs-msg
)
  :components ((:file "_package")
    (:file "Prox_Goal" :depends-on ("_package_Prox_Goal"))
    (:file "_package_Prox_Goal" :depends-on ("_package"))
    (:file "Get_Person" :depends-on ("_package_Get_Person"))
    (:file "_package_Get_Person" :depends-on ("_package"))
  ))