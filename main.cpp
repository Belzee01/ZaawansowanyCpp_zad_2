#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


using namespace std;

class TaskParser {

};

class ProcParser {

};

class TimesParser {

};

class CostsParser {

};

class CommParser {

};

constexpr unsigned int str2int(const char *str, int h = 0) {
    return !str[h] ? 5381 : (str2int(str, h + 1) * 33) ^ str[h];
}

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
    string line;
    ifstream myfile(argv[1]);
    if (myfile.is_open()) {
        while (getline(myfile, line)) {
            if (line.rfind("@tasks") != std::string::npos) {
                tasks = extractInt(line);

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