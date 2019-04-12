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


class Task {
private:
    int weight{};

    std::list<Process> proc;
    int *costs{};
    int *times{};

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
    int id{};

public:
    Task() = default;

    Task(int id) : id(id) {
        this->weight = 0;
    }

    Task(int id, const std::list<Process> &proc) : proc(proc), id(id) {
        this->costs = new int[proc.size()];
        this->times = new int[proc.size()];
        this->weight = 0;
        this->randomizeTimeAndCost();
    }

    void setId(int id) {
        Task::id = id;
    }

    void setWeight(int weight) {
        Task::weight = weight;
    }

    void setProc(const std::list<Process> &proc) {
        Task::proc = proc;
    }

    void setTimes(int *times) {
        Task::times = times;
    }

    void setCosts(int *costs) {
        Task::costs = costs;
    }

    int getId() const {
        return id;
    }

    int *getTimes() const {
        return times;
    }

    int getWeight() const {
        return weight;
    }

    int *getCosts() const {
        return costs;
    }

    bool operator()(const Task &lhs, const Task &rhs) const {
        return lhs.id > rhs.id;
    }

    bool operator<(const Task &rhs) const {
        return id < rhs.id;
    }
};

#endif //TEST_1_TASK_H
