

#ifndef TEST_1_INPUTMODEL_H
#define TEST_1_INPUTMODEL_H


class InputBuilder {
private:
    int tasks;
    int proc;
    int comm;

public:
    InputBuilder();

    virtual ~InputBuilder();

    InputBuilder *withTasks(int tasks) {
        this->tasks = tasks;
        return this;
    }

    InputBuilder *withProc(int proc) {
        this->proc = proc;
        return this;
    }

    InputBuilder *withComm(int comm) {
        this->comm = comm;
        return this;
    }

    int getTasks();

    int getProc();

    int getComm();
};


#endif //TEST_1_INPUTMODEL_H
