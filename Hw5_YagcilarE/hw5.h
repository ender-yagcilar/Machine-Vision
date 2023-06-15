/*
 File name: HW5.h
 Author: Ender Yağcılar
 EE 576 - Project 5
 E-mail: ender.yagcilar@boun.edu.tr
 Date created: 06.05.2023
 Date last modified: -
 */


#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/ml.hpp>

using namespace std;
using namespace cv;
using namespace cv::ml;

struct staticstics_vars {
    double true_pos = 0;
    double true_neg = 0;
    double false_pos = 0;
    double false_neg = 0;
};

class HW5{
    public:
    // Parameters for features extraction
    const int VOCAB_SIZE = 10;
    const int DESCRIPTOR_SIZE = 128;
    const int MIN_HESSIAN = 400;
    
    // Paths for image files
    std::string bus_img_file_path = "/home/ender/opencv_projects/homeworks/Hw5_YagcilarE/cocoapi/PythonAPI/bus";
    std::string truck_img_file_path = "/home/ender/opencv_projects/homeworks/Hw5_YagcilarE/cocoapi/PythonAPI/truck";
    std::vector<cv::String> bus_imgs_paths, truck_imgs_paths;//Those will store each image paths

    // Feature detector  and descriptor extractor is defined here
    Ptr<FeatureDetector> detector = SIFT::create(MIN_HESSIAN);
    Ptr<DescriptorExtractor> extractor = SIFT::create(DESCRIPTOR_SIZE);

    
    // Create a vocabulary of visual words
    TermCriteria tc;

    // BOW extractor and vocabulary
    Ptr<DescriptorMatcher> matcher;
    Ptr<BOWImgDescriptorExtractor> bowExtractor;
    Mat vocabulary;

    //Those mat objects will store features
    Mat bus_features;
    Mat truck_features;

    //SVM Training
    Ptr<SVM> svm_bus;
    Ptr<SVM> svm_truck;

    //Recall Precision Calculation variables
    staticstics_vars truck;
    vector<double> truck_recall;
    vector<double> truck_precision;
    vector<double> truck_fpr;

    staticstics_vars bus;
    vector<double> bus_recall;
    vector<double> bus_precision ;
    vector<double> bus_fpr;

    void initialize();
    void create_vocab();//This method creates vocabulary
    void get_object_features();//This method gets features

    void train_SVM();
    
    void test_bus(double threshold);
    void test_truck(double threshold);
    void draw_roc_bus();
    void draw_roc_truck();


    private:
};