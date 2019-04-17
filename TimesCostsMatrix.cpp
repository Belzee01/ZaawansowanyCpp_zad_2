//
// Created by Belzee on 17.04.2019.
//

#include <vector>
#include "TimesCostsMatrix.h"

TimesCostsMatrix::TimesCostsMatrix(int tasksSize, int procSize) : tasksSize(tasksSize), procSize(procSize) {
    this->tc_matrix = new int *[tasksSize];
    this->indexArr = new int *[tasksSize];
    for (int i = 0; i < tasksSize; ++i) {
        this->tc_matrix[i] = new int[procSize];
        this->indexArr[i] = new int[procSize];
    }
}

void TimesCostsMatrix::evaluateMatrix(Task *taskArr) {

    for (int i = 0; i < tasksSize; ++i) {
        for (int j = 0; j < procSize; ++j) {
            this->tc_matrix[i][j] = taskArr[i].getCosts()[j] * taskArr[i].getTimes()[j];
        }
    }

}

TimesCostsMatrix::~TimesCostsMatrix() {
    for (int i = 0; i < tasksSize; ++i) {
        delete[] this->tc_matrix[i];
        delete[] this->indexArr[i];
    }
    delete[] this->tc_matrix;
    delete[] this->indexArr;
}

void TimesCostsMatrix::evaluateIndexArray() {

    std::vector<ValueIndex> tempHolder;
    for (int i = 0; i < tasksSize; ++i) {
        tempHolder.emplace_back(tc_matrix[i][0], 0);
        for (int j = 1; j < procSize; ++j) {
            for (auto iter = tempHolder.begin(); iter != tempHolder.end(); iter++) {
                if (tc_matrix[i][j] < (*iter).value) {
                    tempHolder.insert(tempHolder.begin(), ValueIndex(tc_matrix[i][j], j));
                    break;
                } else if (iter == tempHolder.end() - 1) {
                    tempHolder.insert(tempHolder.end(), ValueIndex(tc_matrix[i][j], j));
                    break;
                }
            }
        }

        for (int k = 0; k < tempHolder.size(); ++k) {
            this->indexArr[i][k] = tempHolder[k].index;
        }
        tempHolder.clear();
    }
}

