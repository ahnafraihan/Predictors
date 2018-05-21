/* Ahnaf Raihan
 * predictors
 * main.cpp
 */

#include "Predictor.hpp"
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
    if(argc != 3) {
        cerr << "Usage Error: Incorrect program arguments." << endl;
        cerr << "Usage: ./predictors input_trace.txt output.txt" << endl;
        exit(1);
    }
    
    string inputFile = argv[1];
    string outputFile = argv[2];
    
    Predictor p(inputFile);
    
    /*************************************************** ALWAYS TAKEN ***************************************************/
    
    p.alwaysTaken();
    
    /************************************************* ALWAYS NON-TAKEN *************************************************/
    
    p.alwaysNonTaken();
    
    /*********************************** BIMODAL PREDICTOR WITH A SINGLE BIT HISTORY ************************************/
    
    int size = 16;
    for (int i = 2; i < 9; i++) {
        if (i == 4)
            size = 128;
        p.bimodalSingleBit(size);
        size *= 2;
    }
    
    /******************************** BIMODAL PREDICTOR WITH 2-BIT SATURATING COUNTERS **********************************/
    
    size = 16;
    for (int i = 9; i < 16; i++) {
        if (i == 11)
            size = 128;
        p.bimodalDoubleBit(size);
        size *= 2;
    }
    
    /************************************************* GSHARE PREDICTOR *************************************************/
    
    for(int i = 3; i < 12; i++){
        p.gshare(i);
    }
    
    /************************************************ TOURNAMENT PREDICTOR **********************************************/
    
    p.tournament();
    
    /**************************************************** PRINT TO FILE *************************************************/
    
    p.print(outputFile);
    
}
