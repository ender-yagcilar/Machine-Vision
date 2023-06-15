/*
 File name: HW3.h
 Author: Ender Yağcılar
 EE 576 - Project 4
 E-mail: ender.yagcilar@boun.edu.tr
 Date created: 08.04.2023
 Date last modified: -
 */

#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <unordered_map>
#include <opencv2/xfeatures2d.hpp>


using namespace std;
using namespace cv;

class HW4{
    public:

        std::string image_path= "val2017/000000001584.jpg";//path with respect to working directory
        Mat input;
        Mat hsv_img;
        Mat seg_mat;//This mat object stores labels
    
        int label = 1;
        vector<cv::Point2i> eq_table;//Equilibrium table
        const int THRESHOLD =15;//HSV Value Threshhold 
        vector<int> area_upper_thr = {500,3000,100000};
        vector<int> area_lower_thr = {0,500,3000};
        Mat segmentation_result;
        unordered_map<int, int> segment_vs_area_map;//This map stores corresponding area of each segment
        vector<int> areas_between_thresholds;

        Mat interested_segment_area;//This gets seg_mat and makes 0 every pixel that are out of area threshold bounds
        Mat interested_segments;//This is colormap mat for storing and showing segment areas that are inside area threshold bounds

        Mat sift_segment;
        int sift_segment_label = 5777;//I will send sift segment labeled 5777
        void initialize();
        void segmentation();
        void get_segment_areas();
        void show_interested_segment_areas(int segment_size);//Segment_size ->0 for small 1->medium 2-> large
        void findSIFTFeatures(const Mat& segment, const std::string& outputFileName);
        void addPointIfNotPresent(vector<Point2i>& pointVector, const Point2i& point);
        static bool cmp(cv::Point2i a, cv::Point2i b);

    private:
    ;

};