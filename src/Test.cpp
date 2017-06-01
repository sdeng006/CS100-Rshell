#include "Test.h"

Test::Test(string input) {

    if (input.find(" ") == string::npos) {
        this->cmdLine = input; // the case nothing behind test
        return;
    }
    
    size_t location = input.find(" ");
    string tempInput = input;
    
    this->cmdLine = tempInput.substr(0, location); // copy the "test"
    tempInput.erase(0, location + 1); // remove the "test "
    
    char* command = new char[300]; // flag and path
    strcpy(command, tempInput.c_str()); // copy from string to char*
        
    // process to take token
    char* temp = strtok(command, " ");
    while (temp != NULL) {
        this->flags.push_back(temp); // should hold flag and path
        temp = strtok(NULL, " ");
    }
}


bool Test::execute() {
    string f; // flag
    char* p; // path
    
    if (this->flags.size() == 1) { // the case for no flag
        f = "-e"; // default "-e"
        p = this->flags[0]; // hold path
    }
    else if (this->flags.size() == 2) {
        f.assign(this->flags[0], 2); // hold flag
        p = this->flags[1]; // hold path
    }
    else if (this->flags.size() == 0) {
        return false;
    }
    else {
        cout << "Error: invalid input" << endl;
        exit(1);
    }


    struct stat checker;
    bool result = false; // default
    
    // Based on https://linux.die.net/man/2/stat
    // and http://codewiki.wikidot.com/c:system-calls:stat
    // check the file type using the st_mode field:
    bool ch = (stat(p, &checker) < 0);
    if (!ch) {
        if (f == "-e"){
            result = true;
        }
        else if (f == "-d") {
            result = S_ISDIR(checker.st_mode);
        }
        else if (f == "-f") {
            result = S_ISREG(checker.st_mode);
        }
        else {
            cout << "Error: invalid flag" << endl;
            exit(1);
        }
    }
    
    if (result == true) {
        cout << "(True)" << endl;
        return result;
    }
    else {
        cout << "(False)" << endl;
        return result;
    }
} 