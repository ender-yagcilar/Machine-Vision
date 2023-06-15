#include "hw5.h"

void HW5::initialize(){
    
    cv::glob(bus_img_file_path, bus_imgs_paths);
    cv::glob(truck_img_file_path, truck_imgs_paths);

}

void HW5::create_vocab(){
    tc = TermCriteria(TermCriteria::MAX_ITER, 100, 0.001);
    BOWKMeansTrainer trainer(VOCAB_SIZE, tc, KMEANS_PP_CENTERS);
    

        for (int i = 0; i < 100; i++) {
        Mat image = imread(bus_imgs_paths[i], IMREAD_GRAYSCALE);
        std::vector<KeyPoint> keypoints;
        detector->detect(image, keypoints);
        Mat descriptors, imnew;
        extractor->compute(image, keypoints, descriptors);
        trainer.add(descriptors);
        drawKeypoints(image, keypoints, imnew);
    }
    for (int i = 0; i < 100; i++) {
        Mat image = imread(truck_imgs_paths[i], IMREAD_GRAYSCALE);
        std::vector<KeyPoint> keypoints;
        detector->detect(image, keypoints);
        Mat descriptors;
        extractor->compute(image, keypoints, descriptors);
        trainer.add(descriptors);
    }
    vocabulary = trainer.cluster();

    matcher = DescriptorMatcher::create("FlannBased");
    bowExtractor = new BOWImgDescriptorExtractor(extractor, matcher);
    bowExtractor->setVocabulary(vocabulary);
}

void HW5::get_object_features(){
    
    for (int i = 0; i < 100; i++) {
        Mat image = imread(bus_imgs_paths[i], IMREAD_GRAYSCALE);
        std::vector<KeyPoint> keypoints;
        detector->detect(image, keypoints);
        Mat bowFeatures;
        bowExtractor->compute(image, keypoints, bowFeatures);
        bus_features.push_back(bowFeatures);
    }


    for (int i = 0; i < 100; i++) {
        Mat image = imread(truck_imgs_paths[i], IMREAD_GRAYSCALE);
        std::vector<KeyPoint> keypoints;
        detector->detect(image, keypoints);
        Mat bowFeatures;
        bowExtractor->compute(image, keypoints, bowFeatures);
        truck_features.push_back(bowFeatures);
    }
}

void HW5::train_SVM(){

    /*          Train Bus SVM      */
    Mat trainingDataBus;
    vconcat(bus_features, truck_features, trainingDataBus);
    Mat labels_bus(trainingDataBus.rows, 1, CV_32SC1);
    labels_bus.rowRange(0, bus_features.rows) = Scalar(1);
    labels_bus.rowRange(bus_features.rows, labels_bus.rows) = Scalar(-1);
        // Train the SVM
    svm_bus = SVM::create();
    svm_bus->setType(SVM::C_SVC);
    svm_bus->setKernel(SVM::LINEAR);
    svm_bus->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 200, 1e-6));

    svm_bus->train(trainingDataBus, ROW_SAMPLE, labels_bus);

        /*          Train Truck SVM      */
    Mat trainingDataTruck;
    vconcat(bus_features, truck_features, trainingDataTruck);
    Mat labels_truck(trainingDataTruck.rows, 1, CV_32SC1);
    labels_truck.rowRange(0, bus_features.rows) = Scalar(-1);
    labels_truck.rowRange(bus_features.rows, labels_truck.rows) = Scalar(1);
        // Train the SVM
    svm_truck = SVM::create();
    svm_truck->setType(SVM::C_SVC);
    svm_truck->setKernel(SVM::LINEAR);
    svm_truck->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 200, 1e-6));

    svm_truck->train(trainingDataTruck, ROW_SAMPLE, labels_truck);
}

void HW5::test_bus(double threshold){

    /*True positive - false negative*/
    for (int i = 0; i < bus_imgs_paths.size(); i++) {
        
        Mat image = imread(bus_imgs_paths[i], IMREAD_GRAYSCALE);
        
        std::vector<KeyPoint> keypoints;
        detector->detect(image, keypoints);
        Mat bowFeatures;
        bowExtractor->compute(image, keypoints, bowFeatures);
        
        double svm_output = abs(svm_bus->predict(bowFeatures,noArray(), StatModel::RAW_OUTPUT));
        
        int is_positive = (svm_output >= threshold) ? 1 : -1;
        
        if (is_positive == 1) {
            bus.true_pos++;
        } else {
            bus.false_neg++;
        }
    }
    /*False Positive - true negative*/
    for (int i = 0; i < truck_imgs_paths.size(); i++) {
        Mat image = imread(truck_imgs_paths[i], IMREAD_GRAYSCALE);
        
        std::vector<KeyPoint> keypoints;
        detector->detect(image, keypoints);
        Mat bowFeatures;
        bowExtractor->compute(image, keypoints, bowFeatures);
        
        double svm_output = abs(svm_bus->predict(bowFeatures,noArray(), StatModel::RAW_OUTPUT));
        
        int is_positive = (svm_output >= threshold) ? 1 : -1;
        
        if (is_positive == 1) {
            bus.false_pos++;
        } else {
            bus.true_neg++;
        }
    }
    /*Recall - Precision Calculation*/
    bus_recall.push_back(bus.true_pos / (bus.true_pos + bus.false_neg));
    bus_precision.push_back(bus.true_pos / (bus.true_pos + bus.false_pos));
    bus_fpr.push_back(bus.false_pos / (bus.false_pos + bus.true_neg));

}


