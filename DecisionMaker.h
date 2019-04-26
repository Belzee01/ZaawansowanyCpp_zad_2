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


struct FinalCostAndTime {
    int cost;
    int time;

    vector<int> communicationAlreadyPaid;

    void addCost(TaskIdProcessId obj, TasksContainer container);

    int getCost() const {
        return cost;
    }

    int getTime() const {
        return time;
    }
};

struct Path {
    vector<TaskIdProcessId> path;
    FinalCostAndTime costAndTime;

    Path(const vector<TaskIdProcessId> &path) : path(path) {}

    Path(const vector<TaskIdProcessId> &path, const FinalCostAndTime &costAndTime) : path(path),
                                                                                     costAndTime(costAndTime) {}

    const vector<TaskIdProcessId> &getPath() const {
        return path;
    }

    const FinalCostAndTime &getCostAndTime() const {
        return costAndTime;
    }

    void setCostAndTime(const FinalCostAndTime &costAndTime) {
        Path::costAndTime = costAndTime;
    }
};

class DecisionMaker {
private:

public:
    DecisionMaker();

    static vector<Path>
    establishPreferredProcesses(const GraphStepper &stepper, int **indexArr, TasksContainer container);

    static vector<Path> calculateFinalCostAndTime(vector<Path> paths, TasksContainer container);

    static vector<vector<vector<int>>> prepareForOutput(vector<Path> paths, TasksContainer container);
};


#endif //ZAD2_DECISIONMAKER_H
