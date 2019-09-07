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

void bubbleSort(vector<Node *> *arr, TasksContainer<int, float, float> *tasksContainer) {
    int i, j;
    for (i = 0; i < (*arr).size() - 1; i++)
        for (j = 0; j < (*arr).size() - i - 1; j++)
            if (tasksContainer->getTasks()[(*arr)[j]->taskId].getTimes()[(*arr)[j]->processId] >
                tasksContainer->getTasks()[(*arr)[j + 1]->taskId].getTimes()[(*arr)[j + 1]->processId]) {
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
        bubbleSort(&nodes.second, tasksContainer);
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
                    if (tasksContainer->getTasks()[node->taskId].getWeight() <= 0.1)
                        communicationCost += 0.0;//tasksContainer->getComm()[communicationId].getCost();
                    else
                        communicationCost += tasksContainer->getTasks()[node->taskId].getWeight() /
                                             tasksContainer->getComm()[node->communicationChannelId].getCapacity();
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


void rankingSort(vector<IndexValue> *ranking) {
    int i, j;
    for (i = 0; i < (*ranking).size() - 1; i++)
        for (j = 0; j < (*ranking).size() - i - 1; j++)
            if ((*ranking)[j].value > (*ranking)[j + 1].value) {
                IndexValue temp = (*ranking)[j];
                (*ranking)[j] = (*ranking)[j + 1];
                (*ranking)[j + 1] = temp;
            }
}

vector<IndexValue> ranking(vector<vector<vector<Node *>>> graphContainer, float k, float c, float punish = 1) {
    vector<IndexValue> rankingIndexes;

    for (int i = 0; i < graphContainer.size(); ++i) {
        for (int j = 0; j < graphContainer[i].size(); ++j) {
            for (int g = 0; g < graphContainer[i][j].size(); ++g) {

                IndexValue F = {
                        i, j, g, k *
                                 graphContainer[i][j][g]->finalCost
                                 + c *
                                   graphContainer[i][j][g]->finalTime
                                   * punish
                };
                rankingIndexes.push_back(F);
            }
        }
    }

    rankingSort(&rankingIndexes);
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

void getPaths(vector<vector<Node *>> *allPaths, vector<Node *> *currentPath, Node *node) {

    currentPath->push_back(node);
    if (node->children.empty()) {
        vector<Node *> persistPath;
        for (auto p : *currentPath) {
            persistPath.push_back(p);
        }
        allPaths->push_back(persistPath);
        return;
    }
    for (auto n:node->children) {
        getPaths(allPaths, currentPath, n);
        currentPath->pop_back();
    }
}

void applyRandMutation(Node *node) {
    int mutationId = rand() % 4;

    node->mutationIdx = mutationId;
    for (auto child : node->children) {
        applyRandMutation(child);
    }
}

int findCheapestProcess(TasksContainer<int, float, float> *tasksContainer) {
    float tempMin = 99999999;

    int minIdx = -1;
    for (int i = 0; i < tasksContainer->getProcSize(); ++i) {
        if (tasksContainer->getProcesses()[i].getInitialCost() < tempMin) {
            tempMin = tasksContainer->getProcesses()[i].getInitialCost();
            minIdx = i;
        }
    }
    return minIdx;
}

Node *findTaskWithMostExpensiveTK(TasksContainer<int, float, float> *tasksContainer, Node *node,
                                  Node *currentSelect = nullptr) {
    if (currentSelect == nullptr)
        currentSelect = node;

    float currentSelectTK = tasksContainer->getTasks()[currentSelect->taskId].getCosts()[currentSelect->processId] *
                            tasksContainer->getTasks()[currentSelect->taskId].getTimes()[currentSelect->processId];

    float nodeTK = tasksContainer->getTasks()[node->taskId].getCosts()[node->processId] *
                   tasksContainer->getTasks()[node->taskId].getTimes()[node->processId];
    if (currentSelectTK < nodeTK) {

        currentSelect = node;
    }

    for (auto child : node->children) {
        currentSelect = findTaskWithMostExpensiveTK(tasksContainer, child, currentSelect);
    }

    return currentSelect;
}

int getOptimalProcessForTask(Node *node, TasksContainer<int, float, float> *tasksContainer) {

    int minProcId = -1;
    float minTK = 99999999999.0;
    for (int i = 0; i < tasksContainer->getProcSize(); ++i) {
        float tempTK = tasksContainer->getTasks()[node->taskId].getTimes()[i] *
                       tasksContainer->getTasks()[node->taskId].getCosts()[i];

        if (tempTK < minTK) {
            minTK = tempTK;
            minProcId = i;
        }
    }
    return minProcId;
}

int getCheapestProcessForTask(Node *node, TasksContainer<int, float, float> *tasksContainer) {

    int minProcId = -1;
    float minK = 99999999999.0;
    for (int i = 0; i < tasksContainer->getProcSize(); ++i) {
        float tempK = tasksContainer->getTasks()[node->taskId].getCosts()[i];

        if (tempK < minK) {
            minK = tempK;
            minProcId = i;
        }
    }
    return minProcId;
}

int getFastestProcessForTask(Node *node, TasksContainer<int, float, float> *tasksContainer) {

    int minProcId = -1;
    float minK = 99999999999.0;
    for (int i = 0; i < tasksContainer->getProcSize(); ++i) {
        float tempK = tasksContainer->getTasks()[node->taskId].getTimes()[i];

        if (tempK < minK) {
            minK = tempK;
            minProcId = i;
        }
    }
    return minProcId;
}

vector<Node *> findMostExpensivePath(vector<vector<Node *>> paths, TasksContainer<int, float, float> *tasksContainer) {

    int maxPathId = 0;
    float tempCost = 0.0;
    for (int i = 0; i < paths.size(); ++i) {
        float totalCost = 0.0;
        for (auto node: paths[i]) {
            totalCost += tasksContainer->getProcesses()[node->processId].getEffectiveCost();
            totalCost += tasksContainer->getTasks()[node->taskId].getCosts()[node->processId];
        }
        if (totalCost > tempCost) {
            tempCost = totalCost;
            maxPathId = i;
        }
    }
    return paths[maxPathId];
}

void applyCheapestProcesses(vector<Node *> nodes, TasksContainer<int, float, float> *tasksContainer) {

    for (auto node: nodes) {
        int procId = getCheapestProcessForTask(node, tasksContainer);
        node->processId = procId;
    }
}

void applyFastestProcesses(vector<Node *> nodes, TasksContainer<int, float, float> *tasksContainer) {

    for (auto node: nodes) {
        int procId = getFastestProcessForTask(node, tasksContainer);
        node->processId = procId;
    }
}

void groupByProcessesAndApply(Node **nodes, TasksContainer<int, float, float> *tasksContainer) {

    vector<vector<Node *>> res;
    for (auto p : tasksContainer->getProcesses()) {
        res.emplace_back();
    }
    //pogrupuj po processach
    for (int i = 0; i < tasksContainer->getTasksSize(); ++i) {
        res[nodes[i]->processId].push_back(nodes[i]);
    }
    vector<Node *> maxGroup;
    int maxProcId = -1;
    vector<Node *> minGroup;
    int minProcId = -1;

    int tempMaxSize = 0;
    int tempMinSize = INT_MAX;

    //najbardziej obciazony
    //najmnije obciazony
    for (int j = 0; j < res.size(); ++j) {
        if (res[j].size() > tempMaxSize) {
            tempMaxSize = res[j].size();
            maxGroup = res[j];
            maxProcId = j;
        }
        if (res[j].size() < tempMinSize) {
            tempMinSize = res[j].size();
            minGroup = res[j];
            minProcId = j;
        }
    }

    //find longest task to be executed
    float maxTime = -1.0;
    Node *longestTask = nullptr;
    for (auto node: maxGroup) {
        if (tasksContainer->getTasks()[node->taskId].getTimes()[maxProcId] > maxTime) {
            maxTime = tasksContainer->getTasks()[node->taskId].getTimes()[maxProcId];
            longestTask = node;
        }
    }
    longestTask->processId = minProcId;
}

vector<Node *> findCriticalPath(vector<vector<Node *>> paths, TasksContainer<int, float, float> *tasksContainer) {

    int maxPathId = 0;
    float tempTime = 0.0;
    for (int i = 0; i < paths.size(); ++i) {
        float totalTime = 0.0;
        for (auto node: paths[i]) {
            totalTime += tasksContainer->getProcesses()[node->processId].getStartDelay();
            totalTime += tasksContainer->getTasks()[node->processId].getTimes()[node->processId];
        }
        if (totalTime > tempTime) {
            tempTime = totalTime;
            maxPathId = i;
        }
    }
    return paths[maxPathId];
}

int lowestIncreaseInCommunicationCost(float currentCost, Node *node, Node *selectedNode,
                                      TasksContainer<int, float, float> *tasksContainer) {

    int selectedCommunicationId = selectedNode->communicationChannelId;
    float lowestIncrease = 9999999.9;

    for (int i = 0; i < tasksContainer->getComm().size(); ++i) {

        vector<vector<Node *>> localLevels;
        localLevels.emplace_back();
        localLevels[0].push_back(node);
        BFS(&localLevels, node, 1);

        vector<Node *> fllatenLevels;
        vector<Node *> clonedGraph = initialClone(localLevels, &fllatenLevels);
        Node *clonedHead = clone(clonedGraph, fllatenLevels);

        localLevels.clear();
        localLevels.emplace_back();
        localLevels[0].push_back(clonedHead);
        BFS(&localLevels, clonedHead, 1);

        Node **fllatten = new Node *[tasksContainer->getTasksSize()];
        for (int j = 0; j < tasksContainer->getTasksSize(); ++j) {
            fllatten[j] = nullptr;
        }
        flattenToArray(fllatten, clonedHead);

        Node *toBeChanged = fllatten[selectedNode->taskId];
        toBeChanged->communicationChannelId = i;

        float increasedCost = cost(tasksContainer, localLevels);
        if (increasedCost > currentCost) {
            if (increasedCost < lowestIncrease) {
                lowestIncrease = increasedCost;
                selectedCommunicationId = i;
            }
        }
    }

    return selectedCommunicationId;
}

int highestBandwitch(TasksContainer<int, float, float> *tasksContainer) {

    int commId = 0;
    float highestBand = 0.0;

    for (int i = 0; i < tasksContainer->getComm().size(); ++i) {
        if (tasksContainer->getComm()[i].getCapacity() > highestBand) {
            commId = i;
            highestBand = tasksContainer->getComm()[i].getCapacity();
        }
    }
    return commId;
}

int rarestUsedCommunication(Node **nodes, TasksContainer<int, float, float> *tasksContainer) {

    int *commCount = new int[tasksContainer->getComm().size()];
    for (int j = 0; j < tasksContainer->getComm().size(); ++j) {
        commCount[j] = 0;
    }

    for (int i = 0; i < tasksContainer->getTasksSize(); ++i) {
        if (nodes[i]->communicationChannelId != -1)
            commCount[nodes[i]->communicationChannelId] += 1;
    }

    int commId = 0;
    int lowestCommCount = 9999999;
    for (int j = 0; j < tasksContainer->getComm().size(); ++j) {
        if (commCount[j] < lowestCommCount) {
            lowestCommCount = commCount[j];
            commId = j;
        }
    }
    return commId;
}

int lowestCostBandwith(TasksContainer<int, float, float> *tasksContainer) {

    int commId = 0;
    float highestBand = 99999999.9;

    for (int i = 0; i < tasksContainer->getComm().size(); ++i) {
        if (tasksContainer->getComm()[i].getCapacity() * tasksContainer->getComm()[i].getCost() < highestBand) {
            commId = i;
            highestBand = tasksContainer->getComm()[i].getCapacity() * tasksContainer->getComm()[i].getCost();
        }
    }
    return commId;
}

vector<vector<Node *>> groupByProcesses(Node **nodes, TasksContainer<int, float, float> *tasksContainer) {
    vector<vector<Node *>> res;
    for (auto p : tasksContainer->getProcesses()) {
        res.emplace_back();
    }
    //pogrupuj po processach
    for (int i = 0; i < tasksContainer->getTasksSize(); ++i) {
        res[nodes[i]->processId].push_back(nodes[i]);
    }

    return res;
}

int main(int args, char **argv) {

    srand(static_cast <unsigned> (time(0)));

    auto *parser = new Parsers<float, float, int>();
    auto *taskContainer = parser->parse(argv[1]);
    taskContainer->printData();

    ofstream myfile;
    myfile.open("F_ranking.dat");
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
    int alpha = -1;

    float k = -0.1;
    float c = -0.1;

    float beta = 0.1, gamma = 0.6;

    while (alpha < 0) {
        cout << "Pass positive alpha: ";
        cin >> alpha;
        cout << endl;
    }

    while (k < 0) {
        cout << "Pass positive k param: ";
        cin >> k;
        cout << endl;
    }

    while (c < 0) {
        cout << "Pass positive c param: ";
        cin >> c;
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
    int terminationCount = 0;
    Node *bestNode = nullptr;

    myfile << "F" << "\t" << "cost" << "\t" << "time" << "\n";

    while (temp++ < 100000 && terminationCount < 100) {
        cout << temp << endl;
        //TODO calculate cost and time for each graph
        for (int i = 0; i < alpha; ++i) {
            for (int j = 0; j < taskContainer->getTasksSize(); ++j) {
                for (int g = 0; g < taskContainer->getProcSize(); ++g) {
                    //Level evaluation
                    vector<vector<Node *>> localLevels;
                    localLevels.emplace_back();
                    localLevels[0].push_back(graphContainer[i][j][g]);
                    BFS(&localLevels, graphContainer[i][j][g], 1);


                    //Processing times and cost evaluation
                    vector<float> times;
                    times = time(times, taskContainer, localLevels);

                    float currentCost = cost(taskContainer, localLevels);
                    Node *currentNodeHead = graphContainer[i][j][g];
                    currentNodeHead->finalTime = *max_element(times.begin(), times.end());
                    currentNodeHead->finalCost = currentCost;
                }
            }
        }

        //TODO pick solutions to be copied
        vector<IndexValue> r = ranking(graphContainer, k, c);
        if (bestNode == graphContainer[r[0].i][r[0].j][r[0].g]) {
            terminationCount++;
        } else {
            terminationCount = 0;
            bestNode = graphContainer[r[0].i][r[0].j][r[0].g];
        }

        myfile << r[0].value << "\t" << bestNode->finalCost << "\t" << bestNode->finalTime << "\n";

        for (int i = 0; i < r.size(); ++i) {
            float probability = (PI - float(i)) / PI;
            if (solutionToBeCopied.size() < copied) {
                bool isCoppied = probability >= uniformRand();
                if (isCoppied) {
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
            for (int j = 0; j < taskContainer->getTasksSize(); ++j) {
                flatArray1[j] = nullptr;
            }
            flattenToArray(flatArray1, firstNode);

            Node **flatArray2 = new Node *[taskContainer->getTasksSize()];
            for (int j = 0; j < taskContainer->getTasksSize(); ++j) {
                flatArray2[j] = nullptr;
            }
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
                            vector<vector<Node *>> paths;
                            vector<Node *> currentPath;
                            getPaths(&paths, &currentPath, node);

                            //TODO rand from task or communication mutation;
                            int taskComm = rand() % (2);

                            //TODO mutate
                            int option = rand() % 4 + 1;
                            if (taskComm > 0) {
                                switch (option) {
                                    case 1: {
                                        vector<Node *> mostexpensive = findMostExpensivePath(paths, taskContainer);
                                        applyCheapestProcesses(mostexpensive, taskContainer);
                                    }
                                        break;
                                    case 2: {
                                        vector<Node *> critical = findCriticalPath(paths, taskContainer);
                                        applyFastestProcesses(critical, taskContainer);
                                    }
                                        break;
                                    case 3: {
                                        Node *selected = findTaskWithMostExpensiveTK(taskContainer, node);
                                        //find process with cheapest processing costs
                                        int procId = getOptimalProcessForTask(selected, taskContainer);
                                        selected->processId = procId;
                                    }
                                        break;
                                    case 4: {
                                        Node **fllatten = new Node *[taskContainer->getTasksSize()];
                                        flattenToArray(fllatten, node);

                                        groupByProcessesAndApply(fllatten, taskContainer);
                                    }
                                        break;

                                    default:
                                        break;
                                }
                            } else {
                                //losujemy wezel do zmiany
                                Node **fllatten = new Node *[taskContainer->getTasksSize()];
                                for (int l = 0; l < taskContainer->getTasksSize(); ++l) {
                                    fllatten[l] = nullptr;
                                }
                                flattenToArray(fllatten, node);
                                int nodeId = rand() % taskContainer->getTasksSize();
                                switch (option) {
                                    case 1: {
                                        //najmniejszy wzrost kosztu
                                        int commId = lowestIncreaseInCommunicationCost(node->finalCost, node,
                                                                                       fllatten[nodeId],
                                                                                       taskContainer);
                                        fllatten[nodeId]->communicationChannelId = commId;
                                    }
                                        break;
                                    case 2: {
                                        //najszybsza transmisja
                                        int commId = highestBandwitch(taskContainer);
                                        fllatten[nodeId]->communicationChannelId = commId;
                                    }
                                        break;
                                    case 3: {
                                        //najrzdziej uzywany
                                        int commId = rarestUsedCommunication(fllatten, taskContainer);
                                        fllatten[nodeId]->communicationChannelId = commId;
                                    }
                                        break;
                                    case 4: {
                                        //minimalny koszt*przepustowosc
                                        int commId = lowestCostBandwith(taskContainer);
                                        fllatten[nodeId]->communicationChannelId = commId;
                                    }
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
        }

        solutionToBeCopied.clear();
        solutionToBeCross.clear();
        solutionToBeMutated.clear();

    }
    myfile.close();

    //TODO save best ranking to file
    ofstream best_ranking_file;
    best_ranking_file.open("best_ranking_system.dat");

    for (int i = 0; i < alpha; ++i) {
        for (int j = 0; j < taskContainer->getTasksSize(); ++j) {
            for (int g = 0; g < taskContainer->getProcSize(); ++g) {
                Node **fllatten = new Node *[taskContainer->getTasksSize()];
                for (int l = 0; l < taskContainer->getTasksSize(); ++l) {
                    fllatten[l] = nullptr;
                }
                flattenToArray(fllatten, graphContainer[i][j][g]);

                vector<vector<Node *>> nodesByProcesses = groupByProcesses(fllatten, taskContainer);

                for (int m = 0; m < taskContainer->getProcSize(); ++m) {
                    best_ranking_file << "P" << m << ": ";
                    for (auto node : nodesByProcesses[m]) {
                        best_ranking_file << node->taskId << "(" << node->communicationChannelId << "), ";
                    }
                    best_ranking_file << "\n";
                }
                best_ranking_file << "koszt: " << graphContainer[i][j][g]->finalCost << "\n";
                best_ranking_file << "czas: " << graphContainer[i][j][g]->finalTime;
                best_ranking_file << "\n";
                best_ranking_file << "\n";
            }
        }
    }

    best_ranking_file.close();
}