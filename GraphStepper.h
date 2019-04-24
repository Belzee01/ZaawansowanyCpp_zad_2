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
    vector<vector<Task *>> globalPaths;
    int counter;

    bool getPaths(vector<Task *> &paths, int startId, int endId, Task *taskArr, map<Task *, list<Task *>> tasksMap);

public:
    GraphStepper();

    void startSearch(int startId, int endId, TasksContainer container);

    const vector<vector<Task *>> &getGlobalPaths() const;

    vector<Task *> getFirstPath();
};


#endif //ZAD2_GRAPHSTEPPER_H
