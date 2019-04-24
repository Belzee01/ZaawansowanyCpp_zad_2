//
// Created by Kajetan Lipensky on 2019-04-23.
//

#ifndef ZAD2_DECISIONMAKER_H
#define ZAD2_DECISIONMAKER_H

#include "GraphStepper.h"
#include "TimesCostsMatrix.h"
#include <vector>

using namespace std;

class DecisionMaker {
private:
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

public:
    DecisionMaker();

    static vector<vector<TaskIdProcessId>>
    establishPreferredProcesses(const GraphStepper &stepper, int **indexArr, TasksContainer container) {
        //TODO get first path, if during processing it turns out that connection cannot be made due to lack of communication,
        // then take second path and continue until full path can be established
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
                        communicationId = container.getPossibleConnection(bestValueIndex, currentPath.at(
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
