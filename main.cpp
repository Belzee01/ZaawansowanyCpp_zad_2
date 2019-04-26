#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <list>
#include <vector>
#include <numeric>
#include "Task.h"
#include "TasksContainer.h"
#include "Process.h"
#include "Communication.h"
#include "Parsers.h"
#include "TimesCostsMatrix.h"
#include "GraphStepper.h"
#include "DecisionMaker.h"

using namespace std;

int main(int args, char **argv) {

    auto *parser = new Parsers();
    TasksContainer *taskContainer = parser->parse(argv[1]);
    taskContainer->printData();

    cout << endl;

    auto tcMatrix = new TimesCostsMatrix(taskContainer->getTasksSize(), taskContainer->getProcSize());
    tcMatrix->evaluateMatrix(taskContainer->getTasks());
    tcMatrix->evaluateIndexArray();
    tcMatrix->printOutIndexMatrix();

    auto stepper = new GraphStepper();
    stepper->startSearch(0, 8, *taskContainer);

    auto paths = DecisionMaker::establishPreferredProcesses(*stepper, tcMatrix->getIndexArr(), *taskContainer);
    for (auto &path : paths) {
        for (auto &p : path) {
            cout << p.getTaskId() << " x " << p.getProcessId() << " x " << p.getComm() << " -> ";
        }
        cout << endl;
    }
}