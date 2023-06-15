/*
 File name: HW6.h
 Author: Ender Yağcılar
 EE 576 - Project 6
 E-mail: ender.yagcilar@boun.edu.tr
 Date created: 23.05.2023
 Date last modified: 24.05.2023
 */


#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
using namespace cv;

#include <iostream>
#include <dirent.h>
#include <regex>

class HW6{
    public:
        //This contains direction for frames, either car or human
        std::string framesDir;
        std::string car_path="/home/ender/opencv_projects/homeworks/Hw6_YagcilarE/Car2/img/";
        std::string human_path="/home/ender/opencv_projects/homeworks/exam/Human9/img/";
        //Number of frames in that folder.
        int num_of_frames = 0;
    
        //Methods
        void select_dataset();
        void PartA();
        void PartB();
    private:
};