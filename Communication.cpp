#include <utility>

//
// Created by Kajetan Lipensky on 2019-04-05.
//

#include "Communication.h"

Communication::Communication() = default;

Communication::Communication(int id, int cost, int capacity, std::vector<int> procConnections, int procSize) : id(id), cost(cost),
                                                                                                   capacity(capacity),
                                                                                                   procSize(procSize) {
    this->procConnections = std::move(procConnections);
}

int Communication::getCost() const {
    return cost;
}

int Communication::getCapacity() const {
    return capacity;
}


int Communication::getId() const {
    return id;
}

Communication::~Communication() = default;

const vector<int> &Communication::getProcConnections() const {
    return procConnections;
}

