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

template<class T>
class TimesCostsMatrix {
private:
    int tasksSize{};
    int procSize{};

    T **tc_matrix{};

    int **indexArr;// i index represents task no. and value under j index represents index of the process
    // for which time * cost is smallest. This is 2d array, where for the second dimension values are process index
    // sorted by time * cost
    template<typename V>
    struct ValueIndex {
        V value;
        int index;

        ValueIndex(V value, int index) : value(value), index(index) {}
    };

public:
    TimesCostsMatrix() = default;

    TimesCostsMatrix(int tasksSize, int procSize);

    virtual ~TimesCostsMatrix();

    template<class C, class Tm>
    void evaluateMatrix(Task<C, Tm> *taskArr);

    /***
     * Requires evaluateMatrix(Task *taskArr) to be run firstly
     */
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

    int **getIndexArr() const;
};

template<class T>
TimesCostsMatrix<T>::~TimesCostsMatrix() {
    for (int i = 0; i < tasksSize; ++i) {
        delete[] this->tc_matrix[i];
        delete[] this->indexArr[i];
    }
    delete[] this->tc_matrix;
    delete[] this->indexArr;
}

template<class T>
template<class C, class Tm>
void TimesCostsMatrix<T>::evaluateMatrix(Task<C, Tm> *taskArr) {
    for (int i = 0; i < tasksSize; ++i) {
        for (int j = 0; j < procSize; ++j) {
            this->tc_matrix[i][j] = taskArr[i].getCosts()[j] * taskArr[i].getTimes()[j];
        }
    }
}

template<class T>
void TimesCostsMatrix<T>::evaluateIndexArray() {
    std::vector<ValueIndex<T>> tempHolder;
    for (int i = 0; i < tasksSize; ++i) {
        tempHolder.emplace_back(tc_matrix[i][0], 0);
        for (int j = 1; j < procSize; ++j) {
            for (auto iter = tempHolder.begin(); iter != tempHolder.end(); iter++) {
                if (tc_matrix[i][j] < (*iter).value) {
                    tempHolder.insert(tempHolder.begin(), ValueIndex<T>(tc_matrix[i][j], j));
                    break;
                } else if (iter == tempHolder.end() - 1) {
                    tempHolder.insert(tempHolder.end(), ValueIndex<T>(tc_matrix[i][j], j));
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

template<class T>
int **TimesCostsMatrix<T>::getIndexArr() const {
    return indexArr;
}

template<class T>
TimesCostsMatrix<T>::TimesCostsMatrix(int taskSize, int procSize) {
    this->tasksSize = tasksSize, this->procSize = procSize;
    this->tc_matrix = new T *[tasksSize];
    this->indexArr = new int *[tasksSize];
    for (int i = 0; i < tasksSize; ++i) {
        this->tc_matrix[i] = new T[procSize];
        this->indexArr[i] = new int[procSize];
    }

}


#endif //ZAD2_TIMESCOSTSMATRIX_H
