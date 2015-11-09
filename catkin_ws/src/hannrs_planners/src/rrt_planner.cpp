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
#include <hannrs_planners/rrt_planner.h>
#include <pluginlib/class_list_macros.h>
#include <tf/transform_listener.h>
#include <costmap_2d/cost_values.h>
#include <costmap_2d/costmap_2d.h>

//register this planner as a  BaseGlobalPlanner plugin
PLUGINLIB_EXPORT_CLASS(hannrs_planners::RRT_Planner, nav_core::BaseGlobalPlanner)

namespace hannrs_planners{
  
  RRT_Planner::RRT_Planner() : costmap_(NULL), initialized_(false) {}
  
  RRT_Planner::RRT_Planner(std::string name, costmap_2d::Costmap2D* costmap, std::string frame_id) : costmap_(NULL), initialized_(false){
    //initialize the planner
    initialize(name, costmap, frame_id);
  }
  
  void RRT_Planner::initialize(std::string name, costmap_2d::Costmap2DROS* costmap_ros) {
    initialize(name, costmap_ros->getCostmap(), costmap_ros->getGlobalFrameID());
  }
  
  void RRT_Planner::initialize(std::string name, costmap_2d::Costmap2D* costmap, std::string frame_id) {
    if (!initialized_) {
        ros::NodeHandle private_nh("~/" + name);
	private_nh_ = private_nh;
        costmap_ = costmap;
        frame_id_ = frame_id;
	
	private_nh_.param("interpolate_path", interpolate_path_, true);
	
        plan_pub_ = private_nh_.advertise<nav_msgs::Path>("plan", 1);
	
	dsrv_ = new dynamic_reconfigure::Server<RRT_PlannerConfig>(private_nh_);
	dynamic_reconfigure::Server<RRT_PlannerConfig>::CallbackType cb = boost::bind(&RRT_Planner::reconfigureCB, this, _1, _2);
	dsrv_->setCallback(cb);

        //get the tf prefix
        ros::NodeHandle prefix_nh;
        tf_prefix_ = tf::getPrefixParam(prefix_nh);

        initialized_ = true;
    } else
        ROS_WARN("This planner has already been initialized, you can't call it twice, doing nothing");

  }
  
  void RRT_Planner::reconfigureCB(RRT_PlannerConfig &config, uint32_t level){
    max_dist_between_pathframes_ = config.max_dist_between_pathframes;
    default_tolerance_ = config.default_tolerance;
    relative_validity_check_resolution_ = config.relative_validity_check_resolution;
    validity_threshold_ = config.validity_threshold;
  }
  
  //set robot cell as free space
  void RRT_Planner::clearRobotCell(const tf::Stamped<tf::Pose>& global_pose, unsigned int mx, unsigned int my) {
    if (!initialized_) {
	ROS_ERROR("This planner has not been initialized yet, but it is being used, please call initialize() before use");
	return;
    }

    //set the associated costs in the cost map to be free
    costmap_->setCost(mx, my, costmap_2d::FREE_SPACE);
  }
  
  bool RRT_Planner::makePlan(const geometry_msgs::PoseStamped& start, const geometry_msgs::PoseStamped& goal, std::vector<geometry_msgs::PoseStamped>& plan){
    return makePlan(start, goal, default_tolerance_, plan);
  }
  
