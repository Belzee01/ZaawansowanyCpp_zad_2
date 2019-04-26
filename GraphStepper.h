#ifndef ZAD2_GRAPHSTEPPER_H
#define ZAD2_GRAPHSTEPPER_H

#include <iostream>
#include <list>
#include <map>
#include "TasksContainer.h"
#include <vector>

using namespace std;

template<class Com, class C, class Tm>
class GraphStepper {
private:
    vector<vector<Task<C, Tm> *>> globalPaths;
    int counter;

    bool getPaths(vector<Task<C, Tm> *> &paths, int startId, int endId, Task<C, Tm> *taskArr,
                  map<Task<C, Tm> *, list<Task<C, Tm> *>> tasksMap);

public:
    GraphStepper();

    void startSearch(int startId, int endId, TasksContainer<Com, C, Tm> container);

    const vector<vector<Task<C, Tm> *>> &getGlobalPaths() const;
};

template<class Com, class C, class Tm>
GraphStepper<Com, C, Tm>::GraphStepper() {
    this->counter = 0;
}

template<class Com, class C, class Tm>
const vector<vector<Task<C, Tm> *>> &GraphStepper<Com, C, Tm>::getGlobalPaths() const {
    return globalPaths;
}

template<class Com, class C, class Tm>
bool
GraphStepper<Com, C, Tm>::getPaths(vector<Task<C, Tm> *> &paths, int startId, int endId, Task<C, Tm> *taskArr,
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

template<class Com, class C, class Tm>
void GraphStepper<Com, C, Tm>::startSearch(int startId, int endId, TasksContainer<Com, C, Tm> container) {

    vector<Task<C, Tm> *> path;
    getPaths(path, startId, endId, container.getTasks(), container.getTasksMap());
}


#endif //ZAD2_GRAPHSTEPPER_H
