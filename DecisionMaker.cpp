//
// Created by Kajetan Lipensky on 2019-04-23.
//

#include "DecisionMaker.h"

DecisionMaker::DecisionMaker() = default;

vector<Path>
DecisionMaker::establishPreferredProcesses(const GraphStepper &stepper, int **indexArr, TasksContainer container) {
    vector<vector<Task *>> tasksPaths = stepper.getGlobalPaths();
    const vector<Process> &processes = container.getProcesses();

    vector<vector<TaskIdProcessId>> establishedTaskPaths;

    for (auto &tasksPath : tasksPaths) {
        vector<TaskIdProcessId> currentPath;
        int currentIndex = 0;
        for (auto &t: tasksPath) {
            int communicationId = -1;
            int bestValueIndex = -1;
            if (!currentPath.empty()) {
                int k = 0;
                while (communicationId == -1 and k < processes.size()) {
                    bestValueIndex = indexArr[t->getId()][k];
                    communicationId = container.getBestPossibleConnection(bestValueIndex, currentPath.at(
                            currentIndex - 1).getProcessId());
                    k++;
                }
                if (communicationId == -1) {
                    currentPath.clear();
                    break;
                }
            } else {
                bestValueIndex = indexArr[t->getId()][0];
            }
            currentPath.emplace_back(t->getId(), bestValueIndex, communicationId);
            currentIndex++;
        }
        if (!currentPath.empty()) {
            currentPath.at(0).setComm(currentPath.at(1).getComm());
            establishedTaskPaths.push_back(currentPath);
        }
    }

    vector<Path> paths;
    for(auto &path: establishedTaskPaths) {
        paths.emplace_back(path);
    }

    return paths;
}

vector<Path>
DecisionMaker::calculateFinalCostAndTime(vector<Path> paths, TasksContainer container) {

    for (auto &path : paths) {
        auto finalCostTime = FinalCostAndTime();
        for (auto &obj: path.getPath()) {
            finalCostTime.addCost(obj, container);
        }
        path.setCostAndTime(finalCostTime);
    }

    return paths;
}

vector<OutputData>
DecisionMaker::prepareForOutput(vector<Path> paths, TasksContainer container) {
    vector<OutputData> all;

    for (auto &path:paths) {
        vector<vector<int>> tasksSortedByProcesses(container.getProcesses().size());
        for (auto &obj: path.getPath()) {
            tasksSortedByProcesses[obj.getProcessId()].push_back(obj.getTaskId());
        }
        all.emplace_back(tasksSortedByProcesses, path.getCostAndTime());
    }

    return all;
}

void FinalCostAndTime::addCost(TaskIdProcessId obj, TasksContainer container) {
    if (std::find(communicationAlreadyPaid.begin(), communicationAlreadyPaid.end(), obj.getComm()) !=
        communicationAlreadyPaid.end()) {

    } else {
        communicationAlreadyPaid.push_back(obj.getComm());
        cost += container.getComm().at(obj.getComm()).getCost();
    }
    cost += container.getProcesses().at(obj.getProcessId()).getEffectiveCost();
    cost += container.getTasks()[obj.getTaskId()].getCosts()[obj.getProcessId()];
    time += container.getTasks()[obj.getTaskId()].getTimes()[obj.getProcessId()];
}
