/*
 File name: HW3.h
 Author: Ender Yağcılar
 EE 576 - Project 3
 E-mail: ender.yagcilar@boun.edu.tr
 Date created: 23.03.2023
 Date last modified: -
 */

#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <ncurses.h>

class HW3{
    public:
    

            // Define color constants in BGR format
    //https://redketchup.io/color-picker
        std::vector<cv::Scalar> colors_bgr = {
            cv::Scalar(0, 0, 0),//BGR_BLACK0
            cv::Scalar(64, 64, 64),//BGR_DARK_GRAY1
            cv::Scalar(128, 128, 128),//BGR_MID_GRAY2
            cv::Scalar(192, 192, 192),//BGR_LIGHT_GRAY3
            cv::Scalar(255, 255, 255),//BGR_white4
            cv::Scalar(0, 0, 100),//BGR_RED_DARK5
            cv::Scalar(85, 85, 255),//BGR_RED_LIGHT 6
            cv::Scalar(0, 100, 0),//BGR_GREEN_DARK7
            cv::Scalar(160, 240, 160),//BGR_GREEN_LIGHT8

            cv::Scalar(110, 70, 55),//BGR_BLUE_DARK9
            cv::Scalar(255, 180, 130),//BGR_BLUE_LIGHT10

            cv::Scalar(30, 50, 110),//BGR_Orange_DARK11
            cv::Scalar(80, 115, 245),//BGR_ORANGE_LIGHT120, 190, 255

            cv::Scalar(0, 102, 102),//BGR_YELLOW_DARK13
            cv::Scalar(102, 255, 255)//BGR_YELLOW_LIGHT14
        };

        std::vector<cv::Scalar> colors_hsv;
        void create_hsv_table();// Iterate over each BGR color and convert it to HSI format

        cv::Mat image_bgr;
        void get_image();//gets image

        cv::Mat image_hsv;//hsv image
        void make_hsv();//changes image to hsv
        void show_hsv_channels();

        
        void threshold_color();//Creates color to filter and threshold
        cv::Mat Masked_Regions;//Thresholded image is stored here

        void PartA();//Method for doing part A

        void opening_closing();
        cv::Mat opened_closed_Masked_Regions;

        cv::Mat filter = (cv::Mat_<float>(3,3) << -1, -1, -1, -1, 9, -1, -1, -1, -1);
        cv::Mat filteredImage;
        void high_pass_filtering();

        void PartB();//Method for doing part B



        std::string path_to_data = "ColorImageDataset/";//path with respect to working directory
        std::string path_image;
    private:
    ;

};