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
    int procSize;
    std::map<Task *, std::list<Task *>> tasks;
    std::list<Process> processes;
    Task *taskArr;
    std::list<Communication> comm;

public:

    TasksContainer(int numberOfTasks, std::map<Task *, std::list<Task *>> tasks, Task *taskArr) : numberOfTasks(
            numberOfTasks), tasks(std::move(tasks)), taskArr(taskArr) {
    }

    TasksContainer(int numberOfTasks, int procSize, std::map<Task *, std::list<Task *>> tasks, Task *taskArr) : numberOfTasks(
            numberOfTasks),procSize(procSize), tasks(std::move(tasks)), taskArr(taskArr) {
    }

    map<Task *, list<Task *>> getTasksMap() {
        return tasks;
    }

    Task *getTasks() {
        return taskArr;
    }

    int getTasksSize() {
        return numberOfTasks;
    }

    const list<Process> &getProcesses() const {
        return processes;
    }

    const int getProcSize() const {
        return procSize;
    }

    void setProcesses(const list<Process> &processes) {
        TasksContainer::processes = processes;
    }

    void setProcesses(const list<Process> &processes, const int procSize) {
        TasksContainer::processes = processes;
        TasksContainer::procSize = procSize;
    }

    const list<Communication> &getComm() const {
        return comm;
    }

    void setComm(const list<Communication> &comm) {
        TasksContainer::comm = comm;
    }

public:
    void printData() {
        auto tasksAdjList = this->getTasksMap();

        for (auto & it : tasksAdjList) {
            cout << it.first->getId() << ": ";
            for (auto &l: it.second) {
                cout << l->getId() << ", ";
            }
            cout << endl;
        }
        cout << endl;
        for (auto &p : this->getProcesses()) {
            cout << p.getInitialCost() << " " << p.getStartDelay() << " " << p.getTypeOfProcess() << endl;
        }

        cout << endl;
        for (int i = 0; i < this->getTasksSize(); i++) {
            for (int j = 0; j < this->getProcSize(); ++j) {
                cout << this->getTasks()[i].getTimes()[j] << " ";
            }
            cout << endl;
        }
        cout << endl;

        for (int i = 0; i < this->getTasksSize(); i++) {
            for (int j = 0; j < this->getProcSize(); ++j) {
                cout << this->getTasks()[i].getCosts()[j] << " ";
            }
            cout << endl;
        }
        cout << endl;
        auto comms = this->getComm();
        for (auto &c: comms) {
            cout << c.getId() << " " << c.getCost() << " " << c.getCapacity() << " ";
            auto connections = c.getProcConnections();
            for (int i = 0; i < this->getProcSize(); ++i) {
                cout << connections[i] << " ";
            }
            cout << endl;
        }
    }
};


#endif //TEST_1_TASKSCONTAINER_H
