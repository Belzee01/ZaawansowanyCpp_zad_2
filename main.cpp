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

    void parse(ifstream &file, int tasks) {
        list<Task<int>> tasksList;

        string line;
        getline(file, line);

        int found;
        stringstream ss;
        ss << line;
        string temp;
        while (!ss.eof()) {
            ss >> temp;
            //if (stringstream(temp) >> found)
                 //cout<<found;
            cout<<temp << "\n";
            temp = "";
        }
    }
};

class ProcParser {

};

class TimesParser {

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

    string line;
    ifstream myfile(argv[1]);
    if (myfile.is_open()) {
        while (getline(myfile, line)) {
            if (line.rfind("@tasks") != std::string::npos) {
                tasks = extractInt(line);
                taskParser->parse(myfile, tasks);
            }
            if (line.rfind("@proc") != std::string::npos) {
                proc = extractInt(line);
            }
            if (line.rfind("@times") != std::string::npos) {

            }
            if (line.rfind("@cost") != std::string::npos) {

            }
            if (line.rfind("@comm") != std::string::npos) {
                comm = extractInt(line);
            }
        }
        myfile.close();
    } else cout << "Unable to open file";

}