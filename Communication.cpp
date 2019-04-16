//
// Created by Kajetan Lipensky on 2019-04-05.
//

#include "Communication.h"

Communication::Communication() = default;

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

