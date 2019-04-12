#include <utility>

#include <utility>

//
// Created by Kajetan Lipensky on 2019-04-03.
//

#ifndef TEST_1_TASKSCONTAINER_H
#define TEST_1_TASKSCONTAINER_H

#include "Task.h"
#include "Communication.h"
#include "Process.h"
#include <list>
#include <map>

using namespace std;

class TasksContainer {
private:
    int numberOfTasks;
    std::map<Task *, std::list<Task *>> tasks;
    std::list<Process> processes;
    Task *taskArr;
    std::list<Communication> comm;

public:

    TasksContainer(int numberOfTasks, std::map<Task *, std::list<Task *>> tasks, Task *taskArr) : numberOfTasks(
            numberOfTasks), tasks(std::move(tasks)), taskArr(taskArr) {
    }

    map<Task *, list<Task *>> getTasksMap() {
        return tasks;
    }

    Task *getTasks() {
        return taskArr;
    }

    const list<Process> &getProcesses() const {
        return processes;
    }

    void setProcesses(const list<Process> &processes) {
        TasksContainer::processes = processes;
    }

    const list<Communication> &getComm() const {
        return comm;
    }

    void setComm(const list<Communication> &comm) {
        TasksContainer::comm = comm;
    }
};


#endif //TEST_1_TASKSCONTAINER_H
