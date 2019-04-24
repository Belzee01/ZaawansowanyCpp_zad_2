#ifndef ZAD2_TASKSCONTAINER_H
#define ZAD2_TASKSCONTAINER_H

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
    std::vector<Process> processes;
    Task *taskArr;
    std::vector<Communication> comm;

public:

    TasksContainer(int numberOfTasks, std::map<Task *, std::list<Task *>> tasks, Task *taskArr) : numberOfTasks(
            numberOfTasks), tasks(std::move(tasks)), taskArr(taskArr) {
    }

    TasksContainer(int numberOfTasks, int procSize, std::map<Task *, std::list<Task *>> tasks, Task *taskArr)
            : numberOfTasks(
            numberOfTasks), procSize(procSize), tasks(std::move(tasks)), taskArr(taskArr) {
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

    const vector<Process> &getProcesses() const {
        return processes;
    }

    const int getProcSize() const {
        return procSize;
    }

    void setProcesses(const vector<Process> &processes) {
        TasksContainer::processes = processes;
    }

    void setProcesses(const vector<Process> &processes, const int procSize) {
        TasksContainer::processes = processes;
        TasksContainer::procSize = procSize;
    }

    const vector<Communication> &getComm() const {
        return comm;
    }

    void setComm(const vector<Communication> &comm) {
        TasksContainer::comm = comm;
    }

public:

    void printData() {
        auto tasksAdjList = this->getTasksMap();

        for (auto &it : tasksAdjList) {
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

    //TODO add method for checking communication access between two processes
    int getPossibleConnection(int startProcId, int endProcId) {
        for (auto &c: comm) {
            if (c.getProcConnections().at(startProcId) == 1 and c.getProcConnections().at(endProcId) == 1)
                return c.getId();
        }
        return -1;
    }
};


#endif //TEST_1_TASKSCONTAINER_H
