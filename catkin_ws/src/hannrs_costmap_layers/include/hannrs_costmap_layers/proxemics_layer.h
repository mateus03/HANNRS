#ifndef _PROXEMICS_LAYER_H
#define _PROXEMICS_LAYER_H

#include <ros/ros.h>
#include <costmap_2d/layer.h>
#include <costmap_2d/layered_costmap.h>
#include <dynamic_reconfigure/server.h>
#include <cmath>
#include <hannrs_msgs/People.h>
#include <hannrs_costmap_layers/ProxemicsLayerConfig.h>
#include <hannrs_msgs/Prox_Goal.h>

double symetric_gaussian(double A, double x, double y, double xc, double yc, double sigma);
double assymetric_gaussian(double A, double x, double y, double xc, double yc, double theta, double sigma_h, double sigma_s, double sigma_r);
double constrainAngle(double x);

namespace hannrs_costmap_layers{
  
  class ProxemicsLayer : public costmap_2d::Layer{
  public:
    ProxemicsLayer();

    virtual void onInitialize();
    virtual void updateBounds(double origin_x, double origin_y, double origin_yaw, double* min_x, double* min_y, double* max_x, double* max_y);
    virtual void updateCosts(costmap_2d::Costmap2D& master_grid, int min_i, int min_j, int max_i, int max_j);
    
    bool isDiscretized(){
      return false;
    }

  protected:
    double amplitude_, sigma_, sigma_h_, factor_r_, factor_s_, safety_dist_, cutoff_, interaction_dist_, interaction_angle_;
    ros::Subscriber people_sub;
    ros::ServiceServer service;
    void peopleCallBack(const hannrs_msgs::People::ConstPtr& msg);
    bool prox_goalCallBack(hannrs_msgs::Prox_Goal::Request& req, hannrs_msgs::Prox_Goal::Response& res);
    void reconfigureCB(ProxemicsLayerConfig &config, uint32_t level);
    dynamic_reconfigure::Server<ProxemicsLayerConfig> *dsrv_;
    boost::recursive_mutex lock_;
    hannrs_msgs::People people_list;
    std::vector<hannrs_msgs::Person> transformed_people_;
    std::vector<hannrs_msgs::Person> people_copy;
    tf::TransformListener tf_;
    std::string person_, action_;
    
  };
}

#endif