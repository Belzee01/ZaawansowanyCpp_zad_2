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

public:
    int id{};

public:
    Task() = default;

    virtual ~Task() {
        delete[] costs;
        delete[] times;
    }

    void setId(int id) {
        Task::id = id;
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

    int *getCosts() const {
        return costs;
    }

    const std::list<Process> &getProc() const {
        return proc;
    }

    bool operator()(const Task &lhs, const Task &rhs) const {
        return lhs.id > rhs.id;
    }

    bool operator<(const Task &rhs) const {
        return id < rhs.id;
    }
};

#endif //TEST_1_TASK_H
