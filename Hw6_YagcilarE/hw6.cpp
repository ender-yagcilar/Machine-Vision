#include "hw6.h"

void drawVectorField(const Mat& img, const std::vector<Point2f>& points1, const std::vector<Point2f>& points2) {
    Mat output;
    cvtColor(img, output, COLOR_GRAY2BGR);

    for (size_t i = 0; i < points1.size(); ++i) {
        Point2f p1 = points1[i];
        Point2f p2 = points2[i];

        // Draw a line connecting the matched points
        line(output, p1, p2, Scalar(0, 255, 0), 1);
        
        // Draw a circle around the points
        circle(output, p1, 2, Scalar(0, 0, 255), 1);
        circle(output, p2, 2, Scalar(0, 0, 255), 1);
    }

    imshow("Vector Field", output);
    waitKey(0);
}

void HW6::select_dataset(){

/*              Dataset Selection: Human or Car     */
    std::string selection;

    // Prompt the user for input
    std::cout << "Select either 'car' or 'human': ";
    std::cin >> selection;

    // Check the user's selection
    while(true){
        if (selection == "car") {
            std::cout << "Car Dataset is selected. Calculations will be done on car dataset" << std::endl;
            framesDir = car_path;
            break;
        } else if (selection == "human") {
            std::cout << "human Dataset is selected. Calculations will be done on human dataset" << std::endl;
            framesDir = human_path;
            break;
        } else {
            std::cout << "Invalid selection. Select again" << std::endl;
        }
    }

    /*      Getting Image count in that folder      */


    
    std::regex numberRegex(R"(\d+)"); // Regex pattern to extract numbers from filenames

    DIR* dir;
    struct dirent* entry;

    dir = opendir(framesDir.c_str());
    if (dir == NULL) {
        std::cout << "Error opening directory." << std::endl;
        
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) { // Check if it's a regular file
            std::string filename = entry->d_name;

            // Extract the number from the filename using regex
            std::smatch match;
            if (std::regex_search(filename, match, numberRegex)) {
                int number = std::stoi(match[0]);
                if (number > num_of_frames) {
                    num_of_frames = number;
                }
            }
        }
    }

    closedir(dir);

}


void HW6::PartA(){
    std::cout<<"Part A starts"<<std::endl;
    // Read the first frame
    Mat framePrev = imread(framesDir + "0001.jpg", IMREAD_GRAYSCALE);

    // Create a blank image for visualization
    Mat flowVis(framePrev.size(), CV_8UC3, Scalar(0, 0, 0));

    // Take user input for showing optical flow either on the image or on a blank image
    std::string userInput = "blank";
    std::cout << "Do you want to see t optical \nSelect either 'blank' or 'image': ";
    std::cin >> userInput;
    
    for (int frameIndex = 2;frameIndex<num_of_frames; frameIndex++) {
        // Read the current frame
        std::string currentFramePath;
        if(frameIndex<10){
            currentFramePath = framesDir +"000" + std::to_string(frameIndex) + ".jpg";
        }
        else if(frameIndex<100){
            currentFramePath = framesDir +"00" + std::to_string(frameIndex) + ".jpg";
        }
        else if(frameIndex<1000){
            currentFramePath = framesDir +"0" + std::to_string(frameIndex) + ".jpg";
        }
        else{
            currentFramePath = framesDir  + std::to_string(frameIndex) + ".jpg";
        }
            
        
        Mat frameCurr = imread(currentFramePath, IMREAD_GRAYSCALE);
        if (frameCurr.empty()){
            printf("frameCurr empty");
            break;
        }
            

        // Compute dense optical flow
        Mat flow;
        calcOpticalFlowFarneback(framePrev, frameCurr, flow, 0.5, 3, 15, 3, 5, 1.2, 0);

        // Visualize the flow vectors
        if (userInput == "image") {
            // Draw flow vectors on the current frame
            for (int y = 0; y < flow.rows; y += 10) {
                for (int x = 0; x < flow.cols; x += 10) {
                    const Point2f& flowVec = flow.at<Point2f>(y, x);
                    line(frameCurr, Point(x, y), Point(cvRound(x + flowVec.x), cvRound(y + flowVec.y)), Scalar(0, 255, 0));
                }
            }

            // Display the frame with optical flow
            imshow("Optical Flow", frameCurr);
        } else if (userInput == "blank") {
            // Draw flow vectors on the blank image
            flowVis.setTo(Scalar(0, 0, 0));
            for (int y = 0; y < flow.rows; y += 10) {
                for (int x = 0; x < flow.cols; x += 10) {
                    const Point2f& flowVec = flow.at<Point2f>(y, x);
                    line(flowVis, Point(x, y), Point(cvRound(x + flowVec.x), cvRound(y + flowVec.y)), Scalar(0, 255, 0));
                }
            }

            // Display the blank image with optical flow
            imshow("Optical Flow", flowVis);
        }

        // Wait for a key press
        if (waitKey(0) == 27)  // Press ESC to exit
            break;

        // Update the previous frame
        framePrev = frameCurr.clone();
    }

    // Close the window
    destroyAllWindows();

}

