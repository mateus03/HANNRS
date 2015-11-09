#ifndef _INTERACTION_LAYER_H
#define _INTERACTION_LAYER_H

#include <ros/ros.h>
#include <costmap_2d/layer.h>
#include <costmap_2d/layered_costmap.h>
#include <dynamic_reconfigure/server.h>
#include <cmath>
#include <hannrs_msgs/InteractionSet.h>
#include <hannrs_msgs/InteractionSets.h>
#include <hannrs_costmap_layers/proxemics_layer.h>
#include <hannrs_costmap_layers/InteractionLayerConfig.h>

namespace hannrs_costmap_layers{
  
  class InteractionLayer : public costmap_2d::Layer{
  public:
    InteractionLayer();

    virtual void onInitialize();
    virtual void updateBounds(double origin_x, double origin_y, double origin_yaw, double* min_x, double* min_y, double* max_x, double* max_y);
    virtual void updateCosts(costmap_2d::Costmap2D& master_grid, int min_i, int min_j, int max_i, int max_j);
    
    bool isDiscretized(){
      return false;
    }

  protected:
    double radius_, importance_;
    ros::Subscriber itset_sub;
    void itsetCallBack(const hannrs_msgs::InteractionSets::ConstPtr& msg);
    void reconfigureCB(InteractionLayerConfig &config, uint32_t level);
    dynamic_reconfigure::Server<InteractionLayerConfig> *dsrv_;
    boost::recursive_mutex lock_;
    hannrs_msgs::InteractionSets itset_list;
    std::vector<hannrs_msgs::InteractionSet> itset_copy;
    std::vector<hannrs_msgs::InteractionSet> transformed_itsets_;
    tf::TransformListener tf_;
  };
}

#endif