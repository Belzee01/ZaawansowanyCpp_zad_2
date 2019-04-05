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
    int cost;
    int capacity;
    int *procConnections;

    int procSize;

public:
    Communication();

    explicit Communication(int procSize);

    Communication(int cost, int capacity, int *procConnections, int procSize);

    int getCost() const;

    int getCapacity() const;

    int *getProcConnections() const;

public:
    static void coverNoConnections(Communication *comm, int commSize, int procSize) {
        int *z = new int[procSize];
        for (int k = 0; k < procSize; ++k) {
            z[k] = 0;
        }

        for (int i = 0; i < commSize; ++i) {
            for (int j = 0; j < procSize; ++j) {
                z[j] += comm[i].procConnections[j];
            }
        }
        for (int k = 0; k < procSize; ++k) {
            if (z[k] == 0) {
                int commIndex = rand() % commSize;
                comm[commIndex].procConnections[k] = 1;
            }
        }
    }
};


#endif //TEST_1_COMMUNICATION_H
