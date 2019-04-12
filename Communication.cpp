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

Communication::Communication(int id, int cost, int capacity, int *procConnections, int procSize) : id(id), cost(cost),
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

int Communication::getId() const {
    return id;
}

void Communication::setId(int id) {
    Communication::id = id;
}

void Communication::setCost(int cost) {
    Communication::cost = cost;
}

void Communication::setCapacity(int capacity) {
    Communication::capacity = capacity;
}

void Communication::setProcConnections(int *procConnections) {
    Communication::procConnections = procConnections;
}

