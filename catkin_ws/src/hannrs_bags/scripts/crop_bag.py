#! /usr/bin/env python

import rospy
import rosbag
import math

with rosbag.Bag('/home/mateus/catkin_ws/src/hanp_bags/bags/final_exp4_crop.bag', 'w') as outbag:
    for topic, msg, t in rosbag.Bag('/home/mateus/catkin_ws/src/hanp_bags/bags/final_exp4.bag').read_messages():
        if t.to_sec() > 1.41323700838*math.pow(10,9) and t.to_sec() < 1.41323705638*math.pow(10,9):
            outbag.write(topic, msg, t)