//
// Created by Kajetan Lipensky on 2019-04-23.
//

#ifndef ZAD2_DECISIONMAKER_H
#define ZAD2_DECISIONMAKER_H

#include "GraphStepper.h"
#include "TimesCostsMatrix.h"
#include <vector>

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

class DecisionMaker {
private:


public:
    DecisionMaker();

    static vector<vector<TaskIdProcessId>>
    establishPreferredProcesses(const GraphStepper &stepper, int **indexArr, TasksContainer container) {
        vector<vector<Task *>> tasksPaths = stepper.getGlobalPaths();
        const vector<Process> &processes = container.getProcesses();

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
        return establishedTaskPaths;
    }

};


#endif //ZAD2_DECISIONMAKER_H
