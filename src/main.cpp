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
    
   
   for(;;)
   {
       
    char* pch;
    string str;  // get user input
    vector<char> connect_v;  //vector of all the connectors
    vector<string> v; //vector of all the words
    vector<string> cmd_v;   // vector of commands
    
    
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
    
    if(v.size() <=1)
    {
        if(v.at(0) == "exit")
        {
            exit(0);
        }
        else{
            cout << "Please enter a command. " << endl;
            continue;
        }
    }
     
            //cout << "v size" << v.size() << endl;
    unsigned odd = 1;   // used to help combine commands
    for(unsigned i = 1; i <= (v.size()/2) ; ++i)  //set words into command vectors
    {
       // cout << "111111" << endl;
        if(v.size() == 2)
        {
            cmd_v.push_back(v.at(2*i-2) + " " + v.at(odd));
            break;
        }
        else
        {
          //  cout << "enter" << endl;
            cmd_v.push_back(v.at(2*i-2) + " " + v.at(odd));
          //  cout << "v.at(2*i-2): " << v.at(2*i-2) << endl;
            odd = odd + 2;
        }
    }
    
    
    
    // //check
    //   // cout << "v:" << endl;
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
    
    // //  cout << "cmd size: " << cmd_v.size() << endl;
    
    if(cmd_v.size() == 1)
    {
        Base* right = new Cmd (cmd_v.at(0));
            right -> execute();
    }
    
    for(unsigned i = 0; i < cmd_v.size()-1; ++i)
    {
               
               //cout << "check" << endl;
            
 
            
            // Base* left = new Cmd (cmd_v.at(i));
            // Base* right = new Cmd (cmd_v.at(i+1));
            //  if(connect_v.at(i) == ';')
            // {
            //     Semicolon* sm = new Semicolon(left);
            //     sm->execute();
            // }
            
            // else if(connect_v.at(i) == '|')
            // {
            //             //cout << "Enter OR!!" << endl;
            //   if(left->execute())
            //   {
            //     //   cout << "1111111" << endl;
            //   }
            //   connect_v.erase(connect_v.begin()+i);
            //         cout << "size:" << endl;
            //     Or* o = new Or(left,right);
            //     if(o -> execute() == !false)
            //     {
            //         //cout << "OR 222" << endl;
            //       //  right->execute();
            //     }
                
            //     if(connect_v.at(i+1) == '&')
            //     {
            //                 Base* right = new Cmd (cmd_v.at(i+2));
            //                 connect_v.erase(connect_v.begin()+i+1);
            //                 right ->execute();
            //     }
            // }
        
            // else if(connect_v.at(i) == '&')
            // {           
            //           // cout << "Enter ADD!!" << endl;
            //     connect_v.erase(connect_v.begin()+i);
            //     And* a = new And(left,right);
            //     if(a->execute() != false)
            //     {
            //      //   left->execute();
            //       // right->execute();
                    
            //             if(connect_v.at(i+1) == '|')
            //         {
            //             Base* right = new Cmd (cmd_v.at(i+2));
            //                 connect_v.erase(connect_v.begin()+i+1);
            //                 right ->execute();
            //         }
            //     }

            // }
            
   
         Base* left = new Cmd (cmd_v.at(i));
         Base* right = new Cmd (cmd_v.at(i+1));

    
            if(connect_v.at(i) == ';')
            {
                    //cout << ";" << endl;
                Semicolon* sm = new Semicolon(left,right);
                sm->execute();
            }
            
            else if(connect_v.at(i) == '|')
            {
                    //cout << "|" << endl;
               connect_v.erase(connect_v.begin()+i);
                Or* o = new Or(left,right);
                o -> execute();
            }
        
            else if(connect_v.at(i) == '&')
            {
                 //cout << "&" << endl;
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