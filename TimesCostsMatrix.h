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
#include "TasksContainer.h"

using namespace std;

class TimesCostsMatrix {
private:
    int tasksSize{};
    int procSize{};


    int **indexArr;// i index represents task no. and value under j index represents index of the process
    // for which time * cost is smallest. This is 2d array, where for the second dimension values are process index
    // sorted by time * cost
    template<typename V>
    struct ValueIndex {
        V value;
        int index;

        ValueIndex(V value, int index) : value(value), index(index) {}
    };


    template<class C, class Tm>
    C** evaluateMatrix(Task<C, Tm> *taskArr);

public:
    TimesCostsMatrix() = default;

    template<class Com, class C, class Tm>
    TimesCostsMatrix(TasksContainer<Com, C, Tm> container);

    virtual ~TimesCostsMatrix();

    template<class C, class Tm>
    void evaluateIndexArray(Task<C, Tm> *taskArr);

    void printOutIndexMatrix() {
        for (int i = 0; i < tasksSize; ++i) {
            for (int j = 0; j < procSize; ++j) {
                cout << this->indexArr[i][j] << " ";
            }
            cout << endl;
        }
    }

    int **getIndexArr() const;

    vector<vector<float>> tc_mat;
};

TimesCostsMatrix::~TimesCostsMatrix() {
    for (int i = 0; i < tasksSize; ++i) {
        delete[] this->indexArr[i];
    }
    delete[] this->indexArr;
}

template<class C, class Tm>
C** TimesCostsMatrix::evaluateMatrix(Task<C, Tm> *taskArr) {
    C** tc_matrix = new C *[tasksSize];
    for (int i = 0; i < tasksSize; ++i) {
        tc_matrix[i] = new C[procSize];
        tc_mat.emplace_back();
    }

    for (int i = 0; i < tasksSize; ++i) {
        for (int j = 0; j < procSize; ++j) {
            tc_matrix[i][j] = taskArr[i].getCosts()[j] * taskArr[i].getTimes()[j];
            tc_mat[i].push_back(float(tc_matrix[i][j]));
        }
    }
    return tc_matrix;
}

template<class C, class Tm>
void TimesCostsMatrix::evaluateIndexArray(Task<C, Tm> *taskArr) {
    C** tc_matrix = evaluateMatrix(taskArr);
    std::vector<ValueIndex<C>> tempHolder;
    for (int i = 0; i < tasksSize; ++i) {
        tempHolder.emplace_back(tc_matrix[i][0], 0);
        for (int j = 1; j < procSize; ++j) {
            for (auto iter = tempHolder.begin(); iter != tempHolder.end(); iter++) {
                if (tc_matrix[i][j] < (*iter).value) {
                    tempHolder.insert(tempHolder.begin(), ValueIndex<C>(tc_matrix[i][j], j));
                    break;
                } else if (iter == tempHolder.end() - 1) {
                    tempHolder.insert(tempHolder.end(), ValueIndex<C>(tc_matrix[i][j], j));
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

int **TimesCostsMatrix::getIndexArr() const {
    return indexArr;
}

template<class Com, class C, class Tm>
TimesCostsMatrix::TimesCostsMatrix(TasksContainer<Com, C, Tm> container) {
    this->tasksSize = container.getTasksSize(), this->procSize = container.getProcSize();
    this->indexArr = new int *[tasksSize];
    for (int i = 0; i < tasksSize; ++i) {
        this->indexArr[i] = new int[procSize];
    }

}


#endif //ZAD2_TIMESCOSTSMATRIX_H
