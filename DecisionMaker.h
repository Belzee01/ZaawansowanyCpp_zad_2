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

        TaskIdProcessId(int taskId, int processId) : taskId(taskId), processId(processId) {}

        int getTaskId() const {
            return taskId;
        }

        int getProcessId() const {
            return processId;
        }
    };

public:
    DecisionMaker();

    static vector<vector<TaskIdProcessId>> establishPreferredProcesses(const GraphStepper& stepper, int** indexArr, const TasksContainer& container) {
        //TODO get first path, if during processing it turns out that connection cannot be made due to lack of communication,
        // then take second path and continue until full path can be established
        vector<vector<Task *>> tasksPaths = stepper.getGlobalPaths();
        const vector<Process>& processes = container.getProcesses();

        vector<vector<TaskIdProcessId>> establishedTaskPaths;

        for (auto &tasksPath : tasksPaths) {
            vector<TaskIdProcessId> currentPath;
            for (auto &t: tasksPath) {
                //TODO add check if the communication can be made, if not then check next best value process
                currentPath.emplace_back(t->getId(), indexArr[t->getId()][0]);
            }
            establishedTaskPaths.push_back(currentPath);
        }
        return establishedTaskPaths;
    }

};


#endif //ZAD2_DECISIONMAKER_H