  bool RRT_Planner::makePlan(const geometry_msgs::PoseStamped& start, const geometry_msgs::PoseStamped& goal,
                           double tolerance, std::vector<geometry_msgs::PoseStamped>& plan) {
    boost::mutex::scoped_lock lock(mutex_);
    if (!initialized_) {
        ROS_ERROR("This planner has not been initialized yet, but it is being used, please call initialize() before use");
        return false;
    }

    //clear the plan, just in case
    plan.clear();

    ros::NodeHandle n;
    std::string global_frame = frame_id_;

    //until tf can handle transforming things that are way in the past... we'll require the goal to be in our global frame
    if (tf::resolve(tf_prefix_, goal.header.frame_id) != tf::resolve(tf_prefix_, global_frame)) {
        ROS_ERROR(
                "The goal pose passed to this planner must be in the %s frame.  It is instead in the %s frame.", tf::resolve(tf_prefix_, global_frame).c_str(), tf::resolve(tf_prefix_, goal.header.frame_id).c_str());
        return false;
    }

    if (tf::resolve(tf_prefix_, start.header.frame_id) != tf::resolve(tf_prefix_, global_frame)) {
        ROS_ERROR(
                "The start pose passed to this planner must be in the %s frame.  It is instead in the %s frame.", tf::resolve(tf_prefix_, global_frame).c_str(), tf::resolve(tf_prefix_, start.header.frame_id).c_str());
        return false;
    }

    double wx = start.pose.position.x;
    double wy = start.pose.position.y;

    unsigned int start_x_i, start_y_i, goal_x_i, goal_y_i;
    double start_x, start_y, goal_x, goal_y;

    if (!costmap_->worldToMap(wx, wy, start_x_i, start_y_i)) {
        ROS_WARN("The robot's start position is off the global costmap. Planning will always fail, are you sure the robot has been properly localized?");
        return false;
    }

    wx = goal.pose.position.x;
    wy = goal.pose.position.y;

    if (!costmap_->worldToMap(wx, wy, goal_x_i, goal_y_i)) {
        ROS_WARN("The goal sent to the navfn planner is off the global costmap. Planning will always fail to this goal.");
        return false;
    }
    
    //clear the starting cell within the costmap because we know it can't be an obstacle
    tf::Stamped<tf::Pose> start_pose;
    tf::poseStampedMsgToTF(start, start_pose);
    clearRobotCell(start_pose, start_x_i, start_y_i);

    //We first create an instance of the state space we are planning in.
    ob::StateSpacePtr space(new ob::SE2StateSpace());
    //We then set the bounds for the R² component of this state space:
    ob::RealVectorBounds bounds(2);
    double map_upperbound, map_lowerbound;
    
    // get bounds for x coordinate
    map_upperbound = costmap_->getSizeInMetersX() + costmap_->getOriginX();
    map_lowerbound = costmap_->getOriginX();
    bounds.setHigh(0, map_upperbound);
    bounds.setLow(0, map_lowerbound);
    ROS_DEBUG("Setting uper bound and lower bound of map x-coordinate to (%f, %f).", map_upperbound, map_lowerbound);

    // get bounds for y coordinate
    map_upperbound = costmap_->getSizeInMetersY() + costmap_->getOriginY();
    map_lowerbound = costmap_->getOriginY();
    bounds.setHigh(1, map_upperbound);
    bounds.setLow(1, map_lowerbound);
    ROS_DEBUG("Setting uper bound and lower bound of map y-coordinate to (%f, %f).", map_upperbound, map_lowerbound);
    
    // now send it to the planer
    space->as<ob::SE2StateSpace>()->setBounds(bounds);
    
    // now create instance to ompl setup
    og::SimpleSetup simple_setup(space);
    
    // set state validity checker
    simple_setup.setStateValidityChecker(boost::bind(&RRT_Planner::isStateValid2DGrid, this, _1));
    // this call deviates a little bit from the example, as we use the member function of _this_ instance (initialized with the correct map, ...)
    // boost::bind works the following way
    // boost::bind(Adress of Function to pass,
    //			   Adress of instance to which the function shall be associated, - only for member functions -
    //			   list of parameters to pass to the function)
    // a nice introduction is found on http://blog.orionedwards.com/2006/09/function-pointers-in-cc-and-boostbind.html

    // get SpaceInformationPointer from simple_setup (initialized in makePlan routine)
    ob::SpaceInformationPtr si_ptr = simple_setup.getSpaceInformation();
    
    // set validity checking resolution
    si_ptr->setStateValidityCheckingResolution(relative_validity_check_resolution_);
    
    // convert start and goal pose from ROS PoseStamped to ompl ScopedState for SE2
    // convert PoseStamped into Pose2D
    geometry_msgs::Pose2D start2D, goal2D;
    PoseToPose2D(start.pose, start2D);
    PoseToPose2D(goal.pose, goal2D);
    
    // create a Scoped State according to above specified Space (SE2)
    ob::ScopedState<> ompl_scoped_state_start(space);

    // and set this state to the start pose
    ROSPose2DToRRTScopedStateSE2(ompl_scoped_state_start, start2D);

    // check whether this satisfies the bounds of the space
    bool inBound = space->satisfiesBounds(ompl_scoped_state_start->as<ob::SE2StateSpace::StateType>());
    if(!inBound){
	    ROS_ERROR("Start Pose lies outside the bounds of the map - Aborting Planer");
	    return false;
    }

    // create a Scoped State according to above specified Space (SE2)
    ob::ScopedState<> ompl_scoped_state_goal(space);

    // and set this state to goal pose
    ROSPose2DToRRTScopedStateSE2(ompl_scoped_state_goal, goal2D);

    // check whether this satisfies the bounds of the space
    inBound = space->satisfiesBounds(ompl_scoped_state_goal->as<ob::SE2StateSpace::StateType>());
    if(!inBound){
	    ROS_ERROR("Target Pose lies outside the bounds of the map - Aborting Planer");
	    return false;
    }
    
    // set start and goal state to planner
    simple_setup.setStartAndGoalStates(ompl_scoped_state_start, ompl_scoped_state_goal);
    
    // read desired planner-type from parameter server and set according planner to SimpleSetup
    setPlannerType(simple_setup);
    
    // finally --> plan a path (give ompl 1 second to find a valid path)
    ROS_DEBUG("Requesting Plan");
    bool solved = simple_setup.solve(1.0);
    
    if(!solved)	{
      ROS_WARN("No path found");	
    }
    
    // give ompl a chance to simplify the found solution
    simple_setup.simplifySolution();
    
    // if path found -> get resulting path
    og::PathGeometric rrt_path(simple_setup.getSolutionPath());
    
    // convert into vector of pose2D
    ROS_DEBUG("Converting Path from ompl PathGeometric format to vector of PoseStamped");
    std::vector<geometry_msgs::Pose2D> temp_plan_Pose2D;
    geometry_msgs::Pose2D temp_pose;
    int num_frames_inpath = rrt_path.getStateCount();
    
//     ROS_INFO("Num frames in solution path: %d", num_frames_inpath);
    
    for(int i = 0; i < num_frames_inpath; i++)    {
      // get frame and tranform it to Pose2D
      RRTStateSE2ToROSPose2D(rrt_path.getState(i), temp_pose);

      // output states for Debug
      ROS_DEBUG("Coordinates of %dth frame: (x, y, theta) = (%f, %f, %f).", i, temp_pose.x, temp_pose.y, temp_pose.theta);

      // and append them to plan
      temp_plan_Pose2D.push_back(temp_pose);
    }
    
    ROS_DEBUG("Interpolating path...");
    if(interpolate_path_)    {
      ROS_DEBUG("Interpolating path to increase density of frames for local planning");
      // interpolate between frames to meet density requirement of local_planner
      bool ipo_success = interpolatePathPose2D(temp_plan_Pose2D);
      if(!ipo_success){
	ROS_ERROR("Something went wrong during interpolation. Probably plan empty. Aborting!");
	return false;
      }
      num_frames_inpath = (int) temp_plan_Pose2D.size();
      ROS_DEBUG("Interpolated Path has %d frames", num_frames_inpath);
    }
    ROS_DEBUG("Path interpolation done");
    
    // convert into vector of PoseStamped
    std::vector<geometry_msgs::PoseStamped> temp_plan;
    geometry_msgs::PoseStamped temp_pose_stamped;

    for(int i = 0; i < num_frames_inpath; i++){
      // set Frame to PoseStamped
      ros::Time plan_time = ros::Time::now();

      // set header
      temp_pose_stamped.header.stamp = plan_time;
      temp_pose_stamped.header.frame_id = frame_id_;

      // convert Pose2D to pose and set to Pose of PoseStamped
      Pose2DToPose(temp_pose_stamped.pose, temp_plan_Pose2D[i]);

      // append to plan
      temp_plan.push_back(temp_pose_stamped);
    }
    // done -> pass temp_plan to referenced variable plan ...
//     ROS_INFO("Global planning finished: Path Found.");
    plan = temp_plan;
    
//     ROS_INFO("Number of points in final plan: %d", (int)plan.size());
    // publish the plan for visualization purposes ...
    publishPlan(plan);
    
    // and return with true
    return true;
    

  }
  
