#ifndef REDIRECTION_H   
#define REDIRECTION_H

#include "Base.h" 

// Redirection command: input and output
// this one hold the <, >, and >>
class ReCommand : public Base {
    private:
        // the second filename and command
        // are used for two files case
        const char* fileName;
        const char* fileName2;
        string command;
        string command2;
        
        bool contains_pipe;
        int type;
        
        vector<char*> flags;
    
    public:
        ReCommand(string command, const char* fileName, int type);
        ReCommand(string command, string command2, const char* fileName2, int type);
        bool execute();
};

#endif
