//
// Created by Kajetan Lipensky on 2019-04-23.
//

#include "DecisionMaker.h"

DecisionMaker::DecisionMaker() = default;

void DecisionMaker::setPreferredProcesses(GraphStepper stepper, int **indexArr, const TasksContainer& container) {
    vector<Task *> tasks = stepper.getFirstPath();
    vector<Process> processes = container.getProcesses();

    for (auto &t: tasks) {
        //TODO add check if the communication can be made, if not then check next best value process
        int indexOfBestValueProcess = indexArr[t->getId()][0];
        t->setPreferredProcess(processes.at(indexOfBestValueProcess));
    }
}
