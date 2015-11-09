#include "hannrs_vision/people_detector.h"
//contructor
PeopleDetector::PeopleDetector():it(nh){
  cv::namedWindow("undistorted image", cv::WINDOW_NORMAL);
//   cv::namedWindow("MOG2", cv::WINDOW_NORMAL);
  
  std::string stream, borders, classifier, data_file, transform;
    
  ros::NodeHandle pnh("~");
  pnh.getParam("image", stream);
  pnh.getParam("borders_file", borders);
  pnh.getParam("classifier", classifier);
  pnh.getParam("camera", cam);
  pnh.getParam("data", data_file);
  pnh.getParam("transform", transform);
  
  //subscribe video stream
  image_sub = it.subscribe(stream, 1, &PeopleDetector::imageCB, this, image_transport::TransportHints("compressed"));
  
  undistorter = new Undistorter();
  
  cv::FileStorage borders_file = cv::FileStorage(borders, cv::FileStorage::READ);
  borders_file["surfaces"] >> surf;
  borders_file["sitting"] >> sitting;
  borders_file.release();
  
  cv::FileStorage transform_file = cv::FileStorage(transform, cv::FileStorage::READ);
  transform_file["R"] >> R;
  transform_file["T"] >> T;
  transform_file.release();
  
  fst_im = true;
  
  //create Background Subtractor objects
  pMOG2 = new cv::BackgroundSubtractorMOG2(); //MOG2 approach
  
  SVM.load(classifier.c_str());
  
  detections_pub = nh.advertise<hannrs_msgs::VisionDetection>("hannrs/visiondetections", 1);
  visual_pub = nh.advertise<geometry_msgs::PointStamped>("/detections", 1);
  
  pos_data.open(data_file.c_str());
  
  clicks = 0;
}

//destructor
PeopleDetector::~PeopleDetector(){
  cv::destroyAllWindows();
  pos_data.close();
}

//image subscriber callback
void PeopleDetector::imageCB(const sensor_msgs::ImageConstPtr& msg){
  try{
    cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
  }catch(cv_bridge::Exception& e){
    ROS_ERROR("cv_bridge exception: %s", e.what());
    return;
  }
  
  if(fst_im){
    fst_im = false;
  }
  
}

//spinner
void PeopleDetector::spinOnce(){
    ros::spinOnce();
}

//display image
void PeopleDetector::showImage(){
  if(!fst_im){
    cv::imshow("undistorted image", undistorted_im);
//     cv::imshow("MOG2", fgMaskMOG2);
    cv::waitKey(3);
  }
}

