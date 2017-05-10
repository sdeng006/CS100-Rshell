#ifndef CMD_H
#define CMD_H

#include "Base.h"
#include <string>

using namespace std;

class Cmd : public Base {
    private:
        string cmdLine; // used to get user's input line for whole
    public:
        Cmd();
        Cmd(string cmdLine);
        
        string retCmdLine(); // return string cmdLine
        bool execute(); // no need for vir
};

#endif