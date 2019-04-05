#include "DAGenerator.h"

void DAGenerator::cleanUpMatrix() {
    for (int i = 0; i < nodes; ++i) {
        delete[] adjustmentMatrix[i];
    }
    delete[] adjustmentMatrix;
}

void DAGenerator::randomizeNewConnection(int index) {
    int upperNodeIndex = randomizeFromRange(index - 1, 0);
    this->adjustmentMatrix[upperNodeIndex][index] = 1;
}

void DAGenerator::coverIsolatedNode() {
    for (int i = 1; i < nodes; ++i) {
        int sum = 0;
        for (int j = 0; j < nodes; ++j) {
            sum += this->adjustmentMatrix[j][i];
        }
        if (sum < 1) {
            randomizeNewConnection(i);
        }
    }
}

void DAGenerator::randomizeConnections(const std::map<int, std::list<int>> &levels) {
    for (int i = 1; i < levels.size(); i++) {
        auto topLevel = levels.at(i - 1);
        for (auto &tl:topLevel) {
            const auto &lowerLevel = levels.at(i);
            for (auto &ll:lowerLevel) {
                if (isEdge()) {
                    this->adjustmentMatrix[tl][ll] = 1;
                }
            }
        }
    }
}

std::map<int, std::list<int>> DAGenerator::randomizeLevels(int nodeNumber) {
    std::map<int, std::list<int>> levels;
    levels[0].push_back(0);
    nodeNumber--;

    int i = 1;
    int currentIndex = 1;
    while (nodeNumber > 0) {
        long nodesInLevel = rand() % nodeNumber + 1;
        nodeNumber -= nodesInLevel;
        for (int j = 0; j < nodesInLevel; ++j) {
            levels[i].push_back(currentIndex++);
        }
        i++;
    }
    return levels;
}

int **DAGenerator::initializeMatrix(const int rows) {
    int **matrix = new int *[rows];
    for (int i = 0; i < rows; ++i) {
        matrix[i] = new int[rows];
        for (int j = 0; j < rows; ++j) {
            matrix[i][j] = 0;
        }
    }
    return matrix;
}

DAGenerator::~DAGenerator() {
    cleanUpMatrix();
}

DAGenerator::DAGenerator() {
    srand(time(nullptr));
}

int **DAGenerator::generate(const int nodes) {
    this->nodes = nodes;
    this->adjustmentMatrix = initializeMatrix(nodes);
    auto levels = randomizeLevels(nodes);
    randomizeConnections(levels);
    coverIsolatedNode();

    return this->adjustmentMatrix;
}