//main method - detection
void PeopleDetector::detect(){
  if(!fst_im){
//     ros::Time t = ros::Time::now();
    
    //remove distortion
    undistorted_im = undistorter->undistortIm(cv_ptr->image);
        
    
    //update the background model
    pMOG2->operator()(undistorted_im, fgMaskMOG2);
    std::vector<Entity> entities = find_contours();
    std::vector<Entity> entitiesInborders = filter_detections(entities);
    std::vector<Entity> entitiesClassified = classify_detections(entitiesInborders);

    //remove robot ellipses
    people.clear();
//     people = entitiesClassified;
    for(unsigned int i = 0; i < entitiesClassified.size(); i++){
      if(entitiesClassified[i].classification == 3.0){
      }else{
	people.push_back(entitiesClassified[i]);
      }
    }
    
    //compute people ellipse's distance to image center and angle
    for(unsigned int i = 0; i < people.size(); i++){
      cv::Point2f p = people[i].ellipse.center;
      cv::Size2f s = people[i].ellipse.size;
      cv::Point2f im_center = undistorter->image_center();
      cv::Size im_size = undistorter->imageSize();
      
      if(people[i].classification == 1.0){
	people[i].dist2center = 10000.0;
	unsigned int ind;
	for(unsigned int j = 0; j < people[i].contours.size(); j++){
	  double dist = sqrt((people[i].contours[j].x - im_center.x)*(people[i].contours[j].x - im_center.x) + ((im_size.height - people[i].contours[j].y) - im_center.y)*((im_size.height - people[i].contours[j].y) - im_center.y));
	  if(people[i].dist2center > dist){
	    people[i].dist2center = dist;
	    people[i].feet = people[i].contours[j];
	  }
	}
	
	people[i].angle = atan2((im_size.height - people[i].feet.y) - im_center.y, people[i].feet.x - im_center.x);
	cv::circle(undistorted_im, people[i].feet, 10, cv::Scalar(0,255,255), 5, 8);
      }
    }
    
    //compute world pos in camera frame
    for(unsigned int i = 0; i < people.size(); i++){
      if(people[i].classification == 1.0){
	people[i].cam_pos.x = pixel2meters(people[i].dist2center*cos(people[i].angle));
	people[i].cam_pos.y = pixel2meters(people[i].dist2center*sin(people[i].angle));
// 	pos_data << people[i].cam_pos.x << " " << people[i].cam_pos.y << std::endl;
	pos_data << people[i].dist2center << " " << pixel2meters(people[i].dist2center) << std::endl;
      }
    }
    
    std::vector<unsigned int> inds;
    //compute world pos in map frame
    for(unsigned int i = 0; i < people.size(); i++){
      if(people[i].classification == 1.0){
	people[i].world_pos.x = R.at<double>(0,0)*people[i].cam_pos.x + R.at<double>(0,1)*people[i].cam_pos.y + T.at<double>(0);
	people[i].world_pos.y = R.at<double>(1,0)*people[i].cam_pos.x + R.at<double>(1,1)*people[i].cam_pos.y + T.at<double>(1);
// 	pos_data << people[i].world_pos.x << " " << people[i].world_pos.y << std::endl;
      }else{
	if(!sitting.empty()){
	  cv::Point2f p = people[i].ellipse.center;
	  cv::Size2f s = people[i].ellipse.size;
	  cv::Point2f im_center = undistorter->image_center();
	  cv::Size im_size = undistorter->imageSize();
	  
	  if(sitting[0].contains(people[i].ellipse.center)){
	    people[i].world_pos.x = 7.147;
	    people[i].world_pos.y = 1.823;
	    people[i].world_pos.z = -1.670;
	  }else if(sitting[1].contains(people[i].ellipse.center)){
	    people[i].world_pos.x = 5.579;
	    people[i].world_pos.y = 1.362;
	    people[i].world_pos.z = -1.154;
	  }else if(sitting[2].contains(people[i].ellipse.center)){
	    people[i].world_pos.x = 4.430;
	    people[i].world_pos.y = 0.525;
	    people[i].world_pos.z = -0.618;
	  }else{
	    inds.push_back(i);
// 	    people.erase(people.begin()+i);
	  }
	}else{
	  inds.push_back(i);
// 	  people.erase(people.begin()+i);
	}
      }
    }
    
    for(unsigned int l = 0; l < inds.size(); l++){
      people.erase(people.begin()+inds[l]);
    }
    
    //publish topic
    hannrs_msgs::VisionDetection msg;
    msg.header.stamp = ros::Time::now();
    msg.header.frame_id = "/map";
    msg.camera = cam;
    
    for(unsigned int i = 0; i < people.size(); i++){
      msg.positions.push_back(people[i].world_pos);
      msg.classifications.push_back(people[i].classification);
    }
    detections_pub.publish(msg);
    
    geometry_msgs::PointStamped det_msg;
    det_msg.header.stamp = ros::Time::now();
    det_msg.header.frame_id = "/map";
    if(people.size() == 1)
      det_msg.point = people[0].world_pos;
    visual_pub.publish(det_msg);
    
    plotEntities(people);
    
//     double elapsed = ros::Time::now().toSec() - t.toSec();
//     ROS_INFO("Elapsed: %f", elapsed);
  }
}

