#include <setlabels.h>

#include <random>

SetLabels::SetLabels()
{
    _labeled_voxel_cloud = pcl::PointCloud<pcl::PointXYZRGBL>::Ptr(new pcl::PointCloud<pcl::PointXYZRGBL>());
    _tmp_voxel = pcl::PointCloud<pcl::PointXYZRGBL>::Ptr(new pcl::PointCloud<pcl::PointXYZRGBL>());
    _input_cloud = pcl::PointCloud<pcl::PointXYZRGBL>::Ptr(new pcl::PointCloud<pcl::PointXYZRGBL>());
    int numberoflabels = 300;
    color_vector.reserve(numberoflabels);

    for (unsigned int i = 0; i < numberoflabels; i++)
    {
        auto tou = cv::Vec3b(rand() % 256, rand() % 256, rand() % 256);
        // if color to dark
        while (cv::norm(tou) < 30)
        {
            tou = cv::Vec3b(rand() % 256, rand() % 256, rand() % 256);
        }
        color_vector.push_back(tou);
    }
}
SetLabels::~SetLabels() = default;

void SetLabels::loadlabeledcloud(pcl::PointCloud<pcl::PointXYZL>::Ptr labeled_voxel_cloud, std::string pathname)
{
    copyPointCloud(*labeled_voxel_cloud, *_labeled_voxel_cloud);
    //_labeled_voxel_cloud = labeled_voxel_cloud;
    _pathforfiles = pathname + "/list.txt";
}
void SetLabels::loadlabeledcloud_v2(pcl::PointCloud<pcl::PointXYZL>::Ptr labeled_voxel_cloud, std::string pathname)
{
    copyPointCloud(*labeled_voxel_cloud, *_labeled_voxel_cloud);
    copyPointCloud(*labeled_voxel_cloud, *_tmp_voxel);
    for (auto &t : _tmp_voxel->points)
    {
        t.label = 0;
    }
    SetLabels::loadLabelsfromCloud(pathname);
    SetLabels::setLabelstovoxelcloud();
}

void SetLabels::setlabels_v2(pcl::PointCloud<pcl::PointXYZRGB>::Ptr input, int variant)
{
    copyPointCloud(*input, *_input_cloud);

    switch (variant)
    {
    case 1:
        SetLabels::setLabelstoPointCloud();
        break;
    case 2:
        SetLabels::setLabelstoImage();
        break;
    case 3:
        SetLabels::setLabelstoImages();
        break;
    default:
        std::cout << "False Variant" << std::endl;
    }
}

void SetLabels::setlabels(pcl::PointCloud<pcl::PointXYZRGB>::Ptr input)
{
    copyPointCloud(*input, *_input_cloud);
    // Laden der neuen Labels
    std::ifstream File;
    File.open(_pathforfiles);
    std::string line;
    int x;
    File >> x;
    int tmp = _labeled_voxel_cloud->points[0].label;
    int test;
    // Iteration durch Labes
    for (size_t i = 0; i < _labeled_voxel_cloud->size(); i++)
    {
        // Falls Labels wechseln, neues Label laden
        if (tmp == _labeled_voxel_cloud->points[i].label)
        {
            _labeled_voxel_cloud->points[i].label = x;
        }
        else
        {
            while (tmp != _labeled_voxel_cloud->points[i].label)
            {
                tmp++;
                File >> x;
                //std::cout << x << std::endl;
            }
            _labeled_voxel_cloud->points[i].label = x;
        }
        test = i;
    }
    pcl::KdTreeFLANN<pcl::PointXYZRGBL> kdtree;
    kdtree.setInputCloud(_labeled_voxel_cloud);

    int K = 1;
    for (size_t i = 0; i < _input_cloud->size(); i++)
    {
        std::vector<int> pointIdxNKNSearch(K);
        std::vector<float> pointNKNSquaredDistance(K);

        if (kdtree.nearestKSearch(_input_cloud->points[i], K, pointIdxNKNSearch, pointNKNSquaredDistance) > 0)
        {
            _input_cloud->points[i].label = _labeled_voxel_cloud->points[pointIdxNKNSearch[0]].label;
        }
    }
}

pcl::PointCloud<pcl::PointXYZRGBL>::Ptr SetLabels::getcloud()
{
    return _labeled_voxel_cloud;
}

pcl::PointCloud<pcl::PointXYZRGBL>::Ptr SetLabels::getlcloud()
{
    return _input_cloud;
}

cv::Mat SetLabels::getimage()
{
    return image;
}

std::map<int, cv::Mat> SetLabels::getimages()
{
    return images;
}

