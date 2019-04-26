//
// Created by Kajetan Lipensky on 2019-04-05.
//

#ifndef ZAD2_COMMUNICATION_H
#define ZAD2_COMMUNICATION_H

#include <cstdlib>
#include <time.h>
#include <iostream>
#include <list>
#include <vector>

using namespace std;

template <class C=int, class cap=int>
class Communication {
private:
    int id{};
    C cost{};
    cap capacity{};
    std::vector<int> procConnections;

    int procSize{};

public:
    Communication() = default;

    Communication(int id, C cost, cap capacity, const vector<int> &procConnections, int procSize);

    virtual ~Communication();

    C getCost() const;

    cap getCapacity() const;

    int getProcSize() const;

    const vector<int> &getProcConnections() const;

    int getId() const;
};

template<class C, class cap>
C Communication<C, cap>::getCost() const {
    return cost;
}

template<class C, class cap>
cap Communication<C, cap>::getCapacity() const {
    return capacity;
}

template<class C, class cap>
int Communication<C, cap>::getProcSize() const {
    return procSize;
}

template<class C, class cap>
const vector<int> &Communication<C, cap>::getProcConnections() const {
    return procConnections;
}

template<class C, class cap>
int Communication<C, cap>::getId() const {
    return id;
}

template<class C, class cap>
Communication<C, cap>::Communication(int id, C cost, cap capacity, const vector<int> &procConnections, int procSize):id(
        id), cost(cost), capacity(capacity), procConnections(procConnections), procSize(procSize) {}

template<class C, class cap>
Communication<C, cap>::~Communication() = default;


#endif //TEST_1_COMMUNICATION_H
