/* Ahnaf Raihan
 * predictors
 * Predictor.hpp
 */

#include <iostream>
#include <vector>

using namespace std;

struct branch {
    unsigned long long address;
    bool value;
};
struct result {
    string name;
    unsigned long long numCorrect;
};

class Predictor {
private:
    int numBranches;
    vector<branch> branches;
    vector<result> results;
    
public:
    Predictor(string inputFile);
    void alwaysTaken();
    void alwaysNonTaken();
    void bimodalSingleBit(int size);
    void bimodalDoubleBit(int size);
    void gshare(int bits);
    void tournament();
    void print(string file);
};
