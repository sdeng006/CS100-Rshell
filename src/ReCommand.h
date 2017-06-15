#ifndef RECOMMAND_H   
#define RECOMMAND_H

#include "Base.h" 

/*
TYPE
    1.  <       input redirection   
    2.  >       output redirection      >    (overwrite)
    3.  >>      output redirection      >>   (append)
    4.  <   >   
    5.  <   >>  
*/


class ReCommand : public Base {
    protected: 
        string command;
        string command2;            //only used when: $ cat < file1.txt > file2.txt
        char const* fileName;
        char const* fileName2;
        bool contains_pipe;
        int type;
        vector<char*> user_flags;
    
    public:
        ReCommand(string command, char const* fileName, int type);
        ReCommand(string command, string command2, char const* fileName2, int type);
        bool execute();
};



#endif