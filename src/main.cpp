#include <cstdlib>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <string>
#include <cstring>
#include <cstdio>

// open (redirect)
#include <fstream>
#include <sys/stat.h>
#include <fcntl.h>

#include "Base.h"
#include "Command.cpp"
#include "Connector.cpp"
#include "Test.cpp"
#include "ReCommand.cpp"

using namespace std;

const string BLANK = "";

/* cut everything in the input line into separated elements
   to remove the space
   finally return the result
   
   example
    echo A && echo B
        echo
        A
        &&
        echo
        B
*/
vector<string> cutter(const string& input) {
    vector<string> result; // hold commands and connectors
    
    // turn string into char*
    char* stringToChar = new char[300];
    strcpy(stringToChar, input.c_str());
    
    // remove space and separate it 
    char* holder = strtok(stringToChar, " ");
    while (holder != NULL) { 
        string temp = holder;
        result.push_back(temp);
        holder = strtok(NULL, " ");
    }
    
    return result;
}

/* this function is for connecting gathers
   build the connection between commands
   the return value hold the gathers
   example:
   
   before
    echo
    A
    &&
    echo
    B
    ||
    echo
    C

    after
    in vector:
        echo A 
        &&
        echo B 
        ||
        echo C
*/
vector<string> makeUnity (const vector<string>& seperated) {             
    string unitHolder; // hold units of command and connector
    vector<string> result; // returning result
    
    for (unsigned i = 0; i < seperated.size(); ++i) {
        
        // And case and Or case are similar
        if (seperated.at(i) == "&&") {
            if(unitHolder != BLANK) {
                result.push_back(unitHolder);
                unitHolder = BLANK;
            }
            result.push_back("&&");
        }
        else if (seperated.at(i) == "||") {
            if(unitHolder != BLANK) {
                result.push_back(unitHolder);
                unitHolder = BLANK;
            }
            result.push_back("||");
        }
         
        // Semi case: removing the last ";"
        // from recent command and push back as a new index element
        else if (seperated.at(i)[seperated.at(i).size() - 1] == ';') {
            if (unitHolder != BLANK) {
                unitHolder = unitHolder + seperated.at(i);
                unitHolder.erase((unitHolder.begin() + unitHolder.size() - 1));
                result.push_back(unitHolder);
                unitHolder = BLANK;
            }
            result.push_back(";");
        }
        
        // test and bracket case
        // i has [ location
        else if (seperated.at(i) ==  "[") {
            bool isHere = false; // ] is here?
            unsigned rhsIndex; // this holds ] location 
            
            // finding ]
            for (unsigned a = i; a != seperated.size(); a++) {
                // found!
                if (seperated.at(a) ==  "]") {
                    rhsIndex = a;
                    isHere = true;
                } 
            }
                // ] not found
                if (isHere == false) {
                    cout << "Error: bracket mistakes!" << endl;
                    result.pop_back();
                    break;
                }
            
            // add everything inside [  ] into unit
            for (unsigned b = i; b <= rhsIndex; b++) {   
                unitHolder = unitHolder + seperated.at(b) + " ";
            }
            result.push_back(unitHolder);
            
            unitHolder = BLANK;
            i = rhsIndex; // mistaken, i = rhsIndex + 1;
        }
        
        // parentheses case
        // fix me here
        else if (seperated.at(i)[0] == '(') {
            bool pFound = false;
            bool semiFound = false;
            
            while (1) {
                if (seperated.at(i)[seperated.at(i).size() - 1] == ';') {
                    semiFound = true;
                }
                if (seperated.at(i)[seperated.at(i).size() - 1] == ')') {
                    pFound = true;
                }
                
                if (!semiFound && !pFound) {
                    unitHolder = unitHolder + seperated.at(i) + " ";
                    i++;
                }
                else {
                    unitHolder = unitHolder + seperated.at(i);
                    break;
                }
            }
            
            if (pFound) {
                result.push_back(unitHolder);
                unitHolder = BLANK;
            }
            else if(semiFound) {
                unitHolder.erase((unitHolder.begin() + unitHolder.size() - 1));
                result.push_back(unitHolder);
                result.push_back(";");
            }
            
            unitHolder = BLANK;
        }
        
        // fix me here
        //ReCommand < , > , >> ==================================================================================================
        else if(seperated.at(i) ==  "<" || seperated.at(i) ==  ">" || seperated.at(i) ==  ">>")
        {
            if(seperated.at(i) ==  "<")
            {
                unsigned j = i+1;
                
                if(!unitHolder.empty()) 
                {
                    result.push_back(unitHolder);       //command cat, sort, etc
                    unitHolder.clear();
                }
                string pipes = "";
                result.push_back("<");
                while(j < seperated.size() )                     //search for mirror >
                {
                    if(seperated.at(j) == ">" || seperated.at(j) == ">>" 
                    || seperated.at(j) == "&&" || seperated.at(j) == "||" || seperated.at(j) == ";" )
                    {
                        result.push_back(pipes);
                        result.push_back(seperated.at(j));
                        i = j;
                        break;
                    }
                    pipes += seperated.at(j);
                    j++;
                }
            }
            else
            {
                if(!unitHolder.empty()) 
                {
                    result.push_back(unitHolder);
                    unitHolder.clear();
                }
                result.push_back(seperated.at(i));
            }
        }
        //ReCommand < , > , >> ==================================================================================================
        // fix me here
        
        // end command case 
        // make sure to push back it!
        else if ((i + 1) == seperated.size()) {
            unitHolder = unitHolder + seperated.at(i); // + " ";
            result.push_back(unitHolder);
            unitHolder = BLANK;
        }
        
        // nomarl command case
        else {
            // add them together and seperate it by space again
            unitHolder = unitHolder + seperated.at(i) + " ";
        }
    }
    
    // cout << "testing display:" << endl;
    // for(int i = 0; i < result.size(); i++) {
    //     cout << result.at(i) << endl;
    // }
    
    return result;
}

