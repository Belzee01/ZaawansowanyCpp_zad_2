//
// Created by Kajetan Lipensky on 2019-04-03.
//

#include "InputBuilder.h"


InputBuilder::~InputBuilder() {

}

InputBuilder::InputBuilder() : tasks(0), proc(0), comm(0) {}

int InputBuilder::getTasks() {
    return tasks;
}

int InputBuilder::getProc() {
    return proc;
}

int InputBuilder::getComm() {
    return comm;
}
