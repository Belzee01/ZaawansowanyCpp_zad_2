//
// Created by Kajetan Lipensky on 2019-04-26.
//

#include "OutputWriter.h"

OutputWriter::OutputWriter() = default;

void OutputWriter::writeToFile(vector<Path> data) {

    ofstream myfile;
    myfile.open("output");

    for (auto &i : data) {
        for (int j = 0; j < i.size(); ++j) {
            myfile << "P" << j << ":";
            for (auto &task : i[j]) {
                myfile << "T" << task << ", ";
            }
            myfile << "\n";
        }
        myfile << "\n";
    }
    myfile << "koszt: " << costAndTime.getCost() << "\n";
    myfile << "czas: " << costAndTime.getTime() << "\n";


    myfile.close();
}
