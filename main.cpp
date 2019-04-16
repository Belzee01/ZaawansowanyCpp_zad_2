#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <list>
#include "Task.h"
#include "TasksContainer.h"
#include "Process.h"
#include "Communication.h"
#include "Parsers.h"

using namespace std;

int main(int args, char **argv) {

    auto *parser = new Parsers();
    TasksContainer *taskContainer = parser->parse(argv[1]);

    taskContainer->printData();
}