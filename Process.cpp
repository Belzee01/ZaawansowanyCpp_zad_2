//
// Created by Kajetan Lipensky on 2019-04-03.
//

#include "Process.h"

Process::Process(int typeOfProcess) : typeOfProcess(typeOfProcess), bandWith(0) {
    static bool seeded = false;
    if(!seeded) {
        srand(time(NULL));
        seeded = true;
    }
    if (typeOfProcess == 0)
        this->initialCost = rand() % (DEDICATED_MAX - DEDICATED_MIN) + DEDICATED_MIN;
    else
        this->initialCost = rand() % (UNIVERSAL_MAX - UNIVERSAL_MIN) + UNIVERSAL_MIN;
}

int Process::getBandWith() const {
    return bandWith;
}
