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

class Communication {

private:
    int id{};
    int cost{};
    int capacity{};
    std::vector<int> procConnections;

    int procSize{};

public:
    Communication();

    Communication(int id, int cost, int capacity, std::vector<int> procConnections, int procSize);

    virtual ~Communication();

    int getCost() const;

    int getCapacity() const;

    const vector<int> &getProcConnections() const;

public:

    int getId() const;

};


#endif //TEST_1_COMMUNICATION_H
