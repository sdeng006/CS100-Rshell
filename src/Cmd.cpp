#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include <cstdlib>
#include <string>
#include <iostream>

#include "Base.h"
#include "Cmd.h"

using namespace std;

Cmd::Cmd() { }

Cmd::Cmd(string cmdLine) {
    this->cmdLine = cmdLine;
}

// return cmdLine line
string Cmd::retCmdLine() {
    return this->cmdLine;
}

bool Cmd::execute() {
    
    // make space at begin and tail removed
    unsigned int begin = 0;
    unsigned int end = cmdLine.size() - 1;
    while (cmdLine.at(begin) == ' ') {
        begin++;
    }
    while (cmdLine.at(end) == ' ') {
        end--;
    }
    cmdLine = cmdLine.substr(begin, end + 1); // reset command in non-space
    
    // check for leaving
    // Exit.h and Exit.cpp not using for now
    if (cmdLine == "exit") {
        exit(0);
    }

    // code reference to "cplusplus.com/reference/cstring/strtok/"
    // command line be tokens, then char*
    vector<char*> flags; // vector holding parts
    char* pch = (char*)cmdLine.c_str();
    char* parts = strtok(pch, " ");
    while (parts != NULL) {
        flags.push_back(parts);
        parts = strtok(NULL, " ");
    }

    // Char* array to be passed to execvp()
    char** args = new char*[flags.size() + 1];

    for (unsigned int i = 0; i < flags.size(); i++) {
        args[i] = flags.at(i);
    }
    args[flags.size()] = NULL;

    int checker; // holder
    pid_t pid = fork(); // call fork for below pid cases

    // checking fork and execvp()
    if (pid == -1) {
        perror("fork failed");
        exit(1);
    }

    if (pid == 0) {
        if (execvp(args[0], args) == -1) {
            perror("execvp failed");
            exit(1);
        }
    }

    if (pid > 0) {
        waitpid(pid, &checker, 0);
        if (checker > 0) {
            return false; // execvp() failed due to checker failed
        }
        else if (WEXITSTATUS(checker) == 1) {
            return false; // false case
        }
        else if (WEXITSTATUS(checker) == 0) {
            return true; // true case
        }
    }

    cout << "Error: cmd.cpp reaches the last line." << endl;
    return false;
}