void HW6::PartB(){
    int userInput;
    std::cout << "Display vector field on image (1) or on a blank image (2)? ";
    std::cin >> userInput;


     // Variables
    Ptr<Feature2D> sift = SIFT::create();
    std::vector<KeyPoint> prevKeypoints, currKeypoints;
    Mat prevDescriptors, currDescriptors;

    Mat prevFrame = imread(framesDir + "0001.jpg", IMREAD_GRAYSCALE);
    Mat currFrame;

    for (int frameIndex = 2;frameIndex<num_of_frames; frameIndex++) {
        // Read the current frame
        std::string currentFramePath;
        if(frameIndex<10){
            currentFramePath = framesDir +"000" + std::to_string(frameIndex) + ".jpg";
        }
        else if(frameIndex<100){
            currentFramePath = framesDir +"00" + std::to_string(frameIndex) + ".jpg";
        }
        else if(frameIndex<1000){
            currentFramePath = framesDir +"0" + std::to_string(frameIndex) + ".jpg";
        }
        else{
            currentFramePath = framesDir  + std::to_string(frameIndex) + ".jpg";
        }

    currFrame = imread(currentFramePath);
    // Detect SIFT features in the frames
    sift->detectAndCompute(prevFrame, noArray(), prevKeypoints, prevDescriptors);
    sift->detectAndCompute(currFrame, noArray(), currKeypoints, currDescriptors);

        Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);
        std::vector<DMatch> matches;
        matcher->match(prevDescriptors, currDescriptors, matches);

        std::vector<Point2f> matchedPoints1, matchedPoints2;
        for (const DMatch& match : matches)
        {
            if (match.distance < 100 && match.distance > 70) 
            {   
                matchedPoints1.push_back(prevKeypoints[match.queryIdx].pt);
                matchedPoints2.push_back(currKeypoints[match.trainIdx].pt);
            }
        }

        Mat background;
        if (userInput == 2)
            background = Mat::zeros(prevFrame.size(), CV_8UC1);
        else if (userInput == 1)
            background = prevFrame;

        // Draw the matched features and their corresponding vectors
        for (int i = 0; i < matchedPoints1.size(); i++)
        {
            // Get the matched points and flow vector
            Point2f prevPoint = matchedPoints1[i];
            Point2f currPoint = matchedPoints2[i];

            // Draw the feature point and flow vector on the frame
            circle(background, prevPoint, 2, Scalar(255, 0, 0), 1); 
            circle(background, currPoint, 2, Scalar(0, 0, 255), 1); 
            arrowedLine(background, prevPoint, currPoint, Scalar(255, 255, 255), 1, 8, 0, 1);
        }

        // Display the resulting image with features and vector field
        namedWindow("Part B", WINDOW_NORMAL);
        imshow("Part B", background);
        waitKey(0);

        prevFrame = currFrame;
    }
}
