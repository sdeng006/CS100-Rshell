#include "ReCommand.h"

ReCommand::ReCommand(string command, char const* fileName, int type) 
{
    this->fileName = fileName;
    this->type = type;
    string tempCmd;
    size_t x = command.find(" ");
    size_t y = command.find("|");
    char* tempCmdCStr = new char[500]; 
    if( y == string::npos)
    {
        contains_pipe = false;
    }
    else
    {
        contains_pipe = true;
    }
    if (x == string::npos) //npos is a static member constant value with the greatest 
    {                           // possible value for an element of type size_t.
        this->command = command;
    }
    else 
    {
        tempCmd = command; 
        
        this->command = tempCmd.substr(0, x);
        tempCmd.erase(0, x + 1);
        strcpy(tempCmdCStr, tempCmd.c_str());
        char* temp= strtok(tempCmdCStr, " ");
        
        while (temp != NULL) 
        {
            user_flags.push_back(temp);
            temp = strtok(NULL, " ");
        }
    }
    /*
        command = the string upto the first space
        user_flags = vector of char* that is composed of everything after the initial command
    */
}

ReCommand::ReCommand(string command, string command2, char const* fileName2, int type)
{
    //only used when: $ cat < file1.txt > file2.txt
    this->command2 = command2;
    this->fileName2 = fileName2;
    this->type = type;
    string tempCmd;
    size_t x = command.find(" ");
    size_t y = command2.find("|");
    char* tempCmdCStr = new char[500]; 
    if( y == string::npos)
    {
        contains_pipe = false;
        fileName = command2.c_str();
    }
    else
    {
        contains_pipe = true;
    }
    if (x == string::npos) //npos is a static member constant value with the greatest 
    {                           // possible value for an element of type size_t.
        this->command = command;
    }
    else 
    {
        tempCmd = command; 
        
        this->command = tempCmd.substr(0, x);
        tempCmd.erase(0, x + 1);
        strcpy(tempCmdCStr, tempCmd.c_str());
        char* temp= strtok(tempCmdCStr, " ");
        
        while (temp != NULL) 
        {
            user_flags.push_back(temp);
            temp = strtok(NULL, " ");
        }
    }
}



