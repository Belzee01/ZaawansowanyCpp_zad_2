//
// Created by Belzee on 19.04.2019.
//

#include <iostream>
#include <list>
#include <map>
#include "Task.h"
#include <vector>
#include "TasksContainer.h"

#ifndef ZAD2_GRAPHSTEPPER_H
#define ZAD2_GRAPHSTEPPER_H

using namespace std;

class GraphStepper {
private:
    vector<vector<Task *>> globalPaths;
    int counter;
public:
    GraphStepper();

    bool getPaths(vector<Task *> &paths, int startId, int endId, Task *taskArr, map<Task *, list<Task *>> tasksMap);

    void startSearch(int startId, int endId, TasksContainer container);

    const vector<vector<Task *>> &getGlobalPaths() const;
};


#endif //ZAD2_GRAPHSTEPPER_H
