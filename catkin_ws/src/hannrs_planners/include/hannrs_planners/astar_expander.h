#ifndef _ASTAR_EXPANDER_H
#define _ASTAR_EXPANDER_H

#include <ros/ros.h>
#include <costmap_2d/costmap_2d.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Point.h>
#include <nav_msgs/Path.h>
#include <tf/transform_datatypes.h>
#include <vector>
#include <algorithm>
#include <cmath>

#define POT_HIGH 1.0e10 
namespace hannrs_planners{
  class Node{
  public:
    Node(unsigned int i, double c){
      index_ = i;
      cost_ = c;
    }
    
    friend bool operator==(const Node& x, const Node& y){ 
      return x.index_ == y.index_; 
    }

    unsigned int index_;
    double cost_;
  };
  
  class Astar_Expander{
  public:
    Astar_Expander(int nx, int ny);
    bool calculatePotentials(unsigned char* costs, double start_x, double start_y, double end_x, double end_y, double* potential);
    bool getPath(double* potential, double start_x, double start_y, double end_x, double end_y, std::vector<std::pair<double, double> >& path);
  private:
    inline int toIndex(int x, int y) {
            return x + nx_ * y;
    }
    
    std::vector<Node> closedset;
    std::vector<Node> openset;
    std::vector<int> came_from;
    
    std::vector<double> g_score;
    
    int nx_, ny_;
    
    double distBetweenNodes(unsigned int i, unsigned int j);

  };
}

#endif