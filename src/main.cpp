#include <iostream>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <limits>
#include <unistd.h>
#include <string>
#include <stack>


#include "Base.h"
#include "Cmd.cpp"
#include "Connector.cpp"
#include "And.cpp"
#include "Or.cpp"
#include "Semicolon.cpp"
#include "Exit.cpp"


using namespace std;


int main()
{
    char* pch;
    string str;  // get user input
    vector<char> connect_v;  //vector of all the connectors
    vector<string> v; //vector of all the words
    vector<string> cmd_v;   // vector of commands
    
   
   for(;;)
   {
    cout << "$ ";
    getline(cin,str);  // get user's command 
    
    
    for(unsigned i = 0; i < str.size(); ++i)
    {
        if(str.at(i) == ';' || str.at(i) == '|' || str.at(i) == '&' || str.at(i) == '#')
        {
            connect_v.push_back(str.at(i));
        }
    }
    
    connect_v.push_back(';');   // make sure the last one is ';'
    
    
    char* command = new char[str.length()+1];
    strcpy(command,str.c_str());
    
    pch = strtok (command, " ;|&");    // parse the string into vector of all the words
    while (pch != NULL)
    {
        v.push_back(pch);
        pch = strtok(NULL, " ;|&#");
    }
    
    unsigned odd = 1;   // used to help combine commands
    for(unsigned i = 0; i < (v.size()/2) ; ++i)  //set words into command vectors
    {
        
        cmd_v.push_back(v.at(2*i) + " " + v.at(odd));
        odd = odd + 2;
        
    }
    
    
    
    // //check
    //     cout << "v:" << endl;
    // for(unsigned i = 0; i<v.size(); ++i)
    // {
    //     cout << v.at(i) << "   ";
    // }
    //     cout << endl;
    //      cout << "connectors: " << endl;
    // for(unsigned i = 0; i<connect_v.size(); ++i)
    // {
    //     cout << connect_v.at(i) << "   ";
    // }
    //     cout << endl;
    //      cout << "cmd_v: " << endl;
    // for(unsigned i = 0; i<cmd_v.size(); ++i)
    // {
    //     cout << cmd_v.at(i) << "   ";
    // }
    //     cout<< endl;
    
    
    for(unsigned i = 0; i < cmd_v.size()-1; ++i)
    {
         Base* left = new Cmd (cmd_v.at(i));
         Base* right = new Cmd (cmd_v.at(i+1));

    
            if(connect_v.at(i) == ';')
            {
                Semicolon* sm = new Semicolon(left,right);
                sm->execute();
            }
            
            else if(connect_v.at(i) == '|')
            {
               connect_v.erase(connect_v.begin()+i);
                Or* o = new Or(left,right);
                o -> execute();
            }
        
            else if(connect_v.at(i) == '&')
            {
                connect_v.erase(connect_v.begin()+i);
                And* a = new And(left,right);
                a -> execute();
            }
            
            else if(connect_v.at(i) == '#')
            {
                cout << "comment: " << endl;
                break;
            }
    }
    
   }
    
}