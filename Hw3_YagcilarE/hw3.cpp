#include "hw3.h"


void HW3::create_hsv_table(){
    for(int i = 0; i < colors_bgr.size(); i++) {
        cv::Mat bgrMat(1, 1, CV_8UC3, colors_bgr.at(i));
        cv::Mat hsvMat;
        cv::cvtColor(bgrMat, hsvMat, cv::COLOR_BGR2HSV);

        // Get HSV color scalar
        cv::Scalar hsv = hsvMat.at<cv::Vec3b>(0, 0);
        colors_hsv.push_back(hsv);
    }
    
}

void HW3::get_image(){
    
    std::string image_name;
    std::cout << "Enter  the name of image: ";
    std::getline(std::cin, image_name);
    std::string path_image ="image.jpg";

    image_bgr = cv::imread(path_image,cv::IMREAD_COLOR);
    if(! image_bgr.data)
        {
            
            std::cout<<"Could not open file:First" << std::endl;
            this->get_image();
        }

    //Show the selected image
    cv::namedWindow("Selected Image", cv::WINDOW_NORMAL);
    cv::resizeWindow("Selected Image",600,600);
    cv::imshow("Selected Image", image_bgr);
    cv::waitKey();
    
}


void HW3::make_hsv(){
    cv::cvtColor(image_bgr, image_hsv, cv::COLOR_BGR2HSV);
}

void HW3::show_hsv_channels(){

    std::vector<cv::Mat> channels;
    cv::split(image_hsv, channels);
    

    cv::Mat Hue_Channel = channels[0];
    cv::Mat Saturation_Channel = channels[1];
    cv::Mat Value_Channel = channels[2];
    std::cout<<Value_Channel<<std::endl;

    cv::namedWindow("Hue Channel", cv::WINDOW_NORMAL);
    cv::resizeWindow("Hue Channel",600,600);
    cv::imshow("Hue Channel", Hue_Channel);

    cv::namedWindow("Saturation Channel", cv::WINDOW_NORMAL);
    cv::resizeWindow("Saturation Channel",600,600);
    cv::imshow("Saturation Channel", Saturation_Channel);

    cv::namedWindow("Value Channel", cv::WINDOW_NORMAL);
    cv::resizeWindow("Value Channel",600,600);
    cv::imshow("Value Channel", Value_Channel);
    cv::waitKey();
    cv::destroyAllWindows();
}

void HW3::threshold_color(){


    int color_choice;
    std::cout << "Make a color choice to filter(Input integer):\n\n1 - BLACK\n2 - Gray\n3 - WHITE\n4 - RED\n5 - GREEN\n6 - BLUE\n7 - ORANGE\n8 - YELLOW\nYour Choice:";
    std::cin >> color_choice;
    
    cv::Scalar lower;
    cv::Scalar upper;
        // Define the range of color in BGR color space
    switch (color_choice) {
        case 1://Black
            lower = colors_bgr.at(0);
            upper = colors_bgr.at(1);
            break;
        case 2://Gray
            lower = colors_bgr.at(1);
            upper = colors_bgr.at(3);
            break;
        case 3://White
            lower = colors_bgr.at(3);
            upper = colors_bgr.at(4);
            break;
        case 4://Red
            lower = colors_bgr.at(5);
            upper = colors_bgr.at(6);
            break;
        case 5://Green
            lower = colors_bgr.at(7);
            upper = colors_bgr.at(8);
            break;
        case 6://Blue
            lower = colors_bgr.at(9);
            upper = colors_bgr.at(10);
            break;
        case 7://Orange
            lower = colors_bgr.at(11);
            upper = colors_bgr.at(12);
            break;
        case 8://Yellow
            lower = colors_bgr.at(13);
            upper = colors_bgr.at(14);
            break;
    }

    cv::Mat Mask;//Thresholded image is stored here
    // Create a mask that only contains the masked
    cv::inRange(image_bgr, lower, upper, Mask);
    cv::Mat grayImg;
    cv::cvtColor(image_bgr, grayImg, cv::COLOR_BGR2GRAY);
    // Apply the mask to the original image
    
    cv::bitwise_and(grayImg, Mask, Masked_Regions);

    std::cout<<"Original and Masked images"<<std::endl;
    // Display the original image and the masked image
    cv::namedWindow("Original Image", cv::WINDOW_NORMAL);
    cv::resizeWindow("Original Image",600,600);
    cv::imshow("Original Image", image_bgr);
    
    cv::namedWindow("Masked Regions", cv::WINDOW_NORMAL);
    cv::resizeWindow("Masked Regions",600,600);
    cv::imshow("Masked Regions", Masked_Regions);
    cv::waitKey();
    cv::destroyAllWindows();

}

void HW3::PartA(){
    int flag=1;

    while(flag){
        create_hsv_table();
        get_image();
        make_hsv();
        show_hsv_channels();
        threshold_color();
        
        std::cout<<"Part A is done!\nDo you want to repeat or continue with Part B(0:Continue with Part B --- 1:Repeat)\n"<<std::endl;
        std::cin >> flag;
        
    }        
}

void HW3::opening_closing(){

    //I filtered the color i want in previous section.

    //First do opening and closing

        // Perform opening operation
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
    cv::Mat opened;
    cv::Mat closed;
    cv::morphologyEx(Masked_Regions, opened, cv::MORPH_OPEN, kernel);
    cv::morphologyEx(opened, closed, cv::MORPH_OPEN, kernel);
   
    opened_closed_Masked_Regions = closed;
    // Display the results
    cv::namedWindow("Original Image", cv::WINDOW_NORMAL);
    cv::resizeWindow("Original Image",600,600);
    cv::imshow("Original Image", image_bgr);

    cv::namedWindow("Masked Image", cv::WINDOW_NORMAL);
    cv::resizeWindow("Masked Image",600,600);
    cv::imshow("Masked Image", Masked_Regions);

    cv::namedWindow("After Opening and Closing", cv::WINDOW_NORMAL);
    cv::resizeWindow("After Opening and Closing",600,600);
    cv::imshow("After Opening and Closing", opened_closed_Masked_Regions);
    cv::waitKey();
    cv::destroyAllWindows();
    
}

void HW3::high_pass_filtering(){
    cv::filter2D(opened_closed_Masked_Regions, filteredImage, -1, filter);

    // Display the original and the filtered image
    cv::namedWindow("After Opening and Closing", cv::WINDOW_NORMAL);
    cv::resizeWindow("After Opening and Closing",600,600);
    cv::imshow("After Opening and Closing", opened_closed_Masked_Regions);

    cv::namedWindow("After High Pass Filter", cv::WINDOW_NORMAL);
    cv::resizeWindow("After High Pass Filter",600,600);
    cv::imshow("After High Pass Filter", filteredImage);
    cv::waitKey(0);
}

void HW3::PartB(){
    opening_closing();
    high_pass_filtering();
}
