#ifndef TEST_H
#define TEST_H

#include <sys/stat.h>
#include <string>
#include <iostream>
#include "Command.h"

using namespace std;

class Test : public Command {
    private:
        string cmdLine;
        vector<char*> flags; // should only have 2 elements
        
    public:
        Test(string cmdLine);
        bool execute();
};

#endif