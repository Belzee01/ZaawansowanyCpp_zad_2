//
// Created by Belzee on 19.04.2019.
//

#include "GraphStepper.h"

GraphStepper::GraphStepper() {
    this->counter = 0;
};

bool GraphStepper::getPaths(vector<Task *> &paths, int startId, int endId, Task *taskArr,
                            map<Task *, list<Task *>> tasksMap) {
    Task *currentTask = &taskArr[startId];

    if (startId == endId) {
        paths.push_back(currentTask);
        return true;
    }

    list<Task *> childs;
    try {
        childs = tasksMap.at(currentTask);
        paths.push_back(currentTask);
    } catch (out_of_range e){
        paths.push_back(currentTask);
        return false;
    }
    for (auto &child : childs) {
        bool found = getPaths(paths, child->getId(), endId, taskArr, tasksMap);
        if (!found) {
            paths.pop_back();
        } else {
            globalPaths.push_back(paths);
            paths.pop_back();
        }
    }
    return false;
}

void GraphStepper::startSearch(int startId, int endId, TasksContainer container) {

    vector<Task *> path;
    getPaths(path, startId, endId, container.getTasks(), container.getTasksMap());
}

const vector<vector<Task *>> &GraphStepper::getGlobalPaths() const {
    return globalPaths;
}
