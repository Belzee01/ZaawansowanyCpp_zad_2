//
// Created by Kajetan Lipensky on 2019-04-03.
//

#ifndef ZAD2_PROCESS_H
#define ZAD2_PROCESS_H

#include<iostream>
#include<cstdlib>
#include<ctime>

class Process {
private:
    int initialCost{};
    int startDelay{};
    int typeOfProcess; // 0 dedicated, 1 universal

private:

public:

    Process() {
        this->startDelay = -1;
        this->initialCost = -1;
        this->typeOfProcess = -1;
    }

    Process(int initialCost, int startDelay, int typeOfProcess) : initialCost(initialCost), typeOfProcess(typeOfProcess),
                                                                startDelay(startDelay) {}

    int getInitialCost() const {
        return initialCost;
    }

    int getTypeOfProcess() const {
        return typeOfProcess;
    }

    int getStartDelay() const {
        return startDelay;
    }
};


#endif //TEST_1_PROCESS_H
