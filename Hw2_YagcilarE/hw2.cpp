/*
 File name: HW2.h
 Author: Ender Yağcılar
EE 576 - Project 2
 E-mail: ender.yagcilar@boun.edu.tr
 Date created: 23.02.2023
 Date last modified: 9.03.2023
 */

#include "hw2.h"



std::vector<std::vector<cv::Point>> EE576::click_vect = {};



void EE576::select_images(){
    
    std::cout << "Enter N value(0-99):";
    std::cin >> this->N;

    std::string path_image1 =path_to_data+"frame_" + std::to_string(this->N) +".jpg";
    std::string path_image2 =path_to_data+"frame_" + std::to_string(this->N+1)+".jpg";

    this->image1 = cv::imread(path_image1,cv::IMREAD_COLOR);
    
    if(! image1.data)
        {
            
            std::cout<<"Could not open file:First" << std::endl;
            EE576::select_images();
        } 

    this->image2 = cv::imread(path_image2,cv::IMREAD_COLOR);

    if(! this->image2.data)
        {
            
            std::cout<<"Could not open file:Second" << std::endl;
            EE576::select_images();
        }

}

void EE576::print_vector(std::vector<cv::Point> const &input)
{
    for (int i = 0; i < input.size(); i++) {
        std::cout << input.at(i).x<<" - "<<input.at(i).y << std::endl;
    }
};



void EE576::CallBackFunc(int event,int x, int y, int flags, void* userdata)
{
    int * windowID = (int*)userdata;
    if  ( event == cv::EVENT_LBUTTONDOWN )
     {
        printf("basıldı, window = %d,x = %d");
        EE576::click_vect.at(*windowID).push_back(cv::Point(x,y));
     }    

}


void EE576::get_clicks(std::vector<cv::Mat> imgs,std::vector<std::string> wnd_names){
    EE576::click_vect.clear();
    int N = imgs.size();

    int arr[N];
    for(int i = 0; i < N; i++) {
        arr[i] = i;//I use this array to use in setMouseCallback
    }

    for(int i=0;i<N;i++){
        cv::namedWindow(wnd_names.at(i), cv::WINDOW_NORMAL);
        cv::resizeWindow(wnd_names.at(i),this->window_width,this->window_height);

        cv::imshow(wnd_names.at(i), imgs.at(i));

        cv::setMouseCallback(wnd_names.at(i),EE576::CallBackFunc, arr+i);

        std::vector<cv::Point> row1 = { cv::Point(0,0)};
        std::vector<cv::Point> row2 = { cv::Point(0,0)};

        click_vect.push_back(row1);
        click_vect.push_back(row2);
    }cv::waitKey();

}



cv::Mat EE576::calc_homo_part_A(){

   //We take first 4 elements of d and d' vectors and assign it to x and x' vector. Those are ordered such as 
    //[x_of_first_point y_of_first_point x_of_second_point y_of_second_point x_of_3rd_point y_of_3rd_point x_of_4rd_point y_of_4rd_point]
    
    float x[8] = {click_vect[0].at(1).x,click_vect[0].at(1).y,click_vect[0].at(2).x,click_vect[0].at(2).y,click_vect[0].at(3).x,click_vect[0].at(3).y,click_vect[0].at(4).x,click_vect[0].at(4).y};
    float x_prime[8] = {click_vect[1].at(1).x,click_vect[1].at(1).y,click_vect[1].at(2).x,click_vect[1].at(2).y,click_vect[1].at(3).x,click_vect[1].at(3).y,click_vect[1].at(4).x,click_vect[1].at(4).y};
    


    float A[6][6] = {{x[0], x[1],   1,    0,    0,    0},
                     {0,     0,     0,   x[0], x[1],   1},
                     {x[2], x[3],   1,    0,    0,    0},
                     {0,     0,     0,   x[2], x[3],   1},
                     {x[4], x[5],   1,    0,    0,    0},
                     {0,     0,     0,   x[4], x[5],   1}};

    float a[6] = {x_prime[0],x_prime[1],x_prime[2],x_prime[3],x_prime[4],x_prime[5]};

    cv::Mat A_matrix(cv::Size(6,6),CV_32F,&A);
    cv::Mat a_matrix(cv::Size(1,6), CV_32F,&a);
    cv::Mat part_A_h_vect(cv::Size(1,6),CV_32F);

    cv::solve(A_matrix,a_matrix,part_A_h_vect,cv::DECOMP_SVD);// Here ı solve for A * q = a.
    
    //Construction of H matrix from h vector
    float H_partA_mat[3][3] = {{part_A_h_vect.at<float>(0),part_A_h_vect.at<float>(1),part_A_h_vect.at<float>(2)},
                         {part_A_h_vect.at<float>(3),part_A_h_vect.at<float>(4),part_A_h_vect.at<float>(5)},
                         {              0,                      0,                          1}};

    cv::Mat H_partA(cv::Size(3,3), CV_32F,&H_partA_mat);

    return H_partA.clone();


}

