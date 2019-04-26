//
// Created by Kajetan Lipensky on 2019-04-03.
//

#ifndef ZAD2_PROCESS_H
#define ZAD2_PROCESS_H

#include<iostream>
#include<cstdlib>
#include<ctime>

template <class cost=int, class delay=int>
class Process {
private:
    cost initialCost{};
    delay startDelay{};
    int typeOfProcess; // 0 dedicated, 1 universal

private:
    int effectiveCost;

public:

    Process() {
        this->startDelay = -1;
        this->initialCost = -1;
        this->typeOfProcess = -1;
    }

    Process(cost initialCost, delay startDelay, int typeOfProcess) : initialCost(initialCost),
                                                                  typeOfProcess(typeOfProcess),
                                                                  startDelay(startDelay) {

        if (typeOfProcess == 0)
            effectiveCost = 0;
        else
            effectiveCost = initialCost;
    }

    cost getInitialCost() const {
        return initialCost;
    }

    int getTypeOfProcess() const {
        return typeOfProcess;
    }

    delay getStartDelay() const {
        return startDelay;
    }

    cost getEffectiveCost() const {
        return effectiveCost;
    }
};


#endif //TEST_1_PROCESS_H
