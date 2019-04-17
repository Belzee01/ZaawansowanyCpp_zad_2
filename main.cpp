#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <list>
#include <vector>
#include <numeric>
#include "Task.h"
#include "TasksContainer.h"
#include "Process.h"
#include "Communication.h"
#include "Parsers.h"
#include "TimesCostsMatrix.h"

using namespace std;

int main(int args, char **argv) {

    auto *parser = new Parsers();
    TasksContainer *taskContainer = parser->parse(argv[1]);

    taskContainer->printData();

    cout << endl;

    auto tcMatrix = new TimesCostsMatrix(taskContainer->getTasksSize(), taskContainer->getProcSize());
    tcMatrix->evaluateMatrix(taskContainer->getTasks());

//    tcMatrix->printOutMatrix();

    tcMatrix->evaluateIndexArray();
    cout << endl;
    tcMatrix->printOutIndexMatrix();

    //TODO dodac algorytm przejscia po grafie do zadanego punktu z punktu poczatkowego, tak by miec liste/vector takich przejsc
    //TODO do ktorych bedzie mozna kolejno do kazedgo punktu w przejsciu przypozadkowac proces odpowiedzialny za punkt,
    //TODO a dzieki temu nastepnie wyznaczayc czy dane przejscie jest w gole mozliwe na podstawie komunikacji miedzy procesami
    //TODO czyli trzeba miec (punkt, proces) -> (punkt, proces) -> (punkt, proces), itd.
}