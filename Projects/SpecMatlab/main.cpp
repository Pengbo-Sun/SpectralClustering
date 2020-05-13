#include <iostream>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>

#include <matengine.h>
#include <setlabels.h>

#include <string>

int main(int argc, char **argv)
{

  std::string config_src;
  using namespace std;

  /////////////////////////////////////////////////////////////////////////////
  ///////////////////////     Load Data ///////////////////////////
  /////////////////////////////////////////////////////////////////////////////

  pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud_input(
      new pcl::PointCloud<pcl::PointXYZRGB>);

  if (pcl::io::loadPCDFile<pcl::PointXYZRGB>("scene.pcd", *cloud_input) == -1) //* load the file
  {
    PCL_ERROR("Couldn't read the point file \n");
    return (-1);
  }

  pcl::PointCloud<pcl::PointXYZL>::Ptr cloud_label(
      new pcl::PointCloud<pcl::PointXYZL>);

  if (pcl::io::loadPCDFile<pcl::PointXYZL>("labels.pcd", *cloud_label) == -1) //* load the file
  {
    PCL_ERROR("Couldn't read the point file \n");

    return (-1);
  }

  /////////////////////////////////////////////////////////////////////////////
  ///////////////////////     Set Labels            ///////////////////////////
  /////////////////////////////////////////////////////////////////////////////

  SetLabels labeling_lv1;
  labeling_lv1.loadlabeledcloud_v2(cloud_label, "/home/hkis/obj_rec");
  labeling_lv1.setlabels_v2(cloud_input, 1);

  pcl::PointCloud<pcl::PointXYZRGBL>::Ptr scene_labels(new pcl::PointCloud<pcl::PointXYZRGBL>);
  *scene_labels = *labeling_lv1.getlcloud();

  pcl::io::savePCDFileASCII("output.pcd", *scene_labels);

  return 0;
}