void HW5::test_truck(double threshold){

    /*True positive - false negative*/
    for (int i = 0; i < truck_imgs_paths.size(); i++) {

        Mat image = imread(truck_imgs_paths[i], IMREAD_GRAYSCALE);
        
        std::vector<KeyPoint> keypoints;
        detector->detect(image, keypoints);
        Mat bowFeatures;
        bowExtractor->compute(image, keypoints, bowFeatures);
        
        double svm_output = abs(svm_truck->predict(bowFeatures,noArray(), StatModel::RAW_OUTPUT));
        
        int is_positive = (svm_output >= threshold) ? 1 : -1;
        
        if (is_positive == 1) {
            truck.true_pos++;
        } else {
            truck.false_neg++;
        }
    }
    /*False Positive - true negative*/
    for (int i = 0; i < bus_imgs_paths.size(); i++) {
        Mat image = imread(bus_imgs_paths[i], IMREAD_GRAYSCALE);
        
        std::vector<KeyPoint> keypoints;
        detector->detect(image, keypoints);
        Mat bowFeatures;
        bowExtractor->compute(image, keypoints, bowFeatures);
        
        double svm_output = abs(svm_truck->predict(bowFeatures,noArray(), StatModel::RAW_OUTPUT));
        int is_positive = (svm_output >= threshold) ? 1 : -1;
        
        if (is_positive == 1) {
            truck.false_pos++;
        } else {
            truck.true_neg++;
        }
    }

    /*Recall - Precision Calculation*/

    truck_recall.push_back(truck.true_pos / (truck.true_pos + truck.false_neg));
    truck_precision.push_back(truck.true_pos / (truck.true_pos + truck.false_pos));
    truck_fpr.push_back(truck.false_pos / (truck.false_pos + truck.true_neg));
}

void HW5::draw_roc_bus(){

    // create a new plot window
    namedWindow("ROC Curve:Bus", WINDOW_NORMAL);

    // create a Mat object to store the plot
    Mat plot(500, 500, CV_8UC3, Scalar(255, 255, 255));
    
    // draw the axes
    line(plot, Point(50, 450), Point(450, 450), Scalar(0, 0, 0), 2, LINE_AA);
    line(plot, Point(50, 450), Point(50, 50), Scalar(0, 0, 0), 2, LINE_AA);
    
    // draw the ROC curve
    for (int i = 0; i < bus_recall.size() - 1; i++) {
        Point p1(50 + bus_recall[i] * 400, 450 - bus_fpr[i] * 400);
        Point p2(50 + bus_recall[i+1] * 400, 450 - bus_fpr[i+1] * 400);
        line(plot, p1, p2, Scalar(255, 0, 0), 2, LINE_AA);
    }

    // show the plot
    imshow("ROC Curve:Bus", plot);
    waitKey(0);

}


void HW5::draw_roc_truck(){

    // create a new plot window
    namedWindow("ROC Curve:Truck", WINDOW_NORMAL);

    // create a Mat object to store the plot
    Mat plot(500, 500, CV_8UC3, Scalar(255, 255, 255));
    
    // draw the axes
    line(plot, Point(50, 450), Point(450, 450), Scalar(0, 0, 0), 2, LINE_AA);
    line(plot, Point(50, 450), Point(50, 50), Scalar(0, 0, 0), 2, LINE_AA);
    
    // draw the ROC curve
    for (int i = 0; i < truck_recall.size() - 1; i++) {
        Point p1(50 + truck_recall[i] * 400, 450 - truck_fpr[i] * 400);
        Point p2(50 + truck_recall[i+1] * 400, 450 - truck_fpr[i+1] * 400);
        line(plot, p1, p2, Scalar(255, 0, 0), 2, LINE_AA);
    }

    // show the plot
    imshow("ROC Curve:Truck", plot);
    waitKey(0);

}