  void RRT_Planner::setPlannerType(og::SimpleSetup& simple_setup){
    // set default planner
    std::string default_planner("RRT");

    // read desired planner from parameter server
    private_nh_.param("global_planner_type", planner_type_, std::string("RRT"));

    // get SpaceInformationPointer from simple_setup (initialized in makePlan routine)
    ob::SpaceInformationPtr si_ptr = simple_setup.getSpaceInformation();

    // init according planner --> this is a little bit arkward, but as there is no switch/case for strings ...
    if(planner_type_.compare("LazyRRT") == 0)
    {
	    ompl::base::PlannerPtr taregt_planner_ptr(new og::LazyRRT(si_ptr));
	    simple_setup.setPlanner(taregt_planner_ptr);
	    return;
    }

    if(planner_type_.compare("pRRT") == 0)
    {
	    ompl::base::PlannerPtr taregt_planner_ptr(new ompl::geometric::pRRT(si_ptr));
	    simple_setup.setPlanner(taregt_planner_ptr);
	    return;
    }

    if(planner_type_.compare("RRT") == 0)
    {
	    ompl::base::PlannerPtr taregt_planner_ptr(new ompl::geometric::RRT(si_ptr));
	    simple_setup.setPlanner(taregt_planner_ptr);
	    return;
    }

    if(planner_type_.compare("RRTConnect") == 0)
    {
	    ompl::base::PlannerPtr taregt_planner_ptr(new ompl::geometric::RRTConnect(si_ptr));
	    simple_setup.setPlanner(taregt_planner_ptr);
	    return;
    }
    
    if(planner_type_.compare("RRTstar") == 0)
    {
	    ompl::base::PlannerPtr taregt_planner_ptr(new ompl::geometric::RRTstar(si_ptr));
	    simple_setup.setPlanner(taregt_planner_ptr);
	    return;
    }
    
    if(planner_type_.compare("TRRT") == 0)
    {
	    ompl::base::PlannerPtr taregt_planner_ptr(new ompl::geometric::TRRT(si_ptr));
	    simple_setup.setPlanner(taregt_planner_ptr);
	    return;
    }
    
    // if no string fitted in --> do nothing: by default SimpleSetup will use RRT
    return;
  }
  
