//
// Created by Kajetan Lipensky on 2019-04-26.
//

#include "OutputWriter.h"

OutputWriter::OutputWriter() = default;

void OutputWriter::writeToFile(vector<vector<TaskIdProcessId>> paths) {
    ofstream myfile;
    myfile.open ("output");
    myfile << "Writing this to a file.\n";
    myfile.close();
}
