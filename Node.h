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
    float cost;
    float time;

    Node(int taskId, int processId, int processingTime) : taskId(taskId), processId(processId),
                                                          processingTime(processingTime) {
        this->weight = 0.0;
        this->processed = false;
        this->cost = 0.0;
        this->communicationChannelId = -1;
    }

    static Node *parseToGraph(TasksContainer<int, float, float> *tasksContainer, int **indexArr) {

        vector<Node *> nodes;

        Node *head = new Node(0, indexArr[0][0], 0);
        nodes.push_back(head);

        for (int i = 1; i < tasksContainer->getTasksSize(); ++i) {
            nodes.push_back(new Node(i, indexArr[i][0], 0));
        }

        for (auto &it : tasksContainer->getTasksMap()) {
            for (Node* node : nodes) {
                if (it.first->id == node->taskId) {
                    for (auto task : it.second) {
                        Node *child = nodes[task->id];
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
