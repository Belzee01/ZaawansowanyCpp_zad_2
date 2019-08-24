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
#include "OutputWriter.h"
#include "Node.h"
#include <set>
#include <algorithm>


using namespace std;

void BFS(vector<vector<Node *>> *levels, Node *node, int counter = 0) {

    for (auto n : node->children) {
        if ((*levels).size() < counter + 1)
            (*levels).emplace_back();

        if (std::find((*levels)[counter].begin(), (*levels)[counter].end(), n) != (*levels)[counter].end()) {
        } else {
            bool parentPresentInTheLevel = false;
            for (auto p : n->parent) {
                if (std::find((*levels)[counter].begin(), (*levels)[counter].end(), p) != (*levels)[counter].end()) {
                    parentPresentInTheLevel = true;
                }
            }
            if (parentPresentInTheLevel)
                continue;
            (*levels)[counter].push_back(n);
            BFS(levels, n, ++counter);
            --counter;
        }
    }
}

void bubbleSort(vector<Node *> *arr) {
    int i, j;
    for (i = 0; i < (*arr).size() - 1; i++)
        for (j = 0; j < (*arr).size() - i - 1; j++)
            if ((*arr)[j]->cost > (*arr)[j + 1]->cost) {
                Node *temp = (*arr)[j];
                (*arr)[j] = (*arr)[j + 1];
                (*arr)[j + 1] = temp;
            }
}

void pickFastestNode(vector<Node *> *level, TasksContainer<int, float, float> *tasksContainer,
                     TimesCostsMatrix *costsMatrix) {
    map<int, vector<Node *>> processBasedNodes;

    for (auto node : *level) {
        for (int i = 0; i < tasksContainer->getProcSize(); ++i) {
            if (node->processId == i) {
                node->time = tasksContainer->getTasks()[node->taskId].getTimes()[node->processId];
                processBasedNodes[i].push_back(node);
            }
        }
    }

    level->clear();
    for (auto nodes: processBasedNodes) {
        bubbleSort(&nodes.second);
        level->insert(level->begin(), nodes.second.begin(), nodes.second.end());
    }
}

//vector of processing times, each entry is identified by processId
vector<float>
time(vector<float> processingTimes, TasksContainer<int, float, float> *tasksContainer, vector<vector<Node *>> levels,
     TimesCostsMatrix *costsMatrix) {
    for (int i = 0; i < tasksContainer->getProcSize(); ++i) {
        processingTimes.push_back(0.0);
    }

    for (auto level : levels) {
        pickFastestNode(&level, tasksContainer, costsMatrix);
        for (auto node : level) {
            float communicationCost = 0.0;
            for (auto parent: node->parent) {
                if (node->processId != parent->processId) {
                    int communicationId = tasksContainer->getBestPossibleConnection(parent->processId, node->processId);
                    node->communicationChannelId = communicationId;
                    communicationCost += 0.0;//tasksContainer->getComm()[communicationId].getCost();
                }
            }
            float maxProcessingTime = 0.0;
            for (auto parent : node->parent) {
                float temp;
                if (parent->processingTime > processingTimes[node->processId])
                    temp = parent->processingTime;
                else
                    temp = processingTimes[node->processId];

                if (temp > maxProcessingTime)
                    maxProcessingTime = temp;
            }

            processingTimes[node->processId] +=
                    node->time + communicationCost
                    + (maxProcessingTime - processingTimes[node->processId]);
            node->processingTime = processingTimes[node->processId];
        }
    }
    return processingTimes;
}

float
cost(vector<float> processingTimes, TasksContainer<int, float, float> *tasksContainer, vector<vector<Node *>> levels,
     TimesCostsMatrix *costsMatrix) {

    vector<int> communicationAlreadyPaid;

    float cost = 0.0;

    for (auto level : levels) {

        for (auto node : level) {
            if (std::find(communicationAlreadyPaid.begin(), communicationAlreadyPaid.end(),
                          node->communicationChannelId) !=
                communicationAlreadyPaid.end()) {

            } else {
                communicationAlreadyPaid.push_back(node->communicationChannelId);
                cost += tasksContainer->getComm().at(node->communicationChannelId).getCost();
            }
            cost += tasksContainer->getProcesses().at(node->processId).getEffectiveCost();
            cost += tasksContainer->getTasks()[node->taskId].getCosts()[node->processId];
        }
    }
    return cost;
}

int main(int args, char **argv) {

    auto *parser = new Parsers<float, float, int>();
    auto *taskContainer = parser->parse(argv[1]);
    taskContainer->printData();

    cout << endl;

    auto tcMatrix = new TimesCostsMatrix(*taskContainer);
    tcMatrix->evaluateIndexArray(taskContainer->getTasks());
    tcMatrix->printOutIndexMatrix();


    Node *head = Node::parseToGraph(taskContainer, tcMatrix->getIndexArr());

    vector<vector<Node *>> levels;
    levels.emplace_back();

    //TODO find way to get children on proper levels
    levels[0].push_back(head);
    BFS(&levels, head, 1);

    vector<float> processingTimes;

    time(processingTimes, taskContainer, levels, tcMatrix);

//    auto stepper = new GraphStepper();
//
//    int endId = -1;
//    while (endId < 0 || endId > taskContainer->getTasksSize()) {
//        cout<<endl;
//        cout<<"Pass endId value: ";
//        cin >> endId;
//        cout<<endl;
//    }
//
//    auto initialPaths = stepper->startSearch(0, endId, *taskContainer);
//
//    auto paths = DecisionMaker::establishPreferredProcesses(initialPaths, tcMatrix->getIndexArr(),
//                                                                                      *taskContainer);
//    for (auto &path : paths) {
//        for (auto &p : path.getPath()) {
//            cout << p.getTaskId() << " x " << p.getProcessId() << " x " << p.getComm() << " -> ";
//        }
//        cout << endl;
//    }
//
//    paths = DecisionMaker::calculateFinalCostAndTime(paths, *taskContainer);
//    auto outputData = DecisionMaker::prepareForOutput(paths, *taskContainer);
//
//    auto outputWriter = OutputWriter();
//    outputWriter.writeToFile(outputData);
}