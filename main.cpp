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
#include "Node.h"
#include <set>
#include <algorithm>
#include <math.h>


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
            if ((*arr)[j]->time > (*arr)[j + 1]->time) {
                Node *temp = (*arr)[j];
                (*arr)[j] = (*arr)[j + 1];
                (*arr)[j + 1] = temp;
            }
}

void pickFastestNode(vector<Node *> *level, TasksContainer<int, float, float> *tasksContainer) {
    map<int, vector<Node *>> processBasedNodes;

    for (auto node : *level) {
        for (int i = 0; i < tasksContainer->getProcSize(); ++i) {
            if (node->processId == i) {
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
time(vector<float> processingTimes, TasksContainer<int, float, float> *tasksContainer, vector<vector<Node *>> levels) {
    for (int i = 0; i < tasksContainer->getProcSize(); ++i) {
        processingTimes.push_back(0.0);
    }

    for (auto level : levels) {
        pickFastestNode(&level, tasksContainer);
        for (auto node : level) {
            float communicationCost = 0.0;
            for (auto parent: node->parent) {
                if (node->processId != parent->processId) {
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
                    tasksContainer->getTasks()[node->taskId].getTimes()[node->processId]
                    + communicationCost
                    + (maxProcessingTime - processingTimes[node->processId]);
            node->processingTime = processingTimes[node->processId];
        }
    }
    return processingTimes;
}

float
cost(TasksContainer<int, float, float> *tasksContainer, vector<vector<Node *>> levels) {

    vector<int> communicationAlreadyPaid;

    float cost = 0.0;

    for (auto level : levels) {

        for (auto node : level) {
            if (std::find(communicationAlreadyPaid.begin(), communicationAlreadyPaid.end(),
                          node->communicationChannelId) !=
                communicationAlreadyPaid.end()) {

            } else if (node->communicationChannelId == -1) {

            } else {
                communicationAlreadyPaid.push_back(node->communicationChannelId);
                cost += tasksContainer->getComm()[node->communicationChannelId].getCost();
            }
            if (tasksContainer->getProcesses()[node->processId].getTypeOfProcess() == 0) {
                cost += tasksContainer->getProcesses()[node->processId].getInitialCost();
            }
            cost += tasksContainer->getProcesses()[node->processId].getEffectiveCost();
            cost += tasksContainer->getTasks()[node->taskId].getCosts()[node->processId];
        }
    }
    return cost;
}

Node *findNodeWithTaskId(vector<Node *> clonedNodes, Node *node) {
    for (auto n : clonedNodes) {
        if (n->taskId == node->taskId)
            return n;
    }
    return nullptr;
}

Node *clone(vector<Node *> clonedNodes, vector<Node *> flatten) {

    for (int i = 0; i < clonedNodes.size(); ++i) {
        for (auto p : flatten[i]->parent) {
            Node *parent = findNodeWithTaskId(clonedNodes, p);
            clonedNodes[i]->parent.push_back(parent);
        }
        for (auto c : flatten[i]->children) {
            Node *child = findNodeWithTaskId(clonedNodes, c);
            clonedNodes[i]->children.push_back(child);
        }
    }
    return clonedNodes[0];
}


vector<Node *> initialClone(vector<vector<Node *>> levels, vector<Node *> *flatten) {
    vector<Node *> clonedNodes;
    for (auto level: levels) {
        for (auto n : level) {
            Node *cloneNode = new Node(*n);
            clonedNodes.push_back(cloneNode);
            flatten->push_back(n);
        }
    }
    return clonedNodes;
}

struct IndexValue {
    int i;
    int j;
    int g;

    float value;
};

vector<IndexValue> rankingSort(vector<IndexValue> ranking) {

    return ranking;
}

vector<IndexValue> ranking(vector<vector<vector<Node *>>> graphContainer, float k, float c, float punish = 0) {
    vector<IndexValue> rankingIndexes;

    for (int i = 0; i < graphContainer.size(); ++i) {
        for (int j = 0; j < graphContainer[i].size(); ++j) {
            for (int g = 0; g < graphContainer[i][j].size(); ++g) {
                IndexValue F = {
                        i, j, g, k * graphContainer[i][j][g]->cost * c * graphContainer[i][j][g]->time * punish
                };
                rankingIndexes.push_back(F);
            }
        }
    }

    rankingIndexes = rankingSort(rankingIndexes);
    return rankingIndexes;
}

float uniformRand() {
    return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

void flattenToArray(Node **flatArray, Node *node) {
    flatArray[node->taskId] = node;
    for (auto n : node->children) {
        flattenToArray(flatArray, n);
    }
}

//OPTION 0
void minCostMostExpensivePath(Node *head) {
    //TODO find most expensive path

}

//OPTION 1



//OPTION 2




//OPTION 3


int main(int args, char **argv) {

    srand(static_cast <unsigned> (time(0)));

    auto *parser = new Parsers<float, float, int>();
    auto *taskContainer = parser->parse(argv[1]);
    taskContainer->printData();

    cout << endl;

    auto tcMatrix = new TimesCostsMatrix(*taskContainer);
    tcMatrix->evaluateIndexArray(taskContainer->getTasks());
    tcMatrix->printOutIndexMatrix();


    Node *head = Node::parseToGraph(taskContainer, tcMatrix->getIndexArr());

    //Level evaluation
    vector<vector<Node *>> levels;
    levels.emplace_back();
    levels[0].push_back(head);
    BFS(&levels, head, 1);


    //Processing times and cost evaluation
    vector<float> processingTimes;
    time(processingTimes, taskContainer, levels);
    cost(taskContainer, levels);

    //User based data
    int alpha = 1;

    float k = 0.5;
    float c = 0.5;

    float beta = 0.3, gamma = 0.4, theta = 0.3;

    while (alpha < 0) {
        cout << "Pass positive alpha: ";
        cin >> alpha;
        cout << endl;
    }

    //TODO create array of alpha * tasks * proc
    vector<vector<vector<Node *>>> graphContainer;
    for (int i = 0; i < alpha; ++i) {
        graphContainer.emplace_back();
        for (int j = 0; j < taskContainer->getTasksSize(); ++j) {
            graphContainer[i].emplace_back();
        }
    }

    //TODO clone graph structure alpha * tasks * proc times

    for (int i = 0; i < alpha; ++i) {
        for (int j = 0; j < taskContainer->getTasksSize(); ++j) {
            for (int g = 0; g < taskContainer->getProcSize(); ++g) {
                vector<Node *> fllatenLevels;
                vector<Node *> clonedGraph = initialClone(levels, &fllatenLevels);
                Node *clonedHead = clone(clonedGraph, fllatenLevels);

                graphContainer[i][j].push_back(clonedHead);
            }
        }
    }

    int PI = alpha * taskContainer->getTasksSize() * taskContainer->getProcSize();


    //crossover solutions
    int crossover = int(floor(gamma * PI));
    if (crossover % 2 != 0.0)
        crossover += 1;
    vector<Node *> solutionToBeCross;

    //copied solutions:
    int copied = int(floor(beta * PI));
    vector<Node *> solutionToBeCopied;

    //mutating solutions
    int mutation = PI - copied - crossover;
    vector<Node *> solutionToBeMutated;

    int temp = 0;
    while (temp++ < 100) {
        //TODO pick solutions to be copied
        vector<IndexValue> r = ranking(graphContainer, k, c);
        for (int i = 0; i < r.size(); ++i) {
            float probability = (PI - float(i)) / PI;
            if (solutionToBeCopied.size() < copied) {
                if (probability >= uniformRand()) {
                    solutionToBeCopied.push_back(graphContainer[r[i].i][r[i].j][r[i].g]);
                }
            }
        }

        //TODO crossover mechanism
        while (solutionToBeCross.size() < crossover) {

            int firstIndex;
            int secondIndex;
            int thirdIndex;

            Node *firstNode = nullptr;
            while (firstNode == nullptr) {
                firstIndex = rand() % alpha;
                secondIndex = rand() % taskContainer->getTasksSize();
                thirdIndex = rand() % taskContainer->getProcSize();
                firstNode = graphContainer[firstIndex][secondIndex][thirdIndex];
                if (std::find(solutionToBeCross.begin(), solutionToBeCross.end(), firstNode) !=
                    solutionToBeCross.end()) {
                    firstNode = nullptr;
                } else if (std::find(solutionToBeCopied.begin(), solutionToBeCopied.end(), firstNode) !=
                           solutionToBeCopied.end()) {
                    firstNode = nullptr;
                }
            }

            Node *secondNode = nullptr;
            while (secondNode == nullptr) {
                firstIndex = rand() % alpha;
                secondIndex = rand() % taskContainer->getTasksSize();
                thirdIndex = rand() % taskContainer->getProcSize();
                secondNode = graphContainer[firstIndex][secondIndex][thirdIndex];
                if (std::find(solutionToBeCross.begin(), solutionToBeCross.end(), secondNode) !=
                    solutionToBeCross.end()) {
                    secondNode = nullptr;
                } else if (std::find(solutionToBeCopied.begin(), solutionToBeCopied.end(), secondNode) !=
                           solutionToBeCopied.end()) {
                    secondNode = nullptr;
                }
            }

            solutionToBeCross.push_back(firstNode);
            solutionToBeCross.push_back(secondNode);

            Node **flatArray1 = new Node *[taskContainer->getTasksSize()];
            flattenToArray(flatArray1, firstNode);

            Node **flatArray2 = new Node *[taskContainer->getTasksSize()];
            flattenToArray(flatArray2, secondNode);

            int split = rand() % (taskContainer->getTasksSize() - 2) + 1;
            //Crossover
            for (int i = 0; i < split; ++i) {
                Node *tempNode1 = new Node();

                tempNode1->processId = flatArray1[i]->processId;
                tempNode1->communicationChannelId = flatArray1[i]->communicationChannelId;

                Node *tempNode2 = new Node();

                tempNode2->processId = flatArray2[i]->processId;
                tempNode2->communicationChannelId = flatArray2[i]->communicationChannelId;

                flatArray1[i]->processId = tempNode2->processId;
                flatArray1[i]->communicationChannelId = tempNode2->communicationChannelId;

                flatArray2[i]->processId = tempNode1->processId;
                flatArray2[i]->communicationChannelId = tempNode1->communicationChannelId;
            }
        }

        //TODO mutatiom mechanism
        while (solutionToBeMutated.size() < mutation) {
            for (int i = 0; i < alpha; ++i) {
                for (int j = 0; j < taskContainer->getTasksSize(); ++j) {
                    for (int g = 0; g < taskContainer->getProcSize(); ++g) {
                        Node *node = graphContainer[i][j][g];
                        if (std::find(solutionToBeCross.begin(), solutionToBeCross.end(), node) !=
                            solutionToBeCross.end()) {
                        } else if (std::find(solutionToBeCopied.begin(), solutionToBeCopied.end(), node) !=
                                   solutionToBeCopied.end()) {
                        } else {

                            //TODO mutate
                            int option = rand() % 4;
                            switch (option) {
                                case 0:

                                    break;
                                case 1:
                                    break;
                                case 2:
                                    break;
                                case 3:
                                    break;

                                default:
                                    break;
                            }

                            solutionToBeMutated.push_back(node);
                        }
                    }
                }
            }
        }

        solutionToBeCopied.clear();
        solutionToBeCross.clear();
        solutionToBeMutated.clear();
    }
    cout << copied;

}