bool ReCommand::execute() {
    if (command == "exit") {
        exit(EXIT_SUCCESS);
    }
    
    int status;
    bool ran = true;
    char* x[500];
    char* temp = new char[100];
    //-------------------------------------------------------------------
    //                      IMPLEMENT LATER
    //-------------------------------------------------------------------
    if(contains_pipe)
    {
        if( type == 1 )             //      <    input redirection
        {
            /*
                sort < input.txt | tr a-z A-Z
            */
        }
        else if( type == 2 )        //      >    output redirection  (overwrite)
        {
            //get string ouput from pipes and then ofstream it to fileName
        }
        else if( type == 3 )        //      >>   output redirection  (append)
        {
            
        }
        else if( type == 4)         //      <   >   both
        {
            
        }
        else if( type == 5)         //      <   >>  both
        {
            
        }
        else
        {
            cout<<"ERROR with pipe or type: "<<type<<endl;
            return false;
        }
        return true;
    }
    strcpy(temp, command.c_str());  //copies command.c_str() onto temp with a NULL at the end
    x[0] = temp;
    
    if (user_flags.size() != 0) 
    {
		for (unsigned i = 1; i <= user_flags.size(); ++i) 
		{
			x[i] = user_flags[i - 1];
		}
    }
    
    x[user_flags.size() + 1] = NULL; 
    pid_t pid = fork();
    /*
    Description: fork creates a child process that differs from the parent process only in its PID and
    PPID, and in the fact that resource utilizations are set to 0. File locks and pending signals
    are not inherited.
    
    Returns: On success, the PID of the child process is returned in the parent’s thread of execution,
    and a 0 is returned in the child’s thread of execution.

    */
    
    if (pid == -1) //fork failed
    {
        ran = false;
        perror("fork() has failed");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)  //This is done by the child process.
    {  
        int blue = 0;
        if(type == 1)                       // <    input redirection  
        {
            // cout<<"Inside type 1 \n";
            string text = "";
            command += " ";
            command += fileName;
            // cout<<"command: \""<<command<<"\""<<endl;
            // blue = execvp(command.c_str(), x);   //doesn't work with command += fileName
            char buff[512];
            FILE* p = popen(command.c_str(), "r");
            if(!p)
            {
                cout<<"ERROR with popen()\n";
            }
            while(fgets(buff, sizeof(buff), p) != NULL)
            {
                text += buff;   //  text << buff;
            }
            cout<<text<<endl;
            pclose(p);
        }
        else if(type == 2)                  // >    output redirection  (overwrite)
        {
            int fd1 = creat(fileName, 0644);
            dup2(fd1, STDOUT_FILENO);
            blue = execvp(command.c_str(), x);
            close(fd1);
        }
        else if(type == 3)                  // >>   output redirection  (append)
        {
            ifstream inFS;
            inFS.open(fileName);//added after it has been graded
            string text = "";
            string line = "";
            if(inFS.is_open())          //apending
            {
                while(getline(inFS,line))
                {
                    text += line;
                    text += "\n";
                }
                char buff[512];
                FILE* p = popen(command.c_str(), "r");
                if(!p)
                {
                    cout<<"ERROR with popen()\n";
                }
                while(fgets(buff, sizeof(buff), p) != NULL)
                {
                    text += buff;   //  text << buff;
                }
                pclose(p);
                ofstream outFS;
                outFS.open(fileName);
                outFS<<text;
                
            }
            else                    //creating file
            {
                int fd1 = creat(fileName, 0644);
                dup2(fd1, STDOUT_FILENO);
                blue = execvp(command.c_str(), x);
                close(fd1);
            }
        }
        else if( type == 4)     //  <   >
        {
            string text = "";
            char buff[512];
            // char const* commandCombined = command.c_str() + fileName;
            command += " ";
            command += fileName;
            FILE* p = popen(command.c_str(), "r");
            if(!p)
            {
                cout<<"ERROR with popen()\n";
            }
            while(fgets(buff, sizeof(buff), p) != NULL)
            {
                text += buff;   //  text << buff;
            }
            //text contains the output that needs to be put into fileName2
            ofstream outFS;
            outFS.open(fileName2);
            outFS<<text;
        }
        else if( type == 5)
        {
            ifstream inFS;
            inFS.open(fileName);//added after it has been graded
            string text = "";
            string line = "";
            command += " ";
            command += fileName;
            if(inFS.is_open())          //apending
            {
                while(getline(inFS,line))
                {
                    text += line;
                    text += "\n";
                }
                char buff[512];
                FILE* p = popen(command.c_str(), "r");
                if(!p)
                {
                    cout<<"ERROR with popen()\n";
                }
                while(fgets(buff, sizeof(buff), p) != NULL)
                {
                    text += buff;   //  text << buff;
                }
                pclose(p);
                // cout<<"text: "<<text<<endl;
                ofstream outFS;
                outFS.open(fileName2);
                outFS<<text;
                
            }
            else                    //creating file
            {
                int fd1 = creat(fileName2, 0644);
                dup2(fd1, STDOUT_FILENO);
                blue = execvp(command.c_str(), x);
                close(fd1);
            }
        }
        else
        {
            cout<<"Error with int type in ReCommand"<<endl;
        }
        if (blue == -1)   ///* If execvp returns, it must have failed. */
        {
            ran = false;
            perror("execvp() has failed");
            exit(EXIT_FAILURE);
        }
    }
    else if (pid > 0)       /* This is run by the parent.  Wait for the child to terminate. */
    {
        if (waitpid(pid, &status, 0) == -1) //waiting for the child
        {
            perror("wait() has failed");
        }
        
        if (WIFEXITED(status) == 1) //returns a nonzero value if the child process 
        {                                   //terminated normally with exit or _exit.
            if (WEXITSTATUS(status) != 0) //WEXITSTATUS returns the child's exit status.
            {
                ran = false;
            }
        }
    }
    
    return ran;
}


