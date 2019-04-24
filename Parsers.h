//
// Created by Belzee on 16.04.2019.
//

#ifndef ZAD2_PARSERS_H
#define ZAD2_PARSERS_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <list>
#include "Task.h"
#include "TasksContainer.h"
#include "Process.h"
#include "Communication.h"

using namespace std;

class TaskParser {
private:

public:
    TaskParser() = default;

    TasksContainer *parse(ifstream &file, int tasks, Task *taskArr) {
        std::map<Task *, std::list<Task *>> tasksMap;

        for (int i = 0; i < tasks; ++i) {
            string line;
            getline(file, line);

            int found = -1;
            stringstream ss;
            ss << line;

            string temp;
            while (!ss.eof()) {
                ss >> temp;
                if (stringstream(temp) >> found) {
                    for (int j = 0; j < found; ++j) {
                        ss >> temp;
                        int nodeNumber;
                        if (stringstream(temp) >> nodeNumber) {
                            tasksMap[&taskArr[i]].push_back(&taskArr[nodeNumber]);
                        }
                    }
                }
                temp = "";
            }
        }
        return new TasksContainer(tasks, tasksMap, taskArr);
    }
};

class ProcParser {

public:
    ProcParser() = default;

    std::vector<Process> parse(ifstream &file, int proc) {
        std::vector<Process> procList;

        for (int i = 0; i < proc; ++i) {
            string line;
            getline(file, line);

            int found = -1;
            stringstream ss;
            ss << line;

            int cost = -1, bandwith = -1, type = -1;
            ss >> cost >> bandwith >> type;
            procList.push_back(*new Process(cost, bandwith, type));

        }
        return procList;
    }
};

class TimesParser {
public:
    TimesParser() = default;

    void parse(ifstream &file, int tasks, int proc, Task *taskArr) {

        for (int i = 0; i < tasks; ++i) {
            string line;
            getline(file, line);

            int found = -1;
            stringstream ss;
            ss << line;

            int *times = new int[proc];

            string temp;
            int id = 0;
            while (!ss.eof()) {
                ss >> temp;
                if (stringstream(temp) >> found) {
                    times[id++] = found;
                }
                temp = "";
            }
            taskArr[i].setTimes(times);
        }
    }
};

class CostsParser {
public:
    CostsParser() = default;

    void parse(ifstream &file, int tasks, int proc, Task *taskArr) {

        for (int i = 0; i < tasks; ++i) {
            string line;
            getline(file, line);

            int found = -1;
            stringstream ss;
            ss << line;

            int *costs = new int[proc];

            string temp;
            int id = 0;
            while (!ss.eof()) {
                ss >> temp;
                if (stringstream(temp) >> found) {
                    costs[id++] = found;
                }
                temp = "";
            }
            taskArr[i].setCosts(costs);
        }
    }
};

class CommParser {

public:
    CommParser() = default;

    std::vector<Communication> parse(ifstream &file, int comm, int proc) {
        std::vector<Communication> commList;

        for (int i = 0; i < comm; ++i) {
            string line;
            getline(file, line);

            int found = -1;
            stringstream ss;
            ss << line;

            std::vector<int> connections;
            int cost;
            int capacity;
            int id = -1;

            string temp;
            int k = 0;
            int count = 0;
            while (!ss.eof()) {
                ss >> temp;
                switch (count) {
                    case 0: {
                        id = i;
                        ss.clear();
                    }
                        break;
                    case 1: {
                        if (stringstream(temp) >> found) {
                            cost = found;
                        }
                    }
                        break;
                    case 2: {
                        if (stringstream(temp) >> found) {
                            capacity = found;
                        }
                    }
                        break;
                    default: {
                        stringstream(temp) >> found;
                        if (found != -1) {
                            connections.push_back(found);
                            ss.clear();
                            found = -1;
                        }
                    }
                        break;
                }

                temp = "";
                count++;
            }
            commList.emplace_back(id, cost, capacity, connections, proc);

        }
        return commList;
    }
};

class Parsers {
private:
    int extractInt(const string &str) {
        stringstream ss;
        ss << str;
        string temp;
        int found;
        while (!ss.eof()) {
            ss >> temp;
            if (stringstream(temp) >> found)
                return found;
            temp = "";
        }
        return -1;
    }

private:
    Task *taskArr = nullptr;

public:
    Parsers() = default;

    virtual ~Parsers() {
        delete[] taskArr;
    }

    TasksContainer *parse(const char *filename) {
        int tasks;
        int proc;
        int comm;
        TasksContainer *taskContainer = nullptr;

        auto taskParser = new TaskParser();
        auto procParser = new ProcParser();
        auto timeParser = new TimesParser();
        auto costParser = new CostsParser();
        auto commParser = new CommParser();

        string line;
        ifstream myfile(filename);
        if (myfile.is_open()) {
            while (getline(myfile, line)) {
                if (line.rfind("@tasks") != std::string::npos) {
                    tasks = extractInt(line);
                    taskArr = new Task[tasks];
                    for (int k = 0; k < tasks; ++k) {
                        taskArr[k].setId(k);
                    }
                    taskContainer = taskParser->parse(myfile, tasks, taskArr);
                }
                if (line.rfind("@proc") != std::string::npos) {
                    proc = extractInt(line);
                    taskContainer->setProcesses(procParser->parse(myfile, proc), proc);
                }
                if (line.rfind("@times") != std::string::npos) {
                    timeParser->parse(myfile, tasks, proc, taskArr);
                }
                if (line.rfind("@cost") != std::string::npos) {
                    costParser->parse(myfile, tasks, proc, taskArr);
                }
                if (line.rfind("@comm") != std::string::npos) {
                    comm = extractInt(line);
                    taskContainer->setComm(commParser->parse(myfile, comm, proc));
                }
            }
            myfile.close();
        } else cout << "Unable to open file";

        return taskContainer;
    }
};

#endif //ZAD2_PARSERS_H
