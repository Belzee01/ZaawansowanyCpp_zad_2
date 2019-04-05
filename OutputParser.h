//
// Created by Kajetan Lipensky on 2019-04-05.
//

#ifndef TEST_1_OUTPUTPARSER_H
#define TEST_1_OUTPUTPARSER_H

#include <fstream>
#include "InputBuilder.h"
#include <list>
#include "Task.h"

using namespace std;

class OutPutParser {
private:
    ofstream outputFile;
public:
    OutPutParser() {
        outputFile.open("output.txt");
    }

    virtual ~OutPutParser() {
        outputFile.close();
    }

    template<typename T>
    void writeTasks(InputBuilder *input, std::list<Task<T>> *tasksList) {
        outputFile << "@tasks " << input->getTasks() << "\n";
        for (int k = 0; k < input->getTasks(); ++k) {
            outputFile << "T" << k << ": " << tasksList[k].size() << " ";
            for (auto &it : tasksList[k]) {
                outputFile << it.getId() << "(" << it.getWeight() << ")" << " ";
            }
            outputFile << "\n";
        }
    }

    void writeProc(InputBuilder *input, list<Process> processes) {
        outputFile << "@proc " << input->getProc() << "\n";
        for (auto &p : processes) {
            outputFile << p.getInitialCost() << " " << p.getBandWith() << " " << p.getTypeOfProcess() << "\n";
        }
    }

    template<typename T>
    void writeTimes(InputBuilder *input, std::list<Task<T>> *tasksList) {
        outputFile << "@times " << "\n";
        for (int k = 0; k < input->getTasks(); ++k) {
            for (auto &it : tasksList[k]) {
                for (int i = 0; i < input->getProc(); ++i) {
                    outputFile << it.getTimes()[i] << "\t";
                }
                outputFile << "\n";
            }
        }
    }

    template<typename T>
    void writeCosts(InputBuilder *input, std::list<Task<T>> *tasksList) {
        outputFile << "@costs " << "\n";
        for (int k = 0; k < input->getTasks(); ++k) {
            for (auto &it : tasksList[k]) {
                for (int i = 0; i < input->getProc(); ++i) {
                    outputFile << it.getCosts()[i] << "\t";
                }
                outputFile << "\n";
            }
        }
    }

    void writeComm(InputBuilder *input, Communication *comms) {
        outputFile << "@comm " << input->getComm() << "\n";
        for (int k = 0; k < input->getComm(); ++k) {
            outputFile << "CHAN" << k << "\t" << comms[k].getCapacity() << "\t" << comms[k].getCost() << "\t";
            for (int i = 0; i < input->getProc(); ++i) {
                outputFile << comms[k].getProcConnections()[i] << " ";
            }
            outputFile << "\n";
        }
    }

    template<typename T>
    void
    writeAll(InputBuilder *input, std::list<Task<T>> *tasksList,const std::list<Process> &processes, Communication *comms) {
        writeTasks(input, tasksList);
        writeProc(input, processes);
        writeTimes(input, tasksList);
        writeCosts(input, tasksList);
        writeComm(input, comms);
    }
};


#endif //TEST_1_OUTPUTPARSER_H
