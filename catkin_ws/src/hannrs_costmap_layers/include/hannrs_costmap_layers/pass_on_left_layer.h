#ifndef _PASS_ON_LEFT_LAYER_H
#define _PASS_ON_LEFT_LAYER_H

#include <costmap_2d/layer.h>
#include <costmap_2d/layered_costmap.h>
#include <costmap_2d/GenericPluginConfig.h>
#include <dynamic_reconfigure/server.h>
#include <cmath>
#include <hannrs_msgs/People.h>
#include <hannrs_costmap_layers/proxemics_layer.h>
#include <hannrs_costmap_layers/Pass_On_LeftLayerConfig.h>

namespace hannrs_costmap_layers{
  
  class Pass_On_LeftLayer : public costmap_2d::Layer{
  public:
    Pass_On_LeftLayer();

    virtual void onInitialize();
    virtual void updateBounds(double origin_x, double origin_y, double origin_yaw, double* min_x, double* min_y, double* max_x, double* max_y);
    virtual void updateCosts(costmap_2d::Costmap2D& master_grid, int min_i, int min_j, int max_i, int max_j);
    
    bool isDiscretized(){
      return false;
    }
    
  protected:
    double amplitude_, sigma_, sigma_h_, factor_r_, factor_s_, safety_dist_, cutoff_;
    ros::Subscriber people_sub;
    void peopleCallBack(const hannrs_msgs::People::ConstPtr& msg);
    void reconfigureCB(Pass_On_LeftLayerConfig &config, uint32_t level);
    dynamic_reconfigure::Server<Pass_On_LeftLayerConfig> *dsrv_;
    boost::recursive_mutex lock_;
    hannrs_msgs::People people_list;
    std::vector<hannrs_msgs::Person> transformed_people_;
    tf::TransformListener tf_;
  };
  
}

#endif