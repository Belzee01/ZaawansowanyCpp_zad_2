//
// Created by Kajetan Lipensky on 2019-04-03.
//

#ifndef TEST_1_TASKSCONTAINER_H
#define TEST_1_TASKSCONTAINER_H

#include "Task.h"
#include "Process.h"
#include <list>

using namespace std;

template<class T>
class TasksContainer {
private:
    int numberOfTasks;
    std::list<Task<T>> *tasks;

public:
    TasksContainer(int numberOfTasks, std::list<Process> proc, int **adjustmentMatrix) : numberOfTasks(numberOfTasks) {
        this->tasks = new std::list<Task<T>>[numberOfTasks];

        int id = 0;
        for (int i = 0; i < numberOfTasks; ++i) {
            for (int j = 0; j < numberOfTasks; ++j) {
                if (adjustmentMatrix[i][j] == 1)
                    this->tasks[id].push_back(*new Task<T>(j, proc));
            }
            id++;
        }
    }

    list<Task<T>> *getTasks() {
        return tasks;
    }
};


#endif //TEST_1_TASKSCONTAINER_H
