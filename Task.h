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

template<class C, class T, class W=int>
class Task {
private:
    W weight{};

    C *costs{};
    T *times{};

private:

public:
    int id{};

public:
    Task() = default;

    Task(const Task &other) : weight(other.weight), costs(other.costs), times(other.times), id(other.id) {}

    virtual ~Task() {
    }

    void setId(int id) {
        Task::id = id;
    }

    void setTimes(T *times) {
        Task::times = times;
    }

    void setCosts(C *costs) {
        Task::costs = costs;
    }

    int getId() const {
        return id;
    }

    T *getTimes() const {
        return times;
    }

    C *getCosts() const {
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
