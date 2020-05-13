#include <iostream>
#include <fstream>
#include <string>
#include <tuple>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <pcl/io/pcd_io.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/kdtree/kdtree_flann.h>

using namespace std;

class SetLabels
{

public:
  SetLabels();
  ~SetLabels();
  void loadlabeledcloud(pcl::PointCloud<pcl::PointXYZL>::Ptr labeled_voxel_cloud, std::string pathname);
  void loadlabeledcloud_v2(pcl::PointCloud<pcl::PointXYZL>::Ptr labeled_voxel_cloud, std::string pathname);
  void setlabels(pcl::PointCloud<pcl::PointXYZRGB>::Ptr input);
  void setlabels_v2(pcl::PointCloud<pcl::PointXYZRGB>::Ptr input, int variant = 1);
  pcl::PointCloud<pcl::PointXYZRGBL>::Ptr getcloud();
  pcl::PointCloud<pcl::PointXYZRGBL>::Ptr getlcloud();
  cv::Mat getimage();
  std::map<int,cv::Mat> getimages();

  void setorigincolor(cv::Mat origin);

  

private:
  pcl::PointCloud<pcl::PointXYZRGBL>::Ptr _labeled_voxel_cloud;
  pcl::PointCloud<pcl::PointXYZRGBL>::Ptr _tmp_voxel;
  std::string _pathforfiles;
  std::vector<int> labels;

  pcl::PointCloud<pcl::PointXYZRGBL>::Ptr _input_cloud;

  std::vector<cv::Vec3b> color_vector;
  cv::Mat image;
  cv::Mat origincolor;

  std::map<int,cv::Mat> images;

  void setLabelstovoxelcloud();
  void loadLabelsfromCloud(std::string pathname);

  void setLabelstoPointCloud();
  void setLabelstoImage();
  void setLabelstoImages();
  
};
