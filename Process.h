//
// Created by Kajetan Lipensky on 2019-04-03.
//

#ifndef TEST_1_PROCESS_H
#define TEST_1_PROCESS_H

#include<iostream>
#include<cstdlib>
#include<ctime>

#define DEDICATED_MIN 800
#define DEDICATED_MAX 1200

#define UNIVERSAL_MIN 100
#define UNIVERSAL_MAX 300

#define DEDICATED_TIME_MIN 5
#define DEDICATED_TIME_MAX 100

#define UNIVERSAL_TIME_MIN 100
#define UNIVERSAL_TIME_MAX 1000

#define DEDICATED_COST_MIN 100
#define DEDICATED_COST_MAX 1000

#define UNIVERSAL_COST_MIN 5
#define UNIVERSAl_COST_MAX 100

class Process {
private:
    int initialCost;
    int bandWith;
    int typeOfProcess; // 0 dedicated, 1 universal

private:

public:

    Process(int typeOfProcess);

    Process(int initialCost, int typeOfProcess) : initialCost(initialCost), typeOfProcess(typeOfProcess) {}

    int getInitialCost() const {
        return initialCost;
    }

    int getTypeOfProcess() const {
        return typeOfProcess;
    }

    int getBandWith() const;

public:
    static int getDedicatedTimeMin() {
        return DEDICATED_TIME_MIN;
    }

    static int getDedicatedTimeMax() {
        return DEDICATED_TIME_MAX;
    }

    static int getUniversalTimeMin() {
        return UNIVERSAL_TIME_MIN;
    }

    static int getUniversalTimeMax() {
        return UNIVERSAL_TIME_MAX;
    }

    static int getDedicatedCostMin() {
        return DEDICATED_COST_MIN;
    }

    static int getDedicatedCostMax() {
        return DEDICATED_COST_MAX;
    }

    static int getUniversalCostMin() {
        return UNIVERSAL_COST_MIN;
    }

    static int getUniversalCostMax() {
        return UNIVERSAl_COST_MAX;
    }
};


#endif //TEST_1_PROCESS_H
