//
// Created by Kajetan Lipensky on 2019-04-05.
//

#include "Communication.h"

Communication::Communication() = default;

Communication::Communication(int procSize) : procSize(procSize) {

    this->procConnections = new int[procSize];
    this->capacity = rand() % 50 + 5;
    this->cost = rand() % 20 + 5;

    for (int i = 0; i < procSize; ++i) {
        this->procConnections[i] = rand() % 2 + 0;
    }
}

Communication::Communication(int cost, int capacity, int *procConnections, int procSize) : cost(cost),
                                                                                           capacity(capacity),
                                                                                           procConnections(
                                                                                                   procConnections),
                                                                                           procSize(procSize) {}

int Communication::getCost() const {
    return cost;
}

int Communication::getCapacity() const {
    return capacity;
}

int *Communication::getProcConnections() const {
    return procConnections;
}

