//
// Created by Kajetan Lipensky on 2019-04-23.
//

#ifndef ZAD2_DECISIONMAKER_H
#define ZAD2_DECISIONMAKER_H

#include "GraphStepper.h"
#include "TimesCostsMatrix.h"
#include <vector>
#include <algorithm>

using namespace std;

struct TaskIdProcessId {
    int taskId;
    int processId;
    int comm{};

    TaskIdProcessId(int taskId, int processId) : taskId(taskId), processId(processId) {}

    TaskIdProcessId(int taskId, int processId, int comm) : taskId(taskId), processId(processId), comm(comm) {}

    int getTaskId() const {
        return taskId;
    }

    int getProcessId() const {
        return processId;
    }

    int getComm() const {
        return comm;
    }

    void setComm(int comm) {
        TaskIdProcessId::comm = comm;
    }
};

template<class C, class Tm>
struct FinalCostAndTime {
    C cost;
    Tm time;

    vector<int> communicationAlreadyPaid;

    template <class Com>
    void addCost(TaskIdProcessId obj, TasksContainer<Com, C, Tm> container);

    C getCost() const {
        return cost;
    }

    Tm getTime() const {
        return time;
    }
};

template<class C, class Tm>
template<class Com>
void FinalCostAndTime<C, Tm>::addCost(TaskIdProcessId obj, TasksContainer<Com, C, Tm> container) {
    if (std::find(communicationAlreadyPaid.begin(), communicationAlreadyPaid.end(), obj.getComm()) !=
        communicationAlreadyPaid.end()) {

    } else {
        communicationAlreadyPaid.push_back(obj.getComm());
        cost += container.getComm().at(obj.getComm()).getCost();
    }
    cost += container.getProcesses().at(obj.getProcessId()).getEffectiveCost();
    cost += container.getTasks()[obj.getTaskId()].getCosts()[obj.getProcessId()];
    time += container.getTasks()[obj.getTaskId()].getTimes()[obj.getProcessId()];
}

template<class C, class Tm>
struct Path {
    vector<TaskIdProcessId> path;
    FinalCostAndTime<C, Tm> costAndTime;

    Path(const vector<TaskIdProcessId> &path) : path(path) {}

    Path(const vector<TaskIdProcessId> &path, const FinalCostAndTime<C, Tm> &costAndTime) : path(path),
                                                                                     costAndTime(costAndTime) {}

    const vector<TaskIdProcessId> &getPath() const {
        return path;
    }

    const FinalCostAndTime<C, Tm> &getCostAndTime() const {
        return costAndTime;
    }

    void setCostAndTime(const FinalCostAndTime<C, Tm> &costAndTime) {
        Path::costAndTime = costAndTime;
    }
};

template<class C, class Tm>
struct OutputData {
    vector<vector<int>> tasksSortedByProcesses;
    FinalCostAndTime<C, Tm> costAndTime;
    OutputData(const vector<vector<int>> &tasksSortedByProcesses) : tasksSortedByProcesses(
            tasksSortedByProcesses) {}

    OutputData(const vector<vector<int>> &tasksSortedByProcesses, const FinalCostAndTime<C, Tm> &costAndTime)
            : tasksSortedByProcesses(tasksSortedByProcesses), costAndTime(costAndTime) {}

    const vector<vector<int>> &getTasksSortedByProcesses() const {
        return tasksSortedByProcesses;
    }

    const FinalCostAndTime<C, Tm> &getCostAndTime() const {
        return costAndTime;
    }
};

class DecisionMaker {
private:

public:
    DecisionMaker();

    template <class Com, class C, class Tm>
    static vector<Path<C, Tm>>
    establishPreferredProcesses(vector<vector<Task<C, Tm> *>> &stepper, int **indexArr, TasksContainer<Com, C, Tm> container);

    template <class Com, class C, class Tm>
    static vector<Path<C, Tm>> calculateFinalCostAndTime(vector<Path<C, Tm>> paths, TasksContainer<Com, C, Tm> container);

    template <class Com, class C, class Tm>
    static vector<OutputData<C, Tm>> prepareForOutput(vector<Path<C, Tm>> paths, TasksContainer<Com, C, Tm> container);
};


DecisionMaker::DecisionMaker() = default;

template <class Com, class C, class Tm>
vector<Path<C, Tm>>
DecisionMaker::calculateFinalCostAndTime(vector<Path<C, Tm>> paths, TasksContainer<Com, C, Tm> container) {

    for (auto &path : paths) {
        auto finalCostTime = FinalCostAndTime<C, Tm>();
        for (auto &obj: path.getPath()) {
            finalCostTime.addCost(obj, container);
        }
        path.setCostAndTime(finalCostTime);
    }

    return paths;
}

template <class Com, class C, class Tm>
vector<OutputData<C, Tm>>
DecisionMaker::prepareForOutput(vector<Path<C, Tm>> paths, TasksContainer<Com, C, Tm> container) {
    vector<OutputData<C, Tm>> all;

    for (auto &path:paths) {
        vector<vector<int>> tasksSortedByProcesses(container.getProcesses().size());
        for (auto &obj: path.getPath()) {
            tasksSortedByProcesses[obj.getProcessId()].push_back(obj.getTaskId());
        }
        all.emplace_back(tasksSortedByProcesses, path.getCostAndTime());
    }

    return all;
}

template<class Com, class C, class Tm>
vector<Path<C, Tm>>
DecisionMaker::establishPreferredProcesses(vector<vector<Task<C, Tm> *>> &stepper, int **indexArr, TasksContainer<Com, C, Tm> container) {
    vector<vector<Task<C, Tm> *>> tasksPaths = stepper;
    const vector<Process<>> &processes = container.getProcesses();

    vector<vector<TaskIdProcessId>> establishedTaskPaths;

    for (auto &tasksPath : tasksPaths) {
        vector<TaskIdProcessId> currentPath;
        int currentIndex = 0;
        for (auto &t: tasksPath) {
            int communicationId = -1;
            int bestValueIndex = -1;
            if (!currentPath.empty()) {
                int k = 0;
                while (communicationId == -1 and k < processes.size()) {
                    bestValueIndex = indexArr[t->getId()][k];
                    communicationId = container.getBestPossibleConnection(bestValueIndex, currentPath.at(
                            currentIndex - 1).getProcessId());
                    k++;
                }
                if (communicationId == -1) {
                    currentPath.clear();
                    break;
                }
            } else {
                bestValueIndex = indexArr[t->getId()][0];
            }
            currentPath.emplace_back(t->getId(), bestValueIndex, communicationId);
            currentIndex++;
        }
        if (!currentPath.empty()) {
            currentPath.at(0).setComm(currentPath.at(1).getComm());
            establishedTaskPaths.push_back(currentPath);
        }
    }

    vector<Path<C, Tm>> paths;
    for(auto &path: establishedTaskPaths) {
        paths.emplace_back(path);
    }

    return paths;
}

#endif //ZAD2_DECISIONMAKER_H
