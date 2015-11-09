#include <hannrs_costmap_layers/interaction_layer.h>
#include <pluginlib/class_list_macros.h>

PLUGINLIB_EXPORT_CLASS(hannrs_costmap_layers::InteractionLayer, costmap_2d::Layer)

using costmap_2d::LETHAL_OBSTACLE;
using costmap_2d::NO_INFORMATION;

namespace hannrs_costmap_layers{
  
  InteractionLayer::InteractionLayer() {}
  
  void InteractionLayer::onInitialize(){
    ros::NodeHandle nh("~/" + name_);
    itset_sub = nh.subscribe("/hannrs/interaction_sets", 1, &InteractionLayer::itsetCallBack, this);
    current_ = true;
    
    dsrv_ = new dynamic_reconfigure::Server<InteractionLayerConfig>(nh);
    dynamic_reconfigure::Server<InteractionLayerConfig>::CallbackType cb = boost::bind(&InteractionLayer::reconfigureCB, this, _1, _2);
    dsrv_->setCallback(cb);
  }
  
  void InteractionLayer::itsetCallBack(const hannrs_msgs::InteractionSets::ConstPtr& msg){
    boost::recursive_mutex::scoped_lock lock(lock_);
    itset_list = *msg;
  }
  
  void InteractionLayer::reconfigureCB(InteractionLayerConfig &config, uint32_t level){
    enabled_ = config.enabled;
    importance_ = config.importance;
  }
  
  void InteractionLayer::updateBounds(double origin_x, double origin_y, double origin_yaw, double* min_x, double* min_y, double* max_x, double* max_y){
    if (!enabled_)
      return;
    
    std::string global_frame = layered_costmap_->getGlobalFrameID();
    
    if(itset_list.interaction_sets.empty()){
      for(unsigned int i = 0; i < itset_copy.size(); i++){
	*min_x = std::min(*min_x, itset_copy[i].center.x - itset_copy[i].radius.data);
	*min_y = std::min(*min_y, itset_copy[i].center.y - itset_copy[i].radius.data);
	*max_x = std::max(*max_x, itset_copy[i].center.x + itset_copy[i].radius.data);
	*max_y = std::max(*max_y, itset_copy[i].center.y + itset_copy[i].radius.data);
      }
    }else{
      for(int i = 0; i < (int)itset_list.interaction_sets.size(); i++){
	hannrs_msgs::InteractionSet itset = itset_list.interaction_sets[i];
	hannrs_msgs::InteractionSet itset_trans;
	geometry_msgs::PointStamped point2trans, point_trans;
	
	try{
	  point2trans.point = itset.center;
	  point2trans.header.frame_id = itset_list.header.frame_id;
	  tf_.transformPoint(global_frame, point2trans, point_trans);
	  
	  itset_trans.center = point_trans.point;
	  itset_trans.radius = itset.radius;
	  itset_trans.importance = itset.importance;
	  
	  transformed_itsets_.push_back(itset_trans);
	  
	  *min_x = std::min(*min_x, itset_trans.center.x - itset_trans.radius.data);
	  *min_y = std::min(*min_y, itset_trans.center.y - itset_trans.radius.data);
	  *max_x = std::max(*max_x, itset_trans.center.x + itset_trans.radius.data);
	  *max_y = std::max(*max_y, itset_trans.center.y + itset_trans.radius.data);
	  
	}
	catch(tf::LookupException& ex) {
	  ROS_ERROR("No Transform available Error: %s\n", ex.what());
	}
	catch(tf::ConnectivityException& ex) {
	  ROS_ERROR("Connectivity Error: %s\n", ex.what());
	}
	catch(tf::ExtrapolationException& ex) {
	  ROS_ERROR("Extrapolation Error: %s\n", ex.what());
	}
      }
    }

  }
  
  void InteractionLayer::updateCosts(costmap_2d::Costmap2D& master_grid, int min_i, int min_j, int max_i, int max_j){
    boost::recursive_mutex::scoped_lock lock(lock_);
    if (!enabled_)
      return;
    
    if(itset_list.interaction_sets.size() == 0 )
          return;
    
    hannrs_msgs::InteractionSet itset;
    costmap_2d::Costmap2D* costmap = layered_costmap_->getCostmap();
    double res = costmap->getResolution();
    
    if(itset_list.interaction_sets.empty()){
      for(unsigned int i = 0; i < itset_copy.size(); i++){
	itset = itset_copy[i];
	double center_x = itset.center.x, center_y = itset.center.y;
	double r = itset.radius.data;

  //       int dx = std::max(1, (int)(r/res));
  //       int dy = std::max(1, (int)(r/res));
	double dx = r, dy = r;
	
	unsigned int start_x, start_y, end_x, end_y;
	
	if(costmap->worldToMap(center_x - dx, center_y - dy, start_x, start_y)){
	}else{
	  start_x = 0;
	  start_y = 0;
	}
	if(costmap->worldToMap(center_x + dx, center_y + dy, end_x, end_y)){
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
	    
	    double dist = sqrt(pow(x - center_x, 2) + pow(y - center_y, 2));
	    
	    if(dist < r){
	      unsigned char cvalue = (unsigned char)(importance_*costmap_2d::LETHAL_OBSTACLE);
	      costmap->setCost(k, l, std::max(old_cost, costmap_2d::FREE_SPACE));
	    }else
	      continue;
	    
	  }
	}
      }
    }else{
      for(unsigned int i = 0; i < transformed_itsets_.size(); i++){
	itset = transformed_itsets_[i];
	double center_x = itset.center.x, center_y = itset.center.y;
	double r = itset.radius.data;

  //       int dx = std::max(1, (int)(r/res));
  //       int dy = std::max(1, (int)(r/res));
	double dx = r, dy = r;
	
	unsigned int start_x, start_y, end_x, end_y;
	
	if(costmap->worldToMap(center_x - dx, center_y - dy, start_x, start_y)){
	}else{
	  start_x = 0;
	  start_y = 0;
	}
	if(costmap->worldToMap(center_x + dx, center_y + dy, end_x, end_y)){
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
	    
	    double dist = sqrt(pow(x - center_x, 2) + pow(y - center_y, 2));
	    
	    if(dist < r){
	      unsigned char cvalue = (unsigned char)(importance_*costmap_2d::LETHAL_OBSTACLE);
	      costmap->setCost(k, l, std::max(cvalue, old_cost));
	    }else
	      continue;
	    
	  }
	}
      }
      itset_copy = transformed_itsets_;
    }
  }
  
}