cv::Mat EE576::calc_homo_part_B(){
//Reference to "https://docs.opencv.org/4.x/d9/dab/tutorial_homography.html"
    /*       Part B       */
    
    std::vector<cv::Point> corners1, corners2;

    for(int i=0;i<4;i++){
        corners1.push_back(cv::Point2f(click_vect[0].at(i+1).x, click_vect[0].at(i+1).y));
        corners2.push_back(cv::Point2f(click_vect[1].at(i+1).x,click_vect[1].at(i+1).y));
    }

    cv::Mat H_partB = findHomography(corners1, corners2);
    return H_partB;
}



cv::Mat EE576::combine_imgages(cv::Mat H_map){
    cv::Mat image_concat;
    cv::hconcat(this->image1,this->image2,image_concat);

    cv::Mat img1_warp;
    cv::warpPerspective(this->image1, img1_warp, H_map, cv::Size(this->image1.cols, this->image1.rows));
    cv::Mat half = img1_warp(cv::Rect(0, 0, this->image2.cols, this->image2.rows));


    for(int i=0;i<img1_warp.cols;i++){
        for(int j=0;j<img1_warp.rows;j++){
            if(half.at<cv::Vec3b>(j, i)==cv::Vec3b(0,0,0)){        
                img1_warp.at<cv::Vec3b>(j,i) = this->image2.at<cv::Vec3b>(j, i);
            }
        }
    }

    return img1_warp;
}

void EE576::hw2_work(){
    select_images();
    std::vector<cv::Mat> images = {image1,image2};
    std::vector<std::string> wnd_names = {"image0","image1"};
    get_clicks(images,wnd_names);



    if(EE576::click_vect[0].size()<5 ||EE576::click_vect[1].size()<5){std::cout<<"Less than 4 points from each image is selected. Quit"<<std::endl;return;}
    // ı used  5 since first element is dummy




                /*                  Part A and B                  */

    
    cv::Mat H_partA = calc_homo_part_A();
    cv::Mat H_partB = calc_homo_part_B();


    //Results of Part A and B   
    std::cout<<"Homography Matrix for Part A)=\n"<<H_partA<<"\n\nHomography Matrix for Part B)=\n"<<H_partB<<std::endl;

    //             /*                   Part C                       */

    // I couldn't make this part a method since i couldnt make callback function a method. I will work on it in next projects.   
    //Manually Computed Homograpy Matrix
    for(int i = 1; i<N2 ;i++){
        cv::Mat resultant_image = combine_imgages(H_partA);
        cv::Mat new_image = cv::imread(path_to_data+"frame_" + std::to_string((N)+i+1) +".jpg",cv::IMREAD_COLOR);

        images = {resultant_image,new_image};
        std::vector<std::string> wnd_names = {"Final Image - Manual Computed N = "+std::to_string(i),"New Image"};
        get_clicks(images,wnd_names);


        cv::destroyAllWindows();
        H_partA = calc_homo_part_A();
    }

    //Homography Matrix using by opencv cv::findHomography() meth
    for(int i = 1; i<N2 ;i++){
        cv::Mat resultant_image = combine_imgages(H_partB);
        cv::Mat new_image = cv::imread(path_to_data+"frame_" + std::to_string((N)+i+1) +".jpg",cv::IMREAD_COLOR);

        images = {resultant_image,new_image};
        std::vector<std::string> wnd_names = {"Final Image - Manual Computed N = "+std::to_string(i),"New Image"};
        get_clicks(images,wnd_names);


        cv::destroyAllWindows();
        H_partB = calc_homo_part_B();
    }



}



