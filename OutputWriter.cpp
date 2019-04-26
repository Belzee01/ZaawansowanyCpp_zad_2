//
// Created by Kajetan Lipensky on 2019-04-26.
//

#include "OutputWriter.h"

OutputWriter::OutputWriter() = default;

void OutputWriter::writeToFile(vector<OutputData> data) {

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
