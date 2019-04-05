#ifndef TEST_1_DAGENERATOR_H
#define TEST_1_DAGENERATOR_H

#include<iostream>
#include<cstdlib>
#include<ctime>
#include <map>
#include <list>

#define EDGE_PERCENTAGE 40;

using namespace std;

class DAGenerator {
private:
    int nodes;
    int **adjustmentMatrix;

private:
    int **initializeMatrix(int rows);

    std::map<int, std::list<int>> randomizeLevels(int nodes);

    int randomizeFromRange(int max, int min) {
        if ((max - min + 1) < 1)
            return rand() % (1) + min;
        return rand() % (max - min + 1) + min;
    }

    bool isEdge() {
        return randomizeFromRange(100, 0) < EDGE_PERCENTAGE;
    }

    void randomizeConnections(const std::map<int, std::list<int>> &levels);

    void coverIsolatedNode();

    void randomizeNewConnection(int index);

    void cleanUpMatrix();

public:
    DAGenerator();

    virtual ~DAGenerator();

    int **generate(int nodes);
};


#endif //TEST_1_DAGENERATOR_H
