//
// Created by Kajetan Lipensky on 2019-04-23.
//

#ifndef ZAD2_DECISIONMAKER_H
#define ZAD2_DECISIONMAKER_H

#include "GraphStepper.h"
#include "TimesCostsMatrix.h"

class DecisionMaker {
private:

public:
    DecisionMaker();

    void setPreferredProcesses(GraphStepper stepper, int** indexArr, const TasksContainer& container);
};


#endif //ZAD2_DECISIONMAKER_H
