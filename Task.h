//
// Created by Kajetan Lipensky on 2019-04-03.
//

#ifndef TEST_1_TASK_H
#define TEST_1_TASK_H

#include<iostream>
#include<cstdlib>
#include<ctime>
#include "Process.h"
#include <list>

template<typename T=int>
class Task {
private:
    int id;
    T weight;

    std::list<Process> proc;
    T *times;
    T *costs;

private:
    void randomizeTimeAndCost() {
        auto it = proc.begin();

        for (int i = 0; i < this->proc.size(); ++i) {
            if (it->getTypeOfProcess() == 0) {
                this->times[i] = rand() % (Process::getDedicatedTimeMax() - Process::getDedicatedTimeMin()) +
                                 Process::getDedicatedTimeMin();
                this->costs[i] = rand() % (Process::getDedicatedCostMax() - Process::getDedicatedCostMin()) +
                                 Process::getDedicatedCostMin();
            } else {
                this->times[i] = rand() % (Process::getUniversalTimeMax() - Process::getUniversalTimeMin()) +
                                 Process::getUniversalTimeMin();
                this->costs[i] = rand() % (Process::getUniversalCostMax() - Process::getUniversalCostMin()) +
                                 Process::getUniversalCostMin();
            }
            it++;
        }
    }

public:

    Task(int id);

    Task(int id, const std::list<Process> &proc) : proc(proc), id(id) {
        this->costs = new T[proc.size()];
        this->times = new T[proc.size()];
        this->weight = 0;
        this->randomizeTimeAndCost();
    }

    int getId() const {
        return id;
    }

    T getWeight() const {
        return weight;
    }

    int getProc() const {
        return proc;
    }

    T *getTimes() const {
        return times;
    }

    T *getCosts() const {
        return costs;
    }
};


#endif //TEST_1_TASK_H
