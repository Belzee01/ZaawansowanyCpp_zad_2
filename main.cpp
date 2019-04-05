#include <iostream>
#include "DAGenerator.h"
#include "InputBuilder.h"
#include "TasksContainer.h"
#include "Process.h"
#include <list>
#include "Communication.h"
#include <fstream>
#include "OutputParser.h"

using namespace std;

class CommunicationGenerator {
private:
    Communication *comms;

public:
    CommunicationGenerator(int commSize, int procSize) {
        comms = new Communication[commSize];

        for (int i = 0; i < commSize; ++i) {
            comms[i] = *new Communication(procSize);
        }
        Communication::coverNoConnections(comms, commSize, procSize);
    };

    Communication *getComms() const {
        return comms;
    }
};

class ProcessesGenerator {
private:
    std::list<Process> processes;

public:
    explicit ProcessesGenerator(int proc) {
        for (int j = 0; j < proc; ++j) {
            processes.emplace_back(j % 2 == 0.0 ? 1 : 0);
        }
    };

    const list<Process> &getProcesses() const {
        return processes;
    }
};

int main() {
    auto input = new InputBuilder();

    int a, b, c;
    std::cout<<"Pass number of nodes: ";
    cin>>a;
    std::cout<<"Pass number of processes: ";
    cin>>b;
    std::cout<<"Pass number of communication channels: ";
    cin>>c;
    input->withTasks(a)->withProc(b)->withComm(c);

    auto generator = new DAGenerator();
    auto matrix = generator->generate(input->getTasks());

    auto processGenerator = ProcessesGenerator(input->getProc());
    auto processes = processGenerator.getProcesses();

    auto communicationGenerator = CommunicationGenerator(input->getComm(), input->getProc());
    auto comms = communicationGenerator.getComms();

    auto taskContainer = new TasksContainer<int>(input->getTasks(), processes, matrix);
    auto tasksList = taskContainer->getTasks();

    ////WYPISYWANIE ------////////
    auto outputParse = new OutPutParser();
    outputParse->writeAll(input, tasksList, processes, comms);

    delete outputParse;
}