void SetLabels::setorigincolor(cv::Mat origin)
{
    cv::Mat origincolor = origin;
}

void SetLabels::setLabelstovoxelcloud()
{
    for (unsigned int i = 0; i < labels.size(); i++)
    {
        int tmp_label = labels[i];
        for (size_t c_points = 0; c_points < _labeled_voxel_cloud->size(); c_points++)
        {
            if (i + 1 == _labeled_voxel_cloud->points[c_points].label)
            {
                _tmp_voxel->points[c_points].label = tmp_label;
            }
        }
        //if (counter == 0)
        //{
        //    std::cout << "missing element" << std::endl;
        //    std::cout << tmp_label << std::endl;
        //    std::cout << i << std::endl;
        //}
    }
}

void SetLabels::loadLabelsfromCloud(std::string pathname)
{
    _pathforfiles = pathname + "/list.txt";
    std::ifstream File;
    File.open(_pathforfiles);
    while (true)
    {
        int x;
        File >> x;
        if (File.eof())
            break;
        labels.push_back(x);
    }
}

void SetLabels::setLabelstoPointCloud()
{
    pcl::KdTreeFLANN<pcl::PointXYZRGBL> kdtree;
    kdtree.setInputCloud(_tmp_voxel);
    std::cout << "Set to points" << std::endl;
    int K = 1;
    for (size_t i = 0; i < _input_cloud->size(); i++)
    {
        if (pcl::isFinite(_input_cloud->points[i]))
        {
            std::vector<int> pointIdxNKNSearch(K);
            std::vector<float> pointNKNSquaredDistance(K);
            if (kdtree.nearestKSearch(_input_cloud->points[i], K, pointIdxNKNSearch, pointNKNSquaredDistance) > 0)
            {
                _input_cloud->points[i].label = _tmp_voxel->points[pointIdxNKNSearch[0]].label;
            }
        }
    }
}

void SetLabels::setLabelstoImage()
{

    image = cv::Mat::zeros(_input_cloud->height, _input_cloud->width, CV_8UC3);

    pcl::KdTreeFLANN<pcl::PointXYZRGBL> kdtree;
    kdtree.setInputCloud(_tmp_voxel);
    std::cout << "Set to image" << std::endl;
    int K = 1;
    int row = 0;
    int col = 0;
    for (size_t i = 0; i < _input_cloud->size(); i++)
    {
        if (pcl::isFinite(_input_cloud->points[i]))
        {
            std::vector<int> pointIdxNKNSearch(K);
            std::vector<float> pointNKNSquaredDistance(K);
            if (kdtree.nearestKSearch(_input_cloud->points[i], K, pointIdxNKNSearch, pointNKNSquaredDistance) > 0)
            {
                _input_cloud->points[i].label = _tmp_voxel->points[pointIdxNKNSearch[0]].label;

                // care of BGR and RGB !!! ...rgb pointcloud and bgr opencv
                image.at<cv::Vec3b>(row, col) = color_vector[_input_cloud->points[i].label];
            }
        }
        col++;
        if (col == _input_cloud->width)
        {
            col = 0;
            row++;
        }
    }
}

void SetLabels::setLabelstoImages()
{

    pcl::KdTreeFLANN<pcl::PointXYZRGBL> kdtree;
    kdtree.setInputCloud(_tmp_voxel);
    std::cout << "Set to images" << std::endl;
    int K = 1;
    int row = 0;
    int col = 0;
    for (size_t i = 0; i < _input_cloud->size(); i++)
    {
        if (pcl::isFinite(_input_cloud->points[i]))
        {
            std::vector<int> pointIdxNKNSearch(K);
            std::vector<float> pointNKNSquaredDistance(K);
            if (kdtree.nearestKSearch(_input_cloud->points[i], K, pointIdxNKNSearch, pointNKNSquaredDistance) > 0)
            {
                auto it = images.find(_tmp_voxel->points[pointIdxNKNSearch[0]].label);
                if (it != images.end())
                {
                    auto tou = cv::Vec3b(_input_cloud->points[i].b, _input_cloud->points[i].g, _input_cloud->points[i].r);
                    it->second.at<cv::Vec3b>(row, col) = tou;
                }
                else
                {
                    cv::Mat tmp = cv::Mat::zeros(_input_cloud->height, _input_cloud->width, CV_8UC3);
                    images[_tmp_voxel->points[pointIdxNKNSearch[0]].label] = cv::Mat::zeros(_input_cloud->height, _input_cloud->width, CV_8UC3);
                }
            }
        }
        col++;
        if (col == _input_cloud->width)
        {
            col = 0;
            row++;
        }
    }
}