//
// Created by Belzee on 17.04.2019.
//

#ifndef ZAD2_TIMESCOSTSMATRIX_H
#define ZAD2_TIMESCOSTSMATRIX_H

#include <map>
#include <list>
#include "Task.h"
#include "Process.h"
#include <iostream>
#include <algorithm>

using namespace std;

class TimesCostsMatrix {
private:
    int tasksSize{};
    int procSize{};

    int **tc_matrix{};

    int **indexArr;// i index represents task no. and value under j index represents index of the process
    // for which time * cost is smallest. This is 2d array, where for the second dimension values are process index
    // sorted by time * cost

    struct ValueIndex {
        int value;
        int index;

        ValueIndex(int value, int index) : value(value), index(index) {}
    };

public:
    TimesCostsMatrix() = default;

    TimesCostsMatrix(int tasksSize, int procSize);

    virtual ~TimesCostsMatrix();

    void evaluateMatrix(Task *taskArr);

    void evaluateIndexArray();

    void printOutMatrix() {
        for (int i = 0; i < tasksSize; ++i) {
            for (int j = 0; j < procSize; ++j) {
                cout << this->tc_matrix[i][j] << " ";
            }
            cout << endl;
        }
    }

    void printOutIndexMatrix() {
        for (int i = 0; i < tasksSize; ++i) {
            for (int j = 0; j < procSize; ++j) {
                cout << this->indexArr[i][j] << " ";
            }
            cout << endl;
        }
    }
};


#endif //ZAD2_TIMESCOSTSMATRIX_H
