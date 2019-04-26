#ifndef ZAD2_GRAPHSTEPPER_H
#define ZAD2_GRAPHSTEPPER_H

#include <iostream>
#include <list>
#include <map>
#include "TasksContainer.h"
#include <vector>

using namespace std;

class GraphStepper {
private:
    int counter;

    template<class C, class Tm>
    bool getPaths(vector<vector<Task<C, Tm> *>> &globalPaths, vector<Task<C, Tm> *> &paths, int startId, int endId,
                  Task<C, Tm> *taskArr,
                  map<Task<C, Tm> *, list<Task<C, Tm> *>> tasksMap);

public:
    GraphStepper();

    template<class Com, class C, class Tm>
    vector<vector<Task<C, Tm> *>> startSearch(int startId, int endId, TasksContainer<Com, C, Tm> container);
};

GraphStepper::GraphStepper() {
    this->counter = 0;
}

template<class C, class Tm>
bool
GraphStepper::getPaths(vector<vector<Task<C, Tm> *>> &globalPaths, vector<Task<C, Tm> *> &paths,
                       int startId, int endId, Task<C, Tm> *taskArr,
                       map<Task<C, Tm> *, list<Task<C, Tm> *>> tasksMap) {
    Task<C, Tm> *currentTask = &taskArr[startId];

    if (startId == endId) {
        paths.push_back(currentTask);
        return true;
    }

    list<Task<C, Tm> *> childs;
    try {
        childs = tasksMap.at(currentTask);
        paths.push_back(currentTask);
    } catch (out_of_range e) {
        paths.push_back(currentTask);
        return false;
    }
    for (auto &child : childs) {
        bool found = getPaths(globalPaths, paths, child->getId(), endId, taskArr, tasksMap);
        if (!found) {
            paths.pop_back();
        } else {
            globalPaths.push_back(paths);
            paths.pop_back();
        }
    }
    return false;
}

template<class Com, class C, class Tm>
vector<vector<Task<C, Tm> *>>
GraphStepper::startSearch(int startId, int endId, TasksContainer<Com, C, Tm> container) {

    vector<vector<Task<C, Tm> *>> globalPaths;

    vector<Task<C, Tm> *> path;
    getPaths(globalPaths, path, startId, endId, container.getTasks(), container.getTasksMap());

    return globalPaths;
}


#endif //ZAD2_GRAPHSTEPPER_H