  bool RRT_Planner::isStateValid2DGrid(const ob::State *state){
    geometry_msgs::Pose2D checked_state;
    
    RRTStateSE2ToROSPose2D(state, checked_state);
    unsigned int mx, my;
    
    costmap_->worldToMap(checked_state.x,checked_state.y, mx,my);
    unsigned char cost = costmap_->getCost(mx,my);
    
    if(cost < validity_threshold_){
      return true;
    }
    
    return false;
  }
  
  bool RRT_Planner::interpolatePathPose2D(std::vector<geometry_msgs::Pose2D>& path){
    std::vector<geometry_msgs::Pose2D> ipoPath;
    geometry_msgs::Pose2D last_frame, curr_frame, diff_frame, temp_frame;
    double frame_distance, num_insertions;
    int path_size = path.size();
    double path_lenght = 0;

    // check whether planner is already initialized
    if(!initialized_){
      ROS_ERROR("The planner has not been initialized, please call initialize() to use the planner");
      return false;
    }

    // check whether path is correct - at least 2 Elements
    if(path_size < 2){
      ROS_ERROR("Path is not valid. It has only %d Elements. Interpolation not possible. Aborting.", path_size);
      return false;
    }

    // init plan with start frame
    ipoPath.push_back(path[0]);

    // make sure plan is dense enough to be processed by local planner
    for(int i = 1; i < path_size; i++){
      // check wether current frame is close enough to last frame --> otherwise insert interpolated frames
      last_frame = ipoPath[(ipoPath.size()-1)];
      curr_frame = path[i];

      // calc distance between frames
      diff_frame.x = curr_frame.x - last_frame.x;
      diff_frame.y = curr_frame.y - last_frame.y;
      diff_frame.theta = curr_frame.theta - last_frame.theta;
      // normalize angle
      diff_frame.theta = angles::normalize_angle(diff_frame.theta);
      // calulate distance --> following is kind of a heuristic measure, ...
      // ... as it only takes into account the euclidean distance in the cartesian coordinates
      frame_distance = sqrt( diff_frame.x*diff_frame.x + diff_frame.y*diff_frame.y );

      // insert frames until path is dense enough
      if(frame_distance > max_dist_between_pathframes_){
	// just in case --> insert one frame more than neccesarry
	num_insertions = ceil(frame_distance/max_dist_between_pathframes_);
	//ROS_DEBUG("Distance between frames too large (%fm): Inserting %f frames.", frame_distance, num_insertions);
	// n insertions create n+1 intervalls --> add one to division
	diff_frame.x = diff_frame.x/(num_insertions + 1.0);
	diff_frame.y = diff_frame.y/(num_insertions + 1.0);
	diff_frame.theta = diff_frame.theta/(num_insertions + 1.0);
	for(int j = 1; j <= (int)num_insertions; j++){
	  temp_frame.x = last_frame.x + j*diff_frame.x;
	  temp_frame.y = last_frame.y + j*diff_frame.y;
	  temp_frame.theta = last_frame.theta + j*diff_frame.theta;
	  // normalize angle
	  temp_frame.theta = angles::normalize_angle(temp_frame.theta);

	  // append frame to interpolated path
	  ipoPath.push_back(temp_frame);
	}
      }

      // finally insert frame from path
      ipoPath.push_back(curr_frame);
    }

    // done --> copy ipoPath to refernce-variable and return with true
    path = ipoPath;
    
    for(unsigned int i = 1; i < path.size(); i++){
      path_lenght += sqrt(pow(path[i].x - path[i-1].x, 2) + pow(path[i].y - path[i-1].y, 2));
    }
    ROS_INFO("Path length: %f", path_lenght);
    
    return true;
  }
  
