#ifndef COMMAND_H   
#define COMMAND_H

#include "Base.h" 
#include <string>

using namespace std;

class Command : public Base {
    protected: 
        string cmdLine; // used to get user's input line for whole
        vector<char*> flags; // holder step by step
    
    public:
        Command();
        Command(string cmdLine);
        // string retCmdLine(); // return string cmdLine
        bool execute(); // no need for vir
};

#endif