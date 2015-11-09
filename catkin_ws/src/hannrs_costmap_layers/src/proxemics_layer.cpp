#include <hannrs_costmap_layers/proxemics_layer.h>
#include <pluginlib/class_list_macros.h>

PLUGINLIB_EXPORT_CLASS(hannrs_costmap_layers::ProxemicsLayer, costmap_2d::Layer)

using costmap_2d::LETHAL_OBSTACLE;
using costmap_2d::NO_INFORMATION;

double symetric_gaussian(double A, double x, double y, double xc, double yc, double sigma){
  double fx = pow(x-xc,2)/(2*pow(sigma,2));
  double fy = pow(y-yc,2)/(2*pow(sigma,2));
  
  return A*exp(-(fx+fy));
}

double assymetric_gaussian(double A, double x, double y, double xc, double yc, double theta, double sigma_h, double sigma_s, double sigma_r){
  double alpha = atan2(y-yc,x-xc) - theta + M_PI/2;
  alpha = constrainAngle(alpha*180/M_PI)*M_PI/180;
  double sigma;
  if(alpha <= 0){
    sigma = sigma_r;
  }else{
    sigma = sigma_h;
  }
  double a = pow(cos(theta),2)/(2*pow(sigma,2)) + pow(sin(theta),2)/(2*pow(sigma_s,2));
  double b = sin(2*theta)/pow(2*sigma,2) - sin(2*theta)/pow(2*sigma_s,2);
  double c = pow(sin(theta),2)/(2*pow(sigma,2)) + pow(cos(theta),2)/(2*pow(sigma_s,2));
  
  return A*exp(-(a*pow(x-xc,2) + 2*b*(x-xc)*(y-yc) + c*pow(y-yc,2)));
}

double constrainAngle(double x){
    x = fmod(x + 180,360);
    if (x < 0)
        x += 360;
    return x - 180;
}

namespace hannrs_costmap_layers{
  
  ProxemicsLayer::ProxemicsLayer() {}
  
  void ProxemicsLayer::onInitialize(){
    ros::NodeHandle nh("~/" + name_);
    people_sub = nh.subscribe("/hannrs/people", 1, &ProxemicsLayer::peopleCallBack, this);
    service = nh.advertiseService("prox_goal", &ProxemicsLayer::prox_goalCallBack, this);
    current_ = true;
    
    dsrv_ = new dynamic_reconfigure::Server<ProxemicsLayerConfig>(nh);
    dynamic_reconfigure::Server<ProxemicsLayerConfig>::CallbackType cb = boost::bind(&ProxemicsLayer::reconfigureCB, this, _1, _2);
    dsrv_->setCallback(cb);
  }
  
  void ProxemicsLayer::peopleCallBack(const hannrs_msgs::People::ConstPtr& msg){
    boost::recursive_mutex::scoped_lock lock(lock_);
    people_list = *msg;
  }
  
  bool ProxemicsLayer::prox_goalCallBack(hannrs_msgs::Prox_Goal::Request& req, hannrs_msgs::Prox_Goal::Response& res){
    action_ = req.action.data;
    person_ = req.person.data;
    res.wait_time.data = 5.0;
    
    return true;
  }
  
  void ProxemicsLayer::reconfigureCB(ProxemicsLayerConfig &config, uint32_t level){
    enabled_ = config.enabled;
    sigma_h_ = config.sigma_h;
    factor_r_ = config.factor_r;
    factor_s_ = config.factor_s;
    amplitude_ = config.amplitude;
    safety_dist_ = config.Safety_dist;
    cutoff_ = config.Cutoff;
    interaction_dist_ = config.interaction_dist;
    interaction_angle_ = config.interaction_angle;
  }
  