  void RRT_Planner::publishPlan(std::vector<geometry_msgs::PoseStamped> path){
    // check whether planner is already initialized --> should be the case anyway but better be sure
    if(!initialized_){
      ROS_ERROR("This planner has not been initialized yet, but it is being used, please call initialize() before use");
      return;
    }

    // check whether there really is a path --> given an empty path we won't do anything
    if(path.empty()){
      ROS_INFO("Plan is empty - Nothing to display");
      return;
    }

    // create a message for the plan 
    nav_msgs::Path gui_path;
    gui_path.poses.resize(path.size());
    gui_path.header.frame_id = path[0].header.frame_id;
    gui_path.header.stamp = path[0].header.stamp;

    // Extract the plan in world co-ordinates, we assume the path is all in the same frame
    for(unsigned int i=0; i < path.size(); i++){
      gui_path.poses[i] = path[i];
    }

    plan_pub_.publish(gui_path);
  }
  
  // type conversions
  
  void RRT_Planner::RRTStateSE2ToROSPose2D(const ob::State* ompl_state, geometry_msgs::Pose2D& pose2D){
    // get frame and tranform it to Pose2D
    // access element "->"
    // and cast to SE2 StateType "as<>()" with type "ompl::base::SE2StateManifold::StateType"
    // access member function of actual state "->"
    pose2D.x = ompl_state->as<ob::SE2StateSpace::StateType>()->getX();
    pose2D.y = ompl_state->as<ob::SE2StateSpace::StateType>()->getY();
    pose2D.theta = ompl_state->as<ob::SE2StateSpace::StateType>()->getYaw();

    // normalize angle - just in case
    pose2D.theta = angles::normalize_angle(pose2D.theta);

    return;
  }
  
  void RRT_Planner::PoseToPose2D(const geometry_msgs::Pose pose, geometry_msgs::Pose2D& pose2D){
	// use tf-pkg to convert angles
    tf::Pose pose_tf;

    // convert geometry_msgs::PoseStamped to tf::Pose
    tf::poseMsgToTF(pose, pose_tf);

    // now get Euler-Angles from pose_tf
    double useless_pitch, useless_roll, yaw;
    pose_tf.getBasis().getEulerYPR(yaw, useless_pitch, useless_roll);

    // normalize angle
    yaw = angles::normalize_angle(yaw);

    // and set to pose2D
    pose2D.x = pose.position.x;
    pose2D.y = pose.position.y;
    pose2D.theta = yaw;

    return;
  }
  
  void RRT_Planner::ROSPose2DToRRTScopedStateSE2(ob::ScopedState<>& scoped_state,const geometry_msgs::Pose2D pose2D){
    // get frame and tranform it to Pose2D
    // access element "->"
    // and cast to SE2 StateType "as<>()" with type "ompl::base::SE2StateManifold::StateType"
    // access member function of actual state "->"
    scoped_state->as<ompl::base::SE2StateSpace::StateType>()->setX(pose2D.x);
    scoped_state->as<ompl::base::SE2StateSpace::StateType>()->setY(pose2D.y);
    scoped_state->as<ompl::base::SE2StateSpace::StateType>()->setYaw(pose2D.theta);

    return;
  }
  
  void RRT_Planner::Pose2DToPose(geometry_msgs::Pose& pose, const geometry_msgs::Pose2D pose2D){
    // use tf-pkg to convert angles
    tf::Quaternion frame_quat;

    // transform angle from euler-angle to quaternion representation
    frame_quat = tf::createQuaternionFromYaw(pose2D.theta);

    // set position
    pose.position.x = pose2D.x;
    pose.position.y = pose2D.y;
    pose.position.z = 0.0;

    // set quaternion
    pose.orientation.x = frame_quat.x();
    pose.orientation.y = frame_quat.y();
    pose.orientation.z = frame_quat.z();
    pose.orientation.w = frame_quat.w();

    return;
  }

}