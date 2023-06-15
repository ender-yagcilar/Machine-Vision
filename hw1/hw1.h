/*
 File name: HW1.h
 Author: Ender Yağcılar
 E-mail: ender.yagcilar@boun.edu.tr
 Date created: 23.02.2023
 Date last modified: 25.02.2023
 */

#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>


struct Point2D{
    int x;
    int y;
};//I will use this struct to store clicked points

std::vector<Point2D> d;      //Input point vector of first image
std::vector<Point2D> d_prime;//Input point vector of second image

//String array of images
std::string data[8] = 
        {"boatA.png","boatB.png","LePoint1A.png","LePoint1B.png",
        "LePoint2A.png","LePoint2B.png","WhiteBoardA.jpg","WhiteBoardB.jpg"};
std::string path_to_data = "data/";//path with respect to working directory

int window_width = 1500;//width of output window
int window_height = 1000;//height of output window

//prints vector of Point2D
void print_vector(std::vector<Point2D> const &input);

//This function is called when clicked
void CallBackFunc(int event, int x, int y, int flags, void* userdata);





