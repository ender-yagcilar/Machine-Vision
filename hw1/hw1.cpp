#include "hw1.h"


void print_vector(std::vector<Point2D> const &input)
{
    for (int i = 0; i < input.size(); i++) {
        std::cout << input.at(i).x<<" - "<<input.at(i).y << std::endl;
    }
}

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
    Point2D a;
    a.y=y;
     if  ( event == cv::EVENT_LBUTTONDOWN )
     {
          
          if(x<*(int *)userdata){//First image
            a.x=x; 
            d.push_back(a);
          }
          else{//Second image
            a.x=x-*(int *)userdata; 
            d_prime.push_back(a);
          }
     }
}


int main( int argc, char** argv )
{


    int N;
/*This will be read from console and according to this, related images are opened.
0-> boats
1->LePoint1
2->LePoint2
3->WhiteBoards(This is resized)
*/
    std::cout << "Enter N value(0-3):";
    std::cin >> N;
    cv::Mat image1;

    image1 = cv::imread(path_to_data+data[2*N],cv::IMREAD_COLOR);
    if(! image1.data)
        {
            
            std::cout<<"Could not open file:First" << std::endl;
            return -1;
        } 

    cv::Mat image2;
    image2 = cv::imread(path_to_data+data[2*N+1],cv::IMREAD_COLOR);

    if(! image1.data)
        {
            
            std::cout<<"Could not open fileS:Second" << std::endl;
            return -1;
        } 

    if(!(image1.size[0]==image2.size[0]  && image1.size[1]==image2.size[1])){
        std::cout<<"Sizes of those images are different. Resizing is needed."<<std::endl;
        int w = std::max(image1.size[1],image2.size[1]);
        int h = std::max(image2.size[0],image2.size[0]);
        printf("w:%d , h=%d\n\n",w,h);
        cv::resize(image1,image1,cv::Size(w,h),cv::INTER_LINEAR);
        cv::resize(image2,image2,cv::Size(w,h),cv::INTER_LINEAR);
    };

    cv::Mat image_concat;
    cv::hconcat(image1,image2,image_concat);

    cv::namedWindow("image", cv::WINDOW_FREERATIO);
    cv::resizeWindow("image",window_width,window_height);
    

    cv::setMouseCallback("image", CallBackFunc, &image1.size[1]);
    cv::imshow("image", image_concat);
    cv::waitKey();

    printf("\nPoints that are selected in first image:\n");
    print_vector(d);
    printf("\nPoints that are selected in second image:\n");
    print_vector(d_prime);

    return 0;
}