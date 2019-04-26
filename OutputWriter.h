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

    void writeToFile(vector<OutputData> data);
};


#endif //ZAD2_OUTPUTWRITER_H
