//
// Created by Kajetan Lipensky on 2019-04-05.
//

#ifndef TEST_1_COMMUNICATION_H
#define TEST_1_COMMUNICATION_H

#include <cstdlib>
#include <time.h>
#include <iostream>
#include <list>

using namespace std;

class Communication {

private:
    int id{};
    int cost{};
    int capacity{};
    int *procConnections{};

    int procSize{};

public:
    Communication();

    Communication(int id, int cost, int capacity, int *procConnections, int procSize);

    int getCost() const;

    int getCapacity() const;

    int *getProcConnections() const;

public:

    int getId() const;

};


#endif //TEST_1_COMMUNICATION_H
