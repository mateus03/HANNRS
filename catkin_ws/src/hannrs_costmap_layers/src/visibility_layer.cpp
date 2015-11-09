#include <hannrs_costmap_layers/visibility_layer.h>
#include <pluginlib/class_list_macros.h>

PLUGINLIB_EXPORT_CLASS(hannrs_costmap_layers::VisibilityLayer, costmap_2d::Layer)

using costmap_2d::LETHAL_OBSTACLE;
using costmap_2d::NO_INFORMATION;

namespace hannrs_costmap_layers{
  
  VisibilityLayer::VisibilityLayer() {}
  
  void VisibilityLayer::onInitialize(){
    ros::NodeHandle nh("~/" + name_);
    people_sub = nh.subscribe("/hannrs/people", 1, &VisibilityLayer::peopleCallBack, this);
    current_ = true;

    dsrv_ = new dynamic_reconfigure::Server<VisibilityLayerConfig>(nh);
    dynamic_reconfigure::Server<VisibilityLayerConfig>::CallbackType cb = boost::bind(&VisibilityLayer::reconfigureCB, this, _1, _2);
    dsrv_->setCallback(cb);
  }
  
  void VisibilityLayer::peopleCallBack(const hannrs_msgs::People::ConstPtr& msg){
    boost::recursive_mutex::scoped_lock lock(lock_);
    people_list = *msg;
  }
  
  void VisibilityLayer::reconfigureCB(VisibilityLayerConfig &config, uint32_t level){
    enabled_ = config.enabled;
    sigma_h_ = config.sigma_h;
    factor_r_ = config.factor_r;
    factor_s_ = config.factor_s;
    amplitude_ = config.amplitude;
    safety_dist_ = config.Safety_dist;
    cutoff_ = config.Cutoff;
  }
  
  void VisibilityLayer::updateBounds(double origin_x, double origin_y, double origin_yaw, double* min_x, double* min_y, double* max_x, double* max_y){
    if (!enabled_)
      return;
    
    std::string global_frame = layered_costmap_->getGlobalFrameID();
    transformed_people_.clear();
    
    for(unsigned int i = 0; i < people_list.people.size(); i++){
      hannrs_msgs::Person& person = people_list.people[i];
      hannrs_msgs::Person persontrans;
      geometry_msgs::PoseStamped pose2t, posetrans;
      geometry_msgs::Vector3Stamped lin2t, ang2t, lintrans, angtrans;
      
      if(person.status.compare("sitting") == 0){
	try{
	  //transform the person pose
	  pose2t.pose = person.pose;
	  pose2t.header.frame_id = people_list.header.frame_id;
	  tf_.transformPose(global_frame,pose2t, posetrans);
	  
	  //transform the person velocity
	  lin2t.vector = person.velocity.linear;
	  ang2t.vector = person.velocity.angular;
	  lin2t.header.frame_id = people_list.header.frame_id;
	  ang2t.header.frame_id = people_list.header.frame_id;
	  tf_.transformVector(global_frame,lin2t,lintrans);
	  tf_.transformVector(global_frame,ang2t,angtrans);
	  
	  //fill the person attributes
	  persontrans.pose = posetrans.pose;
	  persontrans.velocity.linear = lintrans.vector;
	  persontrans.velocity.angular = angtrans.vector;
	  persontrans.name = person.name;
	  persontrans.status = person.status;
	  
	  //put it into the list of transformed people
	  transformed_people_.push_back(persontrans);
	  
	  *min_x = std::min(*min_x, persontrans.pose.position.x - safety_dist_);
	  *min_y = std::min(*min_y, persontrans.pose.position.y - safety_dist_);
	  *max_x = std::max(*max_x, persontrans.pose.position.x + safety_dist_);
	  *max_y = std::max(*max_y, persontrans.pose.position.y + safety_dist_);
	  
	}
	catch(tf::LookupException& ex) {
	  ROS_ERROR("No Transform available Error: %s\n", ex.what());
	  continue;
	}
	catch(tf::ConnectivityException& ex) {
	  ROS_ERROR("Connectivity Error: %s\n", ex.what());
	  continue;
	}
	catch(tf::ExtrapolationException& ex) {
	  ROS_ERROR("Extrapolation Error: %s\n", ex.what());
	  continue;
	}
      }
    }
  }
  
  void VisibilityLayer::updateCosts(costmap_2d::Costmap2D& master_grid, int min_i, int min_j, int max_i, int max_j){
    boost::recursive_mutex::scoped_lock lock(lock_);
    if (!enabled_)
      return;
    
    if( people_list.people.size() == 0 )
          return;
    
    hannrs_msgs::Person person;
    costmap_2d::Costmap2D* costmap = layered_costmap_->getCostmap();
    double res = costmap->getResolution();
    
    for(unsigned int i = 0; i < transformed_people_.size(); i++){
      person = transformed_people_[i];
      double pos_x = person.pose.position.x, pos_y = person.pose.position.y;
      
      double dx = safety_dist_, dy = safety_dist_;
      
      unsigned int start_x, start_y, end_x, end_y;
      
      if(costmap->worldToMap(pos_x - dx, pos_y - dy, start_x, start_y)){
      }else{
	start_x = 0;
	start_y = 0;
      }
      if(costmap->worldToMap(pos_x + dx, pos_y + dy, end_x, end_y)){
      }else{
	end_x = costmap->getSizeInCellsX();
	end_y = costmap->getSizeInCellsY();
      }
      
      if(start_x < min_i){
	start_x = min_i;
      }
      if(start_y < min_j){
	start_y = min_j;
      }
      if(end_x > max_i){
	end_x = max_i;
      }
      if(end_y > max_j){
	end_y = max_j;
      }
      
      double x,y;
      double f;
      for(int k = start_x; k < end_x; k++){
	for(int l = start_y; l < end_y; l++){
	  unsigned char old_cost = costmap->getCost(k, l);
	  if(old_cost == costmap_2d::NO_INFORMATION)
	    continue;
	  
	 
	  costmap->mapToWorld(k,l,x,y);
	  double theta = tf::getYaw(person.pose.orientation) + M_PI;
	  theta = constrainAngle(theta*180/M_PI)*M_PI/180;

	  f = assymetric_gaussian(amplitude_, x, y, pos_x, pos_y, theta, sigma_h_, factor_s_*sigma_h_, factor_r_*sigma_h_);
	  
	  if(f<cutoff_)
	    continue;
	  
	  unsigned char cvalue = (unsigned char) f;
	  costmap->setCost(k, l, std::max(cvalue, old_cost));
	}
	
      }
      
    }

  }
}