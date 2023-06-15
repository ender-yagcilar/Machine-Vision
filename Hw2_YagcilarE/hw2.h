/*
 File name: HW2.h
 Author: Ender Yağcılar
 EE 576 - Project 2
 E-mail: ender.yagcilar@boun.edu.tr
 Date created: 23.02.2023
 Date last modified: 9.03.2023
 */

#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>





class EE576{
    public:
        
        std::string path_to_data = "data/";//path with respect to working directory

        int window_width = 900;//width of output window
        int window_height = 600;//height of output window

        //Methods
        void print_vector(std::vector<cv::Point> const &input);//Prints a vector in type of Point



        int N;//Parameter to determine which image will be opened
        cv::Mat image1;
        cv::Mat image2;
        void select_images();
        static void CallBackFunc(int event,int x, int y, int flags, void* userdata);

        //std::vector<std::vector<cv::Point>> get_clicks(std::vector<cv::Mat> imgs,std::vector<std::string> wnd_names);//This method shows images in same window
//       std::vector<cv::Point> d;      //Input point vector of first image
//       std::vector<cv::Point> d_prime;//Input point vector of second image

        void get_clicks(std::vector<cv::Mat> imgs,std::vector<std::string> wnd_names);//This method shows images in same window
        static std::vector<std::vector<cv::Point>> click_vect;
                            /*                                                   HW2                                                        */

            /*  Part A-B    */
    /* At least 3 different pair of points are needed to generate 6 equations to solve for 6 unkowns since each point generates 2 equations.
    However,4 points are needed for part B since there are 8 unkownns for that case. Thats why, minimum needed number of points is 4.
    x * H = x'
    */
    int N_1  =4;
    cv::Mat calc_homo_part_A();/*Calculates homogragpy map for part A*/
    cv::Mat calc_homo_part_B();/*Calculates homogragpy map for part B*/

            /*  Part C    */
    /*
    Here, by using images and homography maps, new images are formed. N2 is chosen 3 arbitraryly.
    */
   int N2 = 3;
   cv::Mat combine_imgages(cv::Mat H_map);/*Combines images*/

    /*
    MY COMMENT ON N1 AND N2:
    As N1 increases, opencv findHomography can find more accurate homography maps.
    As N2 increases, deviation increases significantly and images become meaningless.
    */

   void hw2_work();//Method to execute hw2

    private:

};

