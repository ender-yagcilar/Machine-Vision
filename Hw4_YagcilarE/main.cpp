#include "hw4.h"



int main(){

    HW4 hw4;
    hw4.initialize();
    hw4.segmentation();
    hw4.get_segment_areas();
    hw4.show_interested_segment_areas(1);
    hw4.findSIFTFeatures(hw4.sift_segment,"sift_features.txt");

return 0;
}