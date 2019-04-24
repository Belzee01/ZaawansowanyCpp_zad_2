//
// Created by Kajetan Lipensky on 2019-04-03.
//

#ifndef ZAD2_TASK_H
#define ZAD2_TASK_H

#include<iostream>
#include<cstdlib>
#include<ctime>
#include "Process.h"
#include <list>


class Task {
private:
    int weight{};

    int *costs{};
    int *times{};

    Process preferredProc;

private:

public:
    int id{};

public:
    Task() = default;

    Task(const Task& other) : weight(other.weight), costs(other.costs), times(other.times), id(other.id) {}

    virtual ~Task() {
//        delete[] costs;
//        delete[] times;
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

    void setPreferredProcess(Process &process) {
        Task::preferredProc = process;
    }

    const Process &getPreferredProc() const {
        return preferredProc;
    }

    bool operator()(const Task &lhs, const Task &rhs) const {
        return lhs.id > rhs.id;
    }

    bool operator<(const Task &rhs) const {
        return id < rhs.id;
    }
};

#endif //TEST_1_TASK_H
