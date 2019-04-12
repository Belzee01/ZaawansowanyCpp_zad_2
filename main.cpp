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
    TaskParser() {}

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
    ProcParser() {}

    std::list<Process> parse(ifstream &file, int proc) {
        std::list<Process> procList;

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
    TimesParser() {}

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
    CostsParser() {}

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
    CommParser() {}

    std::list<Communication> parse(ifstream &file, int comm, int proc) {
        std::list<Communication> commList;

        for (int i = 0; i < comm; ++i) {
            string line;
            getline(file, line);

            int found = -1;
            stringstream ss;
            ss << line;

            int *connections = new int[proc];
            int cost;
            int capacity;
            int id;

            string temp;
            int k = 0;
            int count = 0;
            while (!ss.eof()) {
                ss >> temp;
                switch (count) {
                    case 0: {
                        if (stringstream(temp) >> found) {
                            id = found;
                        }
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
                        if (stringstream(temp) >> found) {
                            connections[k++] = found;
                        }
                    }
                        break;
                }

                temp = "";
                count++;
            }
            commList.push_back(*new Communication(id, cost, capacity, connections, proc));

        }
        return commList;
    }
};

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

int main(int args, char **argv) {

    int tasks;
    int proc;
    int comm;
    Task *taskArr = nullptr;

    auto taskParser = new TaskParser();
    auto procParser = new ProcParser();
    auto timeParser = new TimesParser();
    auto costParser = new CostsParser();
    auto commParser = new CommParser();

    TasksContainer *taskContainer = nullptr;

    string line;
    ifstream myfile(argv[1]);
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
                cout << "Proc: " << proc << endl;
                taskContainer->setProcesses(procParser->parse(myfile, proc));
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

    auto tasksAdjList = taskContainer->getTasksMap();

    for (auto it = tasksAdjList.begin(); it != tasksAdjList.end(); it++) {
        cout << it->first->getId() << ": ";
        for (auto &l: it->second) {
            cout << l->getId() << ", ";
        }
        cout << endl;
    }
    cout << endl;
    for (auto &p : taskContainer->getProcesses()) {
        cout << p.getInitialCost() << " " << p.getBandWith() << " " << p.getTypeOfProcess() << endl;
    }

    cout << endl;
    for (int i = 0; i < tasks; i++) {
        for (int j = 0; j < proc; ++j) {
            cout << taskContainer->getTasks()[i].getTimes()[j] << " ";
        }
        cout << endl;
    }
    cout << endl;

    for (int i = 0; i < tasks; i++) {
        for (int j = 0; j < proc; ++j) {
            cout << taskContainer->getTasks()[i].getCosts()[j] << " ";
        }
        cout << endl;
    }
    cout << endl;
    auto comms = taskContainer->getComm();
    for (auto &c: comms) {
        cout << c.getId() << " " << c.getCost() << " " << c.getCapacity() << " ";
        auto connections = c.getProcConnections();
        for (int i = 0; i < proc; ++i) {
            cout << connections[i] << " ";
        }
        cout << endl;
    }


    delete[] taskArr;
}