//retrieve the contours and fit ellipses on them
std::vector<Entity> PeopleDetector::find_contours(){
  if(!fst_im){
    std::vector<std::vector<cv::Point> > contoursMOG2;
    std::vector<cv::Vec4i> hierarchyMOG2;
    std::vector<std::vector<cv::Point> > contours_polyMOG2;
    
    std::vector<Entity> entities;
    
    cv::Mat thresholdMOG2;

    cv::threshold(fgMaskMOG2, fgMaskMOG2, 254, 255, cv::THRESH_BINARY);
    
    //filter foreground masks    
    cv::morphologyEx(fgMaskMOG2, fgMaskMOG2, cv::MORPH_OPEN, cv::Mat() , 
		      cv::Point(-1,-1), 2, cv::BORDER_CONSTANT, cv::morphologyDefaultBorderValue());
    cv::morphologyEx(fgMaskMOG2, fgMaskMOG2, cv::MORPH_CLOSE, cv::Mat() , 
		      cv::Point(-1,-1), 4, cv::BORDER_CONSTANT, cv::morphologyDefaultBorderValue());
    
    // Find contours
    thresholdMOG2 = fgMaskMOG2.clone();
    cv::findContours(thresholdMOG2, contoursMOG2, hierarchyMOG2, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );

    contours_polyMOG2 = std::vector<std::vector<cv::Point> >(contoursMOG2.size());
    std::vector<cv::RotatedRect> ellipseMOG2 = std::vector<cv::RotatedRect>(contoursMOG2.size());
    
    for( int i = 0; i < contoursMOG2.size(); i++ ){ 
      cv::approxPolyDP( cv::Mat(contoursMOG2[i]), contours_polyMOG2[i], 3, true );
      if(contours_polyMOG2[i].size() > 5 && cv::contourArea(contours_polyMOG2[i]) > 3000){
	ellipseMOG2[i] = cv::fitEllipse(cv::Mat(contours_polyMOG2[i]));
	entities.push_back(Entity(ellipseMOG2[i], contours_polyMOG2[i]));
      }
    }
    
    return entities;
  }
}

//remove ellipses outside of bounds
std::vector<Entity> PeopleDetector::filter_detections(std::vector<Entity> entities){
  if(!fst_im){
    
    std::vector<Entity> entitiesInborders;
    
    //filter ellipses outside of the defined borders
    cv::Size im_size = undistorter->imageSize();
    for(unsigned int i = 0; i < entities.size(); i++){
      cv::Point2f p = entities[i].ellipse.center;
      cv::Size2f s = entities[i].ellipse.size;

      if(p.y + s.height/2 - im_size.height <= 0 && p.y + s.width/2 - im_size.height <= 0 &&
	p.x + s.height/2 - im_size.height <= 0 && p.x + s.width/2 - im_size.height <= 0){
	if(p.y + s.height/2 - im_size.width <= 0 && p.y + s.width/2 - im_size.width <= 0 &&
	  p.x + s.height/2 - im_size.width <= 0 && p.x + s.width/2 - im_size.width <= 0){
	  if(p.y - s.height/2 > 0 && p.y - s.width/2 > 0){
	    if(p.x - s.height/2 > 0 && p.x - s.width/2 > 0){
	      cv::Point2f rect_points[4]; 
	      entities[i].ellipse.points( rect_points );
	      std::vector<bool> b(4, false);
	      for(unsigned int j = 0; j < surf.size(); j++){
// 		cv::circle(undistorted_im, (rect_points[2] + rect_points[3])*0.5, 10, cv::Scalar(0,0,0), 2, 8);
// 		if(surf[j].contains((rect_points[0] + rect_points[1])*0.5)){
		  b[0] = b[0] + true;
// 		}
		if(surf[j].contains((rect_points[1] + rect_points[2])*0.5)){
		  b[1] = b[1] + true;
		}
// 		if(surf[j].contains((rect_points[2] + rect_points[3])*0.5)){
		  b[2] = b[2] + true;
// 		}
		if(surf[j].contains((rect_points[3] + rect_points[0])*0.5)){
		  b[3] = b[3] + true;
		}
	      }
	      bool check = true;
	      for(unsigned int j = 0; j < b.size(); j++){
		check *= b[j];
	      }
	      if(!check)
		entitiesInborders.push_back(entities[i]);
	    }
	  }
	}
      }
    }
    
    return entitiesInborders;
  }
}

//classify filtered detections
std::vector<Entity> PeopleDetector::classify_detections(std::vector<Entity> entitiesInborders){
  //classify remaining ellipses
  cv::Size im_size = undistorter->imageSize();
  for(unsigned int i = 0; i < entitiesInborders.size(); i++){
    cv::Point2f p = entitiesInborders[i].ellipse.center;
    cv::Size2f s = entitiesInborders[i].ellipse.size;
    
    float d[4] = {p.x/im_size.width, p.y/im_size.height, s.height/im_size.height, s.width/im_size.width};
    cv::Mat t_row(1, 4, CV_32F, d);
    
    float response = SVM.predict(t_row);
    
    entitiesInborders[i].classification = response;
  }
  
  return entitiesInborders;
}

void PeopleDetector::plotEntities(std::vector<Entity> entities){
  if(!fst_im){
    cv::circle(undistorted_im, undistorter->image_center(), 10, cv::Scalar(0,0,0), 2, 8);
    for(unsigned int j = 0; j < surf.size(); j++){
      cv::rectangle(undistorted_im, surf[j], cv::Scalar(0,0,0), 5, 8);
    }
    
    for(unsigned int i = 0; i < entities.size(); i++){
       if(entities[i].classification == 1.0){
	cv::ellipse(undistorted_im, entities[i].ellipse, cv::Scalar(255,0,0), 5, 8 );
      }else if(entities[i].classification == 2.0){
	cv::ellipse(undistorted_im, entities[i].ellipse, cv::Scalar(0,255,0), 5, 8 );
// 	cv::circle(undistorted_im, entities[i].ellipse.center, 10, cv::Scalar(0,0,0), 2, 8);
      }else{
	cv::ellipse(undistorted_im, entities[i].ellipse, cv::Scalar(0,0,255), 5, 8 );
      }
    }
  }
}

double PeopleDetector::pixel2meters(double d){
  return p2mpoly[0]*d + p2mpoly[1];
}

void PeopleDetector::CallBackFunc(int event, int x, int y, int flags){
  if  (event == cv::EVENT_LBUTTONDOWN){
//     cv::Point2f im_center = undistorter->image_center();
//     cv::Size im_size = undistorter->imageSize();
//     
//     double dist = sqrt((x - im_center.x)*(x - im_center.x) + ((im_size.height - y) - im_center.y)*((im_size.height - y) - im_center.y));
//     double angle = atan2((im_size.height - y) - im_center.y, x - im_center.x);
// 
//     
//     std::cout << R.at<double>(0,0)*pixel2meters(dist*cos(angle))  + R.at<double>(0,1)*pixel2meters(dist*cos(angle)) << " " 
//     << R.at<double>(1,0)*pixel2meters(dist*sin(angle)) + R.at<double>(1,1)*pixel2meters(dist*sin(angle)) + T.at<double>(1) << std::endl;

//     for(unsigned int i = 0; i < people.size(); i++){
//       pos_data << people[i].dist2center << " " << pixel2meters(people[i].dist2center) << std::endl; 
//     }
//     std::cout << x << " " << y << std::endl;
  }else if( event == cv::EVENT_RBUTTONDOWN ){
    char b[1];
    sprintf(b,"%d",clicks);
    std::string image = "/home/mateus/undistorted";
    image += std::string(b) + ".png"; 

    cv::imwrite( image, undistorted_im );

    clicks++;
  }else if  ( event == cv::EVENT_MBUTTONDOWN ){

  }else if ( event == cv::EVENT_MOUSEMOVE ){
  }
}