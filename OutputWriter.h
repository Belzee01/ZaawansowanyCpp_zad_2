//
// Created by Kajetan Lipensky on 2019-04-26.
//

#ifndef ZAD2_OUTPUTWRITER_H
#define ZAD2_OUTPUTWRITER_H

#include <iostream>
#include <fstream>
#include <vector>
#include "DecisionMaker.h"

using namespace std;

class OutputWriter {

public:
    OutputWriter();

    template <class C, class Tm>
    void writeToFile(vector<OutputData<C, Tm>> data);
};

template<class C, class Tm>
void OutputWriter::writeToFile(vector<OutputData<C, Tm>> data) {

    ofstream myfile;
    myfile.open("output");

    for (auto &i : data) {
        for (int j = 0; j < i.getTasksSortedByProcesses().size(); ++j) {
            myfile << "P" << j << ":";
            for (auto &task : i.getTasksSortedByProcesses()[j]) {
                myfile << "T" << task << ", ";
            }
            myfile << "\n";
        }
        myfile << "koszt: " << i.getCostAndTime().getCost() << "\n";
        myfile << "czas: " << i.getCostAndTime().getTime() << "\n";
        myfile << "\n";
    }


    myfile.close();
}

OutputWriter::OutputWriter() = default;

#endif //ZAD2_OUTPUTWRITER_H
