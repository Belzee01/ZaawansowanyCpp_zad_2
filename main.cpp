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

    template<typename T>
    TasksContainer<T> *parse(ifstream &file, int tasks) {
        auto *tasksList = new std::list<Task<T>>[tasks];

        for (int i = 0; i < tasks; ++i) {
            string line;
            getline(file, line);

            int found = -1;
            stringstream ss;
            ss << line;

            string temp;
            tasksList[i].push_back(*new Task<T>(i));
            while (!ss.eof()) {
                ss >> temp;
                if (stringstream(temp) >> found) {
                    for (int j = 0; j < found; ++j) {
                        ss >> temp;
                        int nodeNumber;
                        if (stringstream(temp) >> nodeNumber)
                            tasksList[i].push_back(*new Task<T>(nodeNumber));
                    }
                }
                temp = "";
            }
        }
        return new TasksContainer<T>(tasks, tasksList);
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

    template<typename T>
    void parse(ifstream &file, int tasks, int proc, TasksContainer<T> *container) {

        for (int i = 0; i < tasks; ++i) {
            string line;
            getline(file, line);

            int found = -1;
            stringstream ss;
            ss << line;

            int *times = new int[proc];

            for (int j = 0; j < proc; ++j) {
                ss >> times[i];
            }
            &((Task<T>) container->getTasks()[i][0]).setTimes(times);
        }
    }
};

class CostsParser {

};

class CommParser {

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

    auto taskParser = new TaskParser();
    auto procParser = new ProcParser();
    auto timeParser = new TimesParser();

    TasksContainer<int> *taskContainer = nullptr;

    string line;
    ifstream myfile(argv[1]);
    if (myfile.is_open()) {
        while (getline(myfile, line)) {
            if (line.rfind("@tasks") != std::string::npos) {
                tasks = extractInt(line);
                taskContainer = taskParser->parse<int>(myfile, tasks);
            }
            if (line.rfind("@proc") != std::string::npos) {
                proc = extractInt(line);
                cout << "Proc: " << proc << endl;
                taskContainer->setProcesses(procParser->parse(myfile, proc));
            }
            if (line.rfind("@times") != std::string::npos) {

            }
            if (line.rfind("@cost") != std::string::npos) {

            }
            if (line.rfind("@comm") != std::string::npos) {
                comm = extractInt(line);
                cout << "Comm: " << comm << endl;
            }
        }
        myfile.close();
    } else cout << "Unable to open file";

    auto tasksAdjList = taskContainer->getTasks();

    for (int i = 0; i < tasks; ++i) {
        for (auto &l: tasksAdjList[i]) {
            cout << l.getId() << ", ";
        }
        cout << endl;
    }
    cout << endl;
    for (auto &p : taskContainer->getProcesses()) {
        cout << p.getInitialCost() << " " << p.getBandWith() << " " << p.getTypeOfProcess() << endl;
    }

    cout << endl;
    for (int i = 0; i < tasks; ++i) {
        for (auto &t: taskContainer->getTasks()[i]) {
            for (int j = 0; j < proc; ++j) {
                cout << t.getTimes()[j] << " ";
            }
        }
        cout << endl;
    }
}