  void ProxemicsLayer::updateBounds(double origin_x, double origin_y, double origin_yaw, double* min_x, double* min_y, double* max_x, double* max_y){
    if (!enabled_)
      return;
    
    std::string global_frame = layered_costmap_->getGlobalFrameID();
    people_copy = transformed_people_;
    transformed_people_.clear();
    
    if(people_list.people.empty()){
      for(unsigned int i = 0; i < people_copy.size(); i++){
	if(people_copy[i].status.compare("sitting") == 0 || people_copy[i].status.compare("standing") == 0){
	  *min_x = std::min(*min_x, people_copy[i].pose.position.x - safety_dist_);
	  *min_y = std::min(*min_y, people_copy[i].pose.position.y - safety_dist_);
	  *max_x = std::max(*max_x, people_copy[i].pose.position.x + safety_dist_);
	  *max_y = std::max(*max_y, people_copy[i].pose.position.y + safety_dist_);
	}else{
	  double vel_module = sqrt(pow(people_copy[i].velocity.linear.x, 2) + pow(people_copy[i].velocity.linear.y, 2));
	  if(vel_module < 0.8){
	    vel_module = 0.8;
	  }
	  double yaw = tf::getYaw(people_copy[i].pose.orientation);

	  *min_x = std::min(*min_x, people_copy[i].pose.position.x - safety_dist_ - vel_module);
	  *min_y = std::min(*min_y, people_copy[i].pose.position.y - safety_dist_ - vel_module);
	  *max_x = std::max(*max_x, people_copy[i].pose.position.x + safety_dist_ + vel_module);
	  *max_y = std::max(*max_y, people_copy[i].pose.position.y + safety_dist_ + vel_module);
	}
      }
    }else{
      for(unsigned int i = 0; i < people_list.people.size(); i++){
	hannrs_msgs::Person& person = people_list.people[i];
	hannrs_msgs::Person persontrans;
	geometry_msgs::PoseStamped pose2t, posetrans;
	geometry_msgs::Vector3Stamped lin2t, ang2t, lintrans, angtrans;
	    
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
	  
	  if(people_copy.size() > 0){
	    for(unsigned int j = 0; j < people_copy.size(); j++){
	      if(people_copy[j].name.compare(persontrans.name) == 0){
		if(people_copy[j].status.compare("walking") == 0 && persontrans.status.compare("standing") == 0){
		  double vel_module = sqrt(pow(people_copy[j].velocity.linear.x, 2) + pow(people_copy[j].velocity.linear.y, 2));
		  
		  *min_x = std::min(*min_x, persontrans.pose.position.x - safety_dist_ - vel_module);
		  *min_y = std::min(*min_y, persontrans.pose.position.y - safety_dist_ - vel_module);
		  *max_x = std::max(*max_x, persontrans.pose.position.x + safety_dist_ + vel_module);
		  *max_y = std::max(*max_y, persontrans.pose.position.y + safety_dist_ + vel_module);
		}
	      }
	    }
	  }

	  if(persontrans.status.compare("sitting") == 0 || persontrans.status.compare("standing") == 0){
	    *min_x = std::min(*min_x, persontrans.pose.position.x - safety_dist_);
	    *min_y = std::min(*min_y, persontrans.pose.position.y - safety_dist_);
	    *max_x = std::max(*max_x, persontrans.pose.position.x + safety_dist_);
	    *max_y = std::max(*max_y, persontrans.pose.position.y + safety_dist_);
	  }else{
	    double vel_module = sqrt(pow(persontrans.velocity.linear.x, 2) + pow(persontrans.velocity.linear.y, 2));
	    if(vel_module < 0.8){
	      vel_module = 0.8;
	    }
	    double yaw = tf::getYaw(persontrans.pose.orientation);

	    *min_x = std::min(*min_x, persontrans.pose.position.x - safety_dist_ - vel_module);
	    *min_y = std::min(*min_y, persontrans.pose.position.y - safety_dist_ - vel_module);
	    *max_x = std::max(*max_x, persontrans.pose.position.x + safety_dist_ + vel_module);
	    *max_y = std::max(*max_y, persontrans.pose.position.y + safety_dist_ + vel_module);
	  }
	  
	  
	  
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

  void ProxemicsLayer::updateCosts(costmap_2d::Costmap2D& master_grid, int min_i, int min_j, int max_i, int max_j){
    boost::recursive_mutex::scoped_lock lock(lock_);
    if (!enabled_)
      return;
    
    hannrs_msgs::Person person;
    costmap_2d::Costmap2D* costmap = layered_costmap_->getCostmap();
    double res = costmap->getResolution();
    
    if(people_list.people.empty()){
      for(unsigned int i = 0; i < people_copy.size(); i++){
	person = people_copy[i];
	double vel_module = sqrt(pow(person.velocity.linear.x,2) + pow(person.velocity.linear.y,2));
	double pos_x = person.pose.position.x, pos_y = person.pose.position.y;
	
	double dx, dy;
	if(people_copy.size() > 0){

	}else{
	  if(person.status.compare("sitting") == 0 || person.status.compare("standing") == 0){
	    dx = safety_dist_;
	    dy = safety_dist_;
	  }else{
	    dx = safety_dist_ + vel_module;
	    dy = safety_dist_ + vel_module;
	  }
	}
	
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
	    
	    if(person.status.compare("sitting") == 0 || person.status.compare("standing") == 0){
	      costmap->mapToWorld(k,l,x,y);
	      f = symetric_gaussian(amplitude_, x, y, pos_x, pos_y, sigma_h_);
	      
	      if(f<cutoff_)
		continue;
	      
	      unsigned char cvalue = (unsigned char) f;
	      costmap->setCost(k, l, std::max(old_cost, costmap_2d::FREE_SPACE));
	      
	    }else{
	      costmap->mapToWorld(k,l,x,y);
	      double sh = std::max(vel_module, sigma_h_);
	      double theta = tf::getYaw(person.pose.orientation);
	      theta = constrainAngle(theta*180/M_PI)*M_PI/180;

	      f = assymetric_gaussian(amplitude_, x, y, pos_x, pos_y, theta, sh, factor_s_*sh, factor_r_*sh);
	      
	      if(f<cutoff_)
		continue;
	      
	      unsigned char cvalue = (unsigned char) f;
	      costmap->setCost(k, l, std::max(old_cost, costmap_2d::FREE_SPACE));
	    }
	  }
	}
	
      }
    }else{
      for(unsigned int i = 0; i < transformed_people_.size(); i++){
	person = transformed_people_[i];
	double vel_module = sqrt(pow(person.velocity.linear.x,2) + pow(person.velocity.linear.y,2));
	double pos_x = person.pose.position.x, pos_y = person.pose.position.y;
	
	double dx, dy;
	if(people_copy.size() > 0){
	  for(unsigned int j = 0; j < people_copy.size(); j++){
	    if(people_copy[j].name.compare(person.name) == 0){
	      if(people_copy[j].status.compare("walking") == 0 && person.status.compare("standing") == 0){
		double old_vel_module = sqrt(pow(people_copy[j].velocity.linear.x, 2) + pow(people_copy[j].velocity.linear.y, 2));
		dx = safety_dist_ + old_vel_module;
		dy = safety_dist_ + old_vel_module;
	      }else{
		if(person.status.compare("sitting") == 0 || person.status.compare("standing") == 0){
		  dx = safety_dist_;
		  dy = safety_dist_;
		}else{
		  dx = safety_dist_ + vel_module;
		  dy = safety_dist_ + vel_module;
		}
	      }
	    }
	  }
	}else{
	  if(person.status.compare("sitting") == 0 || person.status.compare("standing") == 0){
	    dx = safety_dist_;
	    dy = safety_dist_;
	  }else{
	    dx = safety_dist_ + vel_module;
	    dy = safety_dist_ + vel_module;
	  }
	}
	
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
	    
	    if(person.status.compare("sitting") == 0 || person.status.compare("standing") == 0){
	      costmap->mapToWorld(k,l,x,y);
	      f = symetric_gaussian(amplitude_, x, y, pos_x, pos_y, sigma_h_);
	      
	      if(f<cutoff_)
		continue;
	      
	      unsigned char cvalue = (unsigned char) f;
	      costmap->setCost(k, l, std::max(cvalue, old_cost));
	      
	      if(action_.compare("hand over") == 0 && person_.compare(person.name) == 0){
		double dist = sqrt(pow(x - person.pose.position.x, 2) + pow(y - person.pose.position.y, 2));
		double angle = atan2(y - person.pose.position.y, x - person.pose.position.x);
		angle = constrainAngle(angle*180/M_PI)*M_PI/180;
		double theta = tf::getYaw(person.pose.orientation);
		
		if(dist > interaction_dist_ && abs(angle - theta) < interaction_angle_){
		  costmap->setCost(k, l, std::max(old_cost, costmap_2d::FREE_SPACE));
		}
	      }
	      
	    }else{
	      costmap->mapToWorld(k,l,x,y);
	      double sh = std::max(vel_module, sigma_h_);
	      double theta = tf::getYaw(person.pose.orientation);
	      theta = constrainAngle(theta*180/M_PI)*M_PI/180;

	      f = assymetric_gaussian(amplitude_, x, y, pos_x, pos_y, theta, sh, factor_s_*sh, factor_r_*sh);
	      
	      if(f<cutoff_)
		continue;
	      
	      unsigned char cvalue = (unsigned char) f;
	      costmap->setCost(k, l, std::max(cvalue, old_cost));
	    }
	  }
	}
	
      }

    }
  }
  
}