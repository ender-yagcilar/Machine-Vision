#include "hw4.h"

void HW4::addPointIfNotPresent(std::vector<cv::Point2i>& pointVector, const cv::Point2i& point) {
    bool present = false;
    for (const cv::Point2i& p : pointVector) {
        if (p == point) {
            present = true;
            break;
        }
    }
    if (!present) {
        pointVector.push_back(point);
    }
}

bool HW4::cmp(cv::Point2i a, cv::Point2i b) {
    if (a.x == b.x) {
        return a.y < b.y;
    }
    return a.x < b.x;
}

void HW4::initialize(){
    input = imread(image_path);
    cvtColor(input, hsv_img, COLOR_BGR2HSV);

    seg_mat=Mat(input.rows, input.cols, CV_32S);
    seg_mat.setTo(Scalar(0));
}

void HW4::segmentation(){
        for (int i = 0; i < hsv_img.rows; i++) {
        for (int j = 0; j < hsv_img.cols; j++) {

    bool present = false;
    for (const cv::Point2i& p : pointVector) {
        if (p == point) {
            present = true;
            break;
        }
    }
    if (!present) {
        pointVector.push_back(point);
    }
}

bool HW4::cmp(cv::Point2i a, cv::Point2i b) {
    if (a.x == b.x) {
        return a.y < b.y;
    }
    return a.x < b.x;
}

void HW4::initialize(){
    input = imread(image_path);
    cvtColor(input, hsv_img, COLOR_BGR2HSV);

    seg_mat=Mat(input.rows, input.cols, CV_32S);
    seg_mat.setTo(Scalar(0));
}

void HW4::segmentation(){
        for (int i = 0; i < hsv_img.rows; i++) {
        for (int j = 0; j < hsv_img.cols; j++) {

            Vec3b pixel = hsv_img.at<Vec3b>(i, j);

            int value = pixel.val[2];

            if (value == 0) continue;//Eğer intensity 0'sa bu pixeli geç
            if (seg_mat.at<int>(i, j) != 0) {//Eğer seg_mat matrisinin o pixeli 0 değilse geç
                continue;
            }
            if( (i==0 && hsv_img.at<Vec3b>(i-1, j).val[2] != 0) || (j==0 && hsv_img.at<Vec3b>(i, j-1).val[2] != 0)){//En üstü veya solu gezerken bir üstündeki veya solundaki pixel için
            // 0 dışı değer veriyorsa bu pixeli geç(denedim gördüm kimi değerler için 0 dışı değer verdi)
                continue;
            }


            

            /*Getting intensity values of above and left pixel*/

            //Pixel above
            Vec3b pixel_above = hsv_img.at<Vec3b>(i-1, j);
            int abv_val = pixel_above.val[2];

            //Pixel right
            Vec3b pixel_left = hsv_img.at<Vec3b>(i, j-1);
            int left_val = pixel_left.val[2];

            int abv_val_diff =  abs(value - abv_val);
            int left_val_diff = abs(value - left_val);



            /*          Labeling                */

            if((abv_val_diff > THRESHOLD) && (left_val_diff > THRESHOLD)){
                label+=1;
                seg_mat.at<int>(i, j) = label;
            }

            else if((abv_val_diff < THRESHOLD) && (left_val_diff > THRESHOLD)){
                seg_mat.at<int>(i, j) = seg_mat.at<int>(i-1, j); 
            }

            else if((abv_val_diff > THRESHOLD) && (left_val_diff < THRESHOLD)){
                //Intensity farkı üstün threshholdun üstünde, ama solun altında -> solun labelini at
                seg_mat.at<int>(i, j) = seg_mat.at<int>(i, j-1); 
            }

            else if((abv_val_diff <= THRESHOLD) && (left_val_diff <= THRESHOLD) ){

        
                if(seg_mat.at<int>(i-1, j)==seg_mat.at<int>(i, j-1)){//Eğer üst ve sol aynıysa aynı labeli yapıştır geç
                  


            

            /*Getting intensity values of above and left pixel*/

            //Pixel above
            Vec3b pixel_above = hsv_img.at<Vec3b>(i-1, j);
            int abv_val = pixel_above.val[2];

            //Pixel right
            Vec3b pixel_left = hsv_img.at<Vec3b>(i, j-1);
            int left_val = pixel_left.val[2];

            int abv_val_diff =  abs(value - abv_val);
            int left_val_diff = abs(value - left_val);



            /*          Labeling                */

            if((abv_val_diff > THRESHOLD) && (left_val_diff > THRESHOLD)){
                label+=1;
                seg_mat.at<int>(i, j) = label;
            }

            else if((abv_val_diff < THRESHOLD) && (left_val_diff > THRESHOLD)){
                seg_mat.at<int>(i, j) = seg_mat.at<int>(i-1, j); 
            }

            else if((abv_val_diff > THRESHOLD) && (left_val_diff < THRESHOLD)){
                //Intensity farkı üstün threshholdun üstünde, ama solun altında -> solun labelini at
                seg_mat.at<int>(i, j) = seg_mat.at<int>(i, j-1); 
            }

            else if((abv_val_diff <= THRESHOLD) && (left_val_diff <= THRESHOLD) ){

        
                if(seg_mat.at<int>(i-1, j)==seg_mat.at<int>(i, j-1)){//Eğer üst ve sol aynıysa aynı labeli yapıştır geç
                  
                    //printf(", sol ve üst eşit\n");
                    seg_mat.at<int>(i, j) = seg_mat.at<int>(i-1, j);
                }
                else{
                    seg_mat.at<int>(i, j) = seg_mat.at<int>(i, j-1);
                    cv::Point2i eq_elements(min(seg_mat.at<int>(i, j-1),seg_mat.at<int>(i-1, j)),max(seg_mat.at<int>(i, j-1),seg_mat.at<int>(i-1, j)));
                    addPointIfNotPresent(eq_table,eq_elements);
                }
            }
        }
    }

//Equivalence tablea göre elemanları değiştirme
    sort(eq_table.begin(), eq_table.end(), cmp);
    

    for(int k = eq_table.size()-1;k>0;k--){
        for (int i = 0; i < seg_mat.rows; i++) {
            for (int j = 0; j < seg_mat.cols; j++) {

                //Eğer eq_table'ın k. indexindeki y değeri matriste denk gelirse
                //onu  eq_table'ın k. indexindeki x değerine eşitle
                if(seg_mat.at<int>(i, j) ==eq_table.at(k).y){
                    seg_mat.at<int>(i, j) = eq_table.at(k).x;

                }

            }
        }
    }

    Mat outputMat;

// Normalize the input matrix to the range [0, 255]
double minVal, maxVal;
minMaxLoc(seg_mat, &minVal, &maxVal);
seg_mat.convertTo(outputMat, CV_8U, 255.0/(maxVal - minVal), -minVal);

// Apply the color map to the normalized matrix

applyColorMap(outputMat, segmentation_result, cv::COLORMAP_JET);

// Show the result
imshow("Output", segmentation_result);
waitKey(0);


}

void HW4::get_segment_areas(){


    // This loop fills map for segment vs area map
    for (int i = 0; i < seg_mat.rows; i++) {
        for (int j = 0; j < seg_mat.cols; j++) {
            int element = seg_mat.at<int>(i, j);
            segment_vs_area_map[element]++;
        }
    }

    for(int k=0;k<area_upper_thr.size();k++){

        int count = 0;
        // Iterate through the map and count the number of objects that have values between x and y
        for (const auto& [key, value] : segment_vs_area_map) {
            if (value >= area_lower_thr.at(k) && value <= area_upper_thr.at(k)) {
                //cout<<key<<" - "<<value<<endl;
                count++;
            }
        }
        areas_between_thresholds.push_back(count);
    }


    for(int k=0;k<areas_between_thresholds.size();k++){
        cout <<"Areas between "<< area_lower_thr.at(k)<<" - "<<area_upper_thr.at(k)<<" : " <<areas_between_thresholds[k] << endl;
    }


    seg_mat.copyTo(sift_segment);

    for (int i = 0; i < sift_segment.rows; i++) {
        for (int j = 0; j < sift_segment.cols; j++) {

            if(sift_segment.at<int>(i, j)!=sift_segment_label){
                sift_segment.at<int>(i, j)=0;
                
                
            }
            else{
                sift_segment.at<int>(i, j)=255;
            }
            
        }
    }

}

void HW4::show_interested_segment_areas(int segment_size){

    seg_mat.copyTo(interested_segment_area);

    for (int i = 0; i < seg_mat.rows; i++) {
        for (int j = 0; j < seg_mat.cols; j++) {

            if(interested_segment_area.at<int>(i, j)<area_lower_thr.at(segment_size) || interested_segment_area.at<int>(i, j)>area_upper_thr.at(segment_size)){
                interested_segment_area.at<int>(i, j)=0;
                
            }
            
        }
    }

    // Convert the input Mat to a suitable format for applying color map
    Mat converted;

    // Normalize the input matrix to the range [0, 255]
    double minVal, maxVal;
    minMaxLoc(interested_segment_area, &minVal, &maxVal);
    interested_segment_area.convertTo(converted, CV_8U, 255.0/(maxVal - minVal), -minVal);


    // Define a interested_segments
    applyColorMap(converted, interested_segments, COLORMAP_JET);

    // Set 0-valued pixels to black
    for (int i = 0; i < interested_segment_area.rows; i++) {
        for (int j = 0; j < interested_segment_area.cols; j++) {
            if (interested_segment_area.at<int>(i,j) == 0) {
                interested_segments.at<Vec3b>(i,j) = Vec3b(0, 0, 0);
            }
        }
    }

    // Display the resulting image
    namedWindow("Result", WINDOW_NORMAL);
    imshow("Result", interested_segments);
    waitKey(0);

}

//function that finds the SIFT features of a given segment
void HW4::findSIFTFeatures(const Mat& segment_input, const std::string& outputFileName) {
    Mat segment;
    segment_input.convertTo(segment, CV_8U);
    //cvtColor(segment_input, segment, COLOR_BGR2GRAY);
    // Create SIFT object
    Ptr<SIFT> sift = SIFT::create();

    // Detect keypoints and compute descriptors
    std::vector<KeyPoint> keypoints;
    Mat descriptors;
    sift->detectAndCompute(segment, noArray(), keypoints, descriptors);

    // Draw keypoints on the segment
    Mat outputImage;
    drawKeypoints(segment, keypoints, outputImage);

    // Show the image with keypoints
    imshow("Segment with SIFT keypoints", outputImage);
    waitKey(0);

    // Write the keypoints and descriptors to a text file
    std::ofstream outputFile(outputFileName);
    for (size_t i = 0; i < keypoints.size(); i++) {
        // Write keypoint coordinates and descriptor to file
        const KeyPoint& kp = keypoints[i];
        outputFile << kp.pt.x << " " << kp.pt.y << " ";
        for (int j = 0; j < descriptors.cols; j++) {
            outputFile << descriptors.at<float>(i, j) << " ";
        }
        outputFile << std::endl;
    }
    outputFile.close();
}