//
// Created by Kajetan Lipensky on 2019-08-22.
//

#include <list>
#include <vector>
#include "TasksContainer.h"

#ifndef ZAD2_NODE_H
#define ZAD2_NODE_H

using namespace std;

class Node {

public:
    list<Node *> parent;
    int taskId;
    int processId;
    list<Node *> children;
    float processingTime;
    float weight;
    bool processed;
    int communicationChannelId;
    float finalCost;
    float finalTime;

    int mutationIdx = -1;

    Node(int taskId, int processId, int processingTime) : taskId(taskId), processId(processId),
                                                          processingTime(processingTime) {
        this->weight = 0.0;
        this->processed = false;
        this->communicationChannelId = -1;
        this->finalCost = 9999999999.9;
        this->finalTime = 9999999999.9;
    }


    Node() {}

    Node(Node &other) : taskId(other.taskId), processId(other.processId), processingTime(other.processingTime),
                        weight(other.weight), processed(other.processed),
                        communicationChannelId(other.communicationChannelId) {
        this->finalCost = 9999999999.9;
        this->finalTime = 9999999999.9;
    }

    static Node *parseToGraph(TasksContainer<int, float, float> *tasksContainer, int **indexArr) {

        vector<Node *> nodes;

        Node *head = new Node(0, indexArr[0][0], 0);
        nodes.push_back(head);

        for (int i = 1; i < tasksContainer->getTasksSize(); ++i) {
            Node *node = new Node(i, indexArr[i][0], 0);
            nodes.push_back(node);
        }

        for (auto &it : tasksContainer->getTasksMap()) {
            for (Node *node : nodes) {
                if (it.first->id == node->taskId) {
                    for (auto task : it.second) {
                        Node *child = nodes[task->id];
                        int communicationId = tasksContainer->getBestPossibleConnection(node->processId,
                                                                                        child->processId);
                        child->communicationChannelId = communicationId;
                        child->parent.push_back(node);
                        node->children.push_back(child);
                    }
                }
            }
        }
        return head;
    }
};


#endif //ZAD2_NODE_H
