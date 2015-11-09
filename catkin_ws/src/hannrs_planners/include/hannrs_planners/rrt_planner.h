/*********************************************************************
*
* Software License Agreement (BSD License)
*
*  Copyright (c) 2010, Willow Garage, Inc.
*  All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*   * Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above
*     copyright notice, this list of conditions and the following
*     disclaimer in the documentation and/or other materials provided
*     with the distribution.
*   * Neither the name of the Willow Garage nor the names of its
*     contributors may be used to endorse or promote products derived
*     from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
*  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
*  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
*  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*  POSSIBILITY OF SUCH DAMAGE.
*
* Author: Christian Connette, Eitan Marder-Eppstein
*********************************************************************/

#ifndef _RRT_PLANNER_H
#define _RRT_PLANNER_H

#include <ros/ros.h>
#include <costmap_2d/costmap_2d.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Point.h>
#include <geometry_msgs/Pose2D.h>
#include <nav_msgs/Path.h>
#include <tf/transform_datatypes.h>
#include <vector>
#include <nav_core/base_global_planner.h>
#include <nav_msgs/GetPlan.h>

#include <base_local_planner/world_model.h>
#include <base_local_planner/costmap_model.h>

#include <tf/tf.h>
#include <tf/transform_datatypes.h>
#include <angles/angles.h>

// std c++ classes
#include <math.h>
#include <string.h>

// boost classes
#include <boost/bind.hpp>

// ompl planner specific classes
#include <ompl/base/State.h>
#include <ompl/base/ScopedState.h>
#include <ompl/base/StateSpace.h>
#include <ompl/base/spaces/SE2StateSpace.h>
#include <ompl/base/spaces/SO2StateSpace.h>
#include <ompl/base/spaces/RealVectorBounds.h>
#include <ompl/geometric/SimpleSetup.h>
#include <ompl/base/Path.h>
#include <ompl/geometric/PathGeometric.h>
// ompl planners
#include <ompl/geometric/planners/rrt/LazyRRT.h>
#include <ompl/geometric/planners/rrt/pRRT.h>
#include <ompl/geometric/planners/rrt/RRT.h>
#include <ompl/geometric/planners/rrt/RRTConnect.h>
#include <ompl/geometric/planners/rrt/RRTstar.h>
#include <ompl/geometric/planners/rrt/TRRT.h>

#include <hannrs_planners/RRT_PlannerConfig.h>


namespace ob = ompl::base;
namespace og = ompl::geometric;

namespace hannrs_planners{

  class RRT_Planner : public nav_core::BaseGlobalPlanner{
  public:
    /**
      * @brief  Default constructor for the PlannerCore object
      */
    RRT_Planner();
    
    /**
      * @brief  Constructor for the PlannerCore object
      * @param  name The name of this planner
      * @param  costmap A pointer to the costmap to use
      * @param  frame_id Frame of the costmap
      */
    RRT_Planner(std::string name, costmap_2d::Costmap2D* costmap, std::string frame_id);
    
    /**
      * @brief  Initialization function for the PlannerCore object
      * @param  name The name of this planner
      * @param  costmap_ros A pointer to the ROS wrapper of the costmap to use for planning
      */
    void initialize(std::string name, costmap_2d::Costmap2DROS* costmap_ros);

    void initialize(std::string name, costmap_2d::Costmap2D* costmap, std::string frame_id);
    
    /**
      * @brief Given a goal pose in the world, compute a plan
      * @param start The start pose
      * @param goal The goal pose
      * @param plan The plan... filled by the planner
      * @return True if a valid plan was found, false otherwise
      */
    bool makePlan(const geometry_msgs::PoseStamped& start, const geometry_msgs::PoseStamped& goal,
		  std::vector<geometry_msgs::PoseStamped>& plan);

    /**
      * @brief Given a goal pose in the world, compute a plan
      * @param start The start pose
      * @param goal The goal pose
      * @param tolerance The tolerance on the goal point for the planner
      * @param plan The plan... filled by the planner
      * @return True if a valid plan was found, false otherwise
      */
    bool makePlan(const geometry_msgs::PoseStamped& start, const geometry_msgs::PoseStamped& goal, double tolerance,
		  std::vector<geometry_msgs::PoseStamped>& plan);
    
    
  protected:
    ros::NodeHandle private_nh_;
    costmap_2d::Costmap2D* costmap_;
    costmap_2d::Costmap2DROS* costmap_ros_;
    std::string frame_id_;
    ros::Publisher plan_pub_;
    bool initialized_;
    
  private:
    bool interpolate_path_;
    double default_tolerance_;
    double relative_validity_check_resolution_;
    double max_dist_between_pathframes_;
    double validity_threshold_;
    std::string tf_prefix_;
    boost::mutex mutex_;
    std::string planner_type_; ///<@brief parameter to switch between different planners provided through ompl
    
    void clearRobotCell(const tf::Stamped<tf::Pose>& global_pose, unsigned int mx, unsigned int my);
    bool isStateValid2DGrid(const ob::State *state);
    void RRTStateSE2ToROSPose2D(const ob::State* ompl_state, geometry_msgs::Pose2D& pose2D);
    void PoseToPose2D(const geometry_msgs::Pose pose, geometry_msgs::Pose2D& pose2D);
    void ROSPose2DToRRTScopedStateSE2(ob::ScopedState<>& scoped_state, const geometry_msgs::Pose2D pose2D);
    void setPlannerType(ompl::geometric::SimpleSetup& simple_setup);
    bool interpolatePathPose2D(std::vector<geometry_msgs::Pose2D>& path);
    void Pose2DToPose(geometry_msgs::Pose& pose, const geometry_msgs::Pose2D pose2D);
    void publishPlan(std::vector<geometry_msgs::PoseStamped> path);
    
    dynamic_reconfigure::Server<RRT_PlannerConfig> *dsrv_;
    void reconfigureCB(RRT_PlannerConfig &config, uint32_t level);
  };
}

#endif