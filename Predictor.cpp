/* Ahnaf Raihan
 * predictors
 * Predictor.cpp
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include "Predictor.hpp"

using namespace std;

Predictor::Predictor(string file) {
    string address, value;
    numBranches = 0;
    ifstream inf(file);
    
    if(inf.fail()) {
        cerr << "Error: Cannot find input file: " << file << endl;
        exit(1);
    }
    
    while (inf >> address >> value) {
        numBranches++;
        branch b;
        
        address = address.substr(2);
        stringstream stream;
        stream << address;
        stream >> hex >> b.address;
        
        if (value == "T")
            b.value = true;
        else if (value == "NT")
            b.value = false;
        else {
            cerr << "Error: Undefined branch value: " << value << endl;
            exit(1);
        }
        branches.push_back(b);
    }
    inf.close();
}

void Predictor::alwaysTaken() {
    result r;
    r.name = "Always Taken";
    r.numCorrect = 0;
    
    for (int i = 0; i < branches.size(); i++) {
        if (branches[i].value == true)
            r.numCorrect++;
    }
    results.push_back(r);
}

void Predictor::alwaysNonTaken() {
    result r;
    r.name = "Always Non-Taken";
    r.numCorrect = 0;
    
    for (int i = 0; i < branches.size(); i++) {
        if (branches[i].value == false)
            r.numCorrect++;
    }
    results.push_back(r);
}

void Predictor::bimodalSingleBit(int size) {
    result r;
    r.name = "Bimodal Single Bit " + to_string(size);
    r.numCorrect = 0;
    int history[size];
    
    for (int i = 0; i < size; i++)
        history[i] = true;
    
    for (int i = 0; i < branches.size(); i++) {
        int j = branches[i].address % size;
        if (history[j] == true && branches[i].value == true)
            r.numCorrect++;
        else if (history[j] == true && branches[i].value == false)
            history[j]--;
        else if (history[j] == false && branches[i].value == false)
            r.numCorrect++;
        else
            history[j]++;
    }
    results.push_back(r);
}

void Predictor::bimodalDoubleBit(int size) {
    result r;
    r.name = "Bimodal Double Bit " + to_string(size);
    r.numCorrect = 0;
    int history[size];
    
    for (int i = 0; i < size; i++)
        history[i] = 3;
    
    for (int i = 0; i < branches.size(); i++) {
        int j = branches[i].address % size;
        if (history[j] > 1 && branches[i].value == 1) {
            r.numCorrect++;
            if(history[j] != 3)
                history[j]++;
        }
        else if (history[j] < 2 && branches[i].value == 0) {
            r.numCorrect++;
            if(history[j] != 0)
                history[j]--;
        }
        else if (history[j] < 2 && branches[i].value == 1) {
            history[j]++;
        }
        else {
            history[j]--;
        }
    }
    results.push_back(r);
}

void Predictor::gshare(int bits) {
    result r;
    r.name = "Gshare " + to_string(bits);
    r.numCorrect = 0;
    int history[2048];
    
    int ghr = 0;
    int size = pow(2, bits) - 1;
    
    for (int i = 0; i < 2048; i++)
        history[i] = 3;
    
    for (int i = 0; i < branches.size(); i++) {
        int j = (branches[i].address ^ (ghr & size)) % 2048;
        if (history[j] > 1 && branches[i].value == 1) {
            r.numCorrect++;
            if(history[j] != 3)
                history[j]++;
        }
        else if (history[j] < 2 && branches[i].value == 0) {
            r.numCorrect++;
            if(history[j] != 0)
                history[j]--;
        }
        else if (history[j] < 2 && branches[i].value == 1) {
            history[j]++;
        }
        else {
            history[j]--;
        }
        
        ghr <<= 1;
        ghr += branches[i].value;
        ghr &= size;
    }
    results.push_back(r);
}

void Predictor::tournament() {
    result r;
    r.name = "Tournament " + to_string(11);;
    int bHistory[2048], gHistory[2048], tHistory[2048];
    
    int ghr = 0;
    int size = pow(2, 11) - 1;
    
    for (int i = 0; i < 2048; i++) {
        bHistory[i] = 3;
        gHistory[i] = 3;
        tHistory[i] = 0;
    }
    
    for (int i = 0; i < branches.size(); i++) {
        int bJ = (branches[i].address % 2048);
        int b;
        
        if(bHistory[bJ] > 1)
            b = 1;
        else
            b = 0;
        
        if (bHistory[bJ] > 1 && branches[i].value == 1) {
            if(bHistory[bJ] != 3)
                bHistory[bJ]++;
        }
        else if (bHistory[bJ] < 2 && branches[i].value == 0) {
            if(bHistory[bJ] != 0)
                bHistory[bJ]--;
        }
        else if (bHistory[bJ] < 2 && branches[i].value == 1) {
            bHistory[bJ]++;
        }
        else {
            bHistory[bJ]--;
        }
        
        int gJ = (ghr ^ branches[i].address) % 2048;
        int g;
        
        if (gHistory[gJ] > 1)
            g = 1;
        else
            g = 0;
        
        if (gHistory[gJ] > 1 && branches[i].value == 1) {
            if(gHistory[gJ] != 3)
                gHistory[gJ]++;
        }
        else if (gHistory[gJ] < 2 && branches[i].value == 0) {
            if(gHistory[gJ] != 0)
                gHistory[gJ]--;
        }
        else if (gHistory[gJ] < 2 && branches[i].value == 1) {
            gHistory[gJ]++;
        }
        else
            gHistory[gJ]--;
        
        ghr <<= 1;
        ghr += branches[i].value;
        ghr &= 2047;
        
        if(tHistory[bJ] < 2) {
            if (g == branches[i].value)
                r.numCorrect++;
        }
        else {
            if (b == branches[i].value)
                r.numCorrect++;
        }
        if (b == branches[i].value && g != b) {
            if (tHistory[bJ] < 3)
                tHistory[bJ]++;
        }
        else if (g == (int)branches[i].value && b != g) {
            if (tHistory[bJ] > 0)
                tHistory[bJ]--;
        }
        
    }
    results.push_back(r);
}

void Predictor::print(string file) {
    ofstream ofs(file);
    
    if(ofs.fail()) {
        cerr << "Error: Invalid output file: " << file << endl;
        exit(1);
    }
    
    ofs << results.at(0).numCorrect << "," << numBranches << ";" << endl;
    ofs << results.at(1).numCorrect << "," << numBranches << ";" << endl;
    for (int i = 2; i < 9; i++) {
        if (i == 8)
            ofs << results.at(i).numCorrect << "," << numBranches << ";";
        else
            ofs << results.at(i).numCorrect << "," << numBranches << "; ";
    }
    ofs << endl;
    for (int i = 9; i < 16; i++) {
        if (i == 15)
            ofs << results.at(i).numCorrect << "," << numBranches << ";";
        else
            ofs << results.at(i).numCorrect << "," << numBranches << "; ";
    }
    ofs << endl;
    for (int i = 16; i < 25; i++) {
        if (i == 24)
            ofs << results.at(i).numCorrect << "," << numBranches << ";";
        else
            ofs << results.at(i).numCorrect << "," << numBranches << "; ";
    }
    ofs << endl;
    ofs << results.at(25).numCorrect << "," << numBranches << ";" << endl;
    ofs.close();
}
