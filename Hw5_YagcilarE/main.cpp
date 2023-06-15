#include "hw5.h"


int main()
{
    HW5 hw5;
    
    /*  Part  A*/

    hw5.initialize();
    hw5.create_vocab();
    hw5.get_object_features();

    /*  Part B  */
    hw5.train_SVM();

    /* Part C   */
    vector<double> threshold = {0.5,0.75,0.9};
    hw5.test_bus(threshold[0]);
    hw5.test_truck(threshold[0]);

    hw5.test_bus(threshold[1]);
    hw5.test_truck(threshold[1]);

    hw5.test_bus(threshold[2]);
    hw5.test_truck(threshold[2]);


    for (int i = 0; i < threshold.size(); i++) {
        cout << "Threshold:\t" <<threshold[i] << endl;
        cout << "Bus:  \tRecall:"<<hw5.bus_recall[i]<<"\tPrecision:"<<hw5.bus_precision[i]<<endl;
        cout << "Truck:\tRecall:"<<hw5.truck_recall[i]<<"\tPrecision:"<<hw5.truck_precision[i]<<endl;
        cout << "\n-----***-----"<<endl;
    }

    hw5.draw_roc_bus();
    hw5.draw_roc_truck();

    return 0;
}
