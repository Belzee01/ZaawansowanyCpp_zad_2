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
#include "OutputWriter.h"

using namespace std;

int main(int args, char **argv) {

    auto *parser = new Parsers<float, float, int>();
    auto *taskContainer = parser->parse(argv[1]);
    taskContainer->printData();

    cout << endl;

    auto tcMatrix = new TimesCostsMatrix(*taskContainer);
    tcMatrix->evaluateIndexArray(taskContainer->getTasks());
    tcMatrix->printOutIndexMatrix();
    auto stepper = new GraphStepper();

    int endId = -1;
    while (endId < 0 || endId > taskContainer->getTasksSize()) {
        cout<<endl;
        cout<<"Pass endId value: ";
        cin >> endId;
        cout<<endl;
    }

    auto initialPaths = stepper->startSearch(0, endId, *taskContainer);

    auto paths = DecisionMaker::establishPreferredProcesses(initialPaths, tcMatrix->getIndexArr(),
                                                                                      *taskContainer);
    for (auto &path : paths) {
        for (auto &p : path.getPath()) {
            cout << p.getTaskId() << " x " << p.getProcessId() << " x " << p.getComm() << " -> ";
        }
        cout << endl;
    }

    paths = DecisionMaker::calculateFinalCostAndTime(paths, *taskContainer);
    auto outputData = DecisionMaker::prepareForOutput(paths, *taskContainer);

    auto outputWriter = OutputWriter();
    outputWriter.writeToFile(outputData);
}