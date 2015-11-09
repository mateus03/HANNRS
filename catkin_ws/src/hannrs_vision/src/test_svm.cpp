//opencv
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>

//ROS
#include <ros/ros.h>

int main(int argc, char* argv[]){
  ros::init(argc, argv, "svm_train");
  
  std::string data_file, test_file, class_file;
  
  ros::NodeHandle pnh("~");
  pnh.getParam("data_file", data_file);
  pnh.getParam("test_file", test_file);
  pnh.getParam("classifier", class_file);
  
  cv::Mat training_data, train_lables;
  cv::FileStorage data(data_file, cv::FileStorage::READ);
  data["data"] >> training_data;
  data["labels"] >> train_lables;
  
  cv::Mat test_data, test_lables;
  cv::FileStorage test(test_file, cv::FileStorage::READ);
  test["data"] >> test_data;
  test["labels"] >> test_lables;
  
  CvSVMParams params;
  params.svm_type = CvSVM::C_SVC;
  params.kernel_type = CvSVM::RBF;
  params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 10000, FLT_EPSILON);
  
  // Train the SVM
  CvSVM SVM;
  SVM.train_auto(training_data, train_lables, cv::Mat(), cv::Mat(), params);

  CvSVMParams params2 = SVM.get_params();
  std::cout << params2.C << " " <<params2.gamma << std::endl;
  
  int c = 0;
  for(unsigned int i = 0; i < test_data.rows; i++){
    float response = SVM.predict(test_data.row(i));
    
    if(response == test_lables.at<float>(i)){
      c++;
    }
  }
  
  std::cout << c << " " << test_data.rows << " " << c*1.0/(test_data.rows*1.0) << std::endl;
  
  std::cout << class_file << std::endl;
//   SVM.save(class_file.c_str());
  
  return 0;
}