// build the tree
Base* buildtree(const vector<string>& vCommand) {
    
    vector<string> seperate;
    vector<string> comm;
    
    string element;
    string temp;
    
    Base* root; // we are going to return this
    
    bool rhs = false;
    
    for (unsigned i = 0; i < vCommand.size(); ++i) {
        if (rhs) {
            rhs = false;
            continue;
        }
        
        // and case
        else if (vCommand.at(i) == "&&") {
            if (vCommand.at(i + 1)[0] == '(') {
                element = vCommand.at(i + 1);
                element = element.substr(1, element.size() - 2);
                vector<string> cutterElement = cutter(element);
                vector<string> unities = makeUnity(cutterElement);
                root = new And(root, buildtree(unities));
                rhs = true;
            }
            else {   
                if (vCommand.at(i + 1).find("test") == 0) {
                    string holder = vCommand.at(i + 1);
                    Test* t = new Test(holder);
                    root = new And(root, t);
                }
                else if (vCommand.at(i + 1)[0] == '[') {
                    temp = "test";
                    string holder = vCommand.at(i + 1);
                    for (unsigned j = 1; holder.at(j) != ']'; j++) {
                        temp = temp + holder.at(j);
                    }
                    Test* t = new Test(temp);
                    root = new And(root, t);
                }
                else {
                    Command* c = new Command(vCommand.at(i + 1));
                    root = new And(root, c);
                }
                rhs = true;
            }
        }
        
        // or case
        else if (vCommand.at(i) == "||") {
            if (vCommand.at(i + 1)[0] == '(') {
                element = vCommand.at(i + 1);
                element = element.substr(1, element.size() - 2);
                vector<string> cutterElement = cutter(element);
                vector<string> unities = makeUnity(cutterElement);
                root = new Or(root, buildtree(comm));
                rhs = true;
            }
            else {
                if (vCommand.at(i + 1).find("test") == 0) {
                    string holder = vCommand.at(i + 1);
                    Test* t = new Test(holder);
                    root = new Or(root, t);
                }
                else if (vCommand.at(i + 1)[0] == '[') {
                    temp = "test";
                    string holder = vCommand.at(i + 1);
                    for (unsigned j = 1; holder.at(j) != ']'; ++j) {
                        temp = temp + holder.at(j);
                    }

                    Test* t = new Test(temp);
                    root = new Or(root, t);
                }
                else {
                    Command* c = new Command(vCommand.at(i + 1));
                    root = new Or(root, c);
                }
                rhs = true;
            }
        }
        
        // semi case
        else if (vCommand.at(i) == ";") {
            if ((i + 1) == vCommand.size()) {
                root = new Semicolon(root);
            }
            else if (vCommand.at(i + 1)[0] == '(') {
                element = vCommand.at(i + 1);
                element = element.substr(1, element.size() - 2);
                vector<string> cutterElement = cutter(element);
                vector<string> unities = makeUnity(cutterElement);
                root = new Semicolon(root, buildtree(unities));
                rhs = true;
            }
            else {
                if (vCommand.at(i + 1).find("test") == 0) {
                    string holder = vCommand.at(i + 1);
                    Test* t = new Test(holder);
                    root = new Semicolon(root, t);
                }
                else if (vCommand.at(i + 1)[0] == '[') {
                    temp = "test";
                    string holder = vCommand.at(i + 1);
                    for (unsigned j = 1; holder.at(j) != ']'; ++j) {
                        temp =temp + holder.at(j);
                    }

                    Test* t = new Test(temp);
                    root = new Semicolon(root, t);
                }
                else {
                    Command* c1 = new Command(vCommand.at(i + 1));
                    root = new Semicolon(root, c1);
                }
                rhs = true;
            }
        }
        
        // path case
        else if (vCommand.at(i)[0] == '(') {
            element = vCommand.at(i);
            element = element.substr(1, element.size() - 2);
            
            vector<string> cutterElement = cutter(element);
            vector<string> unities = makeUnity(cutterElement);
            root = buildtree(unities);
        }
        
        // others
        else {    
            // first [] test case
            if (vCommand.at(i)[0] == '[') {
                temp = "test";
                string holder = vCommand.at(i);
                for (unsigned j = 1; holder.at(j) != ']'; ++j) {
                    temp = temp + holder.at(j);
                }

                root = new Test(temp);
            }
            
            // first test case
            else if (vCommand.at(i).find("test") == 0) {
                root = new Test(vCommand.at(i));
            }
            
            // fix me
            // redirection case ===================================================================================================================
            else if((i + 2 < vCommand.size() ) && (vCommand.at(i+1) == ">" || vCommand.at(i+1) == ">>" || vCommand.at(i+1) == "<" ) )   //redirection
            {
                if(vCommand.at(i+1) == "<")                                                                              //input redirection
                {
                    if(i+4 < vCommand.size())
                    {
                        if(i<vCommand.size()-4 && vCommand.at(i+3) == ">>")   //type 5
                        {
                            /*
                            vCommand.at(i)       = command
                            vCommand.at(i+1)     = '<'
                            vCommand.at(i+2)     = command2
                            vCommand.at(i+3)     = ">>"
                            vCommand.at(i+4)     = fileName
                            */
                            ReCommand* c5 = new ReCommand(vCommand.at(i), vCommand.at(i+2),vCommand.at(i+4).c_str(),5);
                            root = c5;
                            i +=4;
                        }
                        else if(i<vCommand.size()-4 && vCommand.at(i+3) == ">")   //type 4
                        {
                            ReCommand* c4 = new ReCommand(vCommand.at(i), vCommand.at(i+2),vCommand.at(i+4).c_str(),4);
                            root = c4;
                            i +=4;
                        }
                        else       //type 1
                        {
                            /*
                                vCommand.at(i)       = command
                                vCommand.at(i+1)     = '<' 
                                vCommand.at(i+2)     = fileName
                            */
                            ReCommand* c1 = new ReCommand(vCommand.at(i),vCommand.at(i+2).c_str(),1);
                            root = c1;
                            i+=2;
                        }
                    }
                    else       //type 1
                    {
                        /*
                            vCommand.at(i)       = command
                            vCommand.at(i+1)     = '<' 
                            vCommand.at(i+2)     = fileName
                        */
                        ReCommand* c1 = new ReCommand(vCommand.at(i),vCommand.at(i+2).c_str(),1);
                        root = c1;
                        i+=2;
                    }
                }
                /*
                    vCommand.at(i)       = command
                    vCommand.at(i+1)     = '>' or ">>"
                    vCommand.at(i+2)     = fileName
                */
                else if(vCommand.at(i+1) == ">")         //type 2
                {
                    ReCommand* c2 = new ReCommand(vCommand.at(i),vCommand.at(i+2).c_str(),2);
                    root = c2;
                    i+=2;
                }
                else if(vCommand.at(i+1) == ">>")        //type 3
                {
                    ReCommand* c3 = new ReCommand(vCommand.at(i),vCommand.at(i+2).c_str(),3);
                    root = c3;
                    i+=2;
                }
            }
            // redirection case ===================================================================================================================
            // fix me
            
            // normal command case
            else {
                root = new Command(vCommand.at(i));
            }
        }
    }
    return root;
}

int main() {
	string userInput;
	vector<string> v;
	vector<string> cmd;

	while(1) {
	    
        cout << "$ ";
        getline(cin, userInput); 
        
        // if blank, skip it and continue loop
        if(userInput.empty()) {
            continue;
        }
        
        // remove # and what is behind
        // size_t checker = userInput.find('#'); // mistaken
        size_t commentChecker = userInput.find_first_of("#"); // find the beginning #
        if(commentChecker != string::npos) {
            userInput.erase(userInput.begin() + commentChecker, userInput.end());
        }
        
        v = cutter(userInput);
        cmd = makeUnity(v);
        
        Base* t1 = buildtree(cmd);
        t1->execute();
        v.clear();
        cmd.clear();
    }

	return 0;
}
