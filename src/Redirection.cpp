#include "Redirection.h"

ReCommand::ReCommand(string command, const char* fileName, int type) {
    
    // command2 is not using in this case
    // command as command1 is holding the string before the first space
    
    this->fileName = fileName;
    this->type = type;
    contains_pipe = false;
    
    size_t x = command.find(" ");
    
    // either removing space or not
    // no space
    if (x == string::npos) { 
        this->command = command;
    }
    // space located
    else {
        string tempCmd = command; 
        
        this->command = tempCmd.substr(0, x);
        tempCmd.erase(0, x + 1);
        
        char* stringToChar = new char[300]; 
        strcpy(stringToChar, tempCmd.c_str());
        char* temp = strtok(stringToChar, " ");
        
        // everything after space
        while (temp != NULL) {
            flags.push_back(temp);
            temp = strtok(NULL, " ");
        }
    }
}

ReCommand::ReCommand(string command, string command2, const char* fileName2, int type) {
    
    // if it memtions two fileName, string the first command holds the first fileName
    // and then the left ones are command2 or/and fileName2
    this->command2 = command2;
    this->fileName2 = fileName2;
    this->type = type;
    
    size_t x = command.find(" ");
    size_t y = command2.find("|");
    
    char* tempCmdCStr = new char[300];
    contains_pipe = true;
    if(y == string::npos) {
        contains_pipe = false;
        this->fileName = command2.c_str();
    }
    
    // this part same as the last one
    // no space
    if (x == string::npos) { 
        this->command = command;
    }
    // space located
    else {
        string tempCmd = command; 
        
        this->command = tempCmd.substr(0, x);
        tempCmd.erase(0, x + 1);
        
        char* stringToChar = new char[300]; 
        strcpy(stringToChar, tempCmd.c_str());
        char* temp = strtok(stringToChar, " ");
        
        // everything after space
        while (temp != NULL) {
            flags.push_back(temp);
            temp = strtok(NULL, " ");
        }
    }
}

bool ReCommand::execute() {
    // fix me!
    if(contains_pipe) {
        cout << "Pipe is not working yet" << endl;
        return true;
    }
    
    if (command == "exit") {
        exit(0);
    }
    
    char* x[300];
    char* temp = new char[200];
    strcpy(temp, command.c_str());  //copies command.c_str() onto temp with a NULL at the end
    x[0] = temp;
    
    unsigned size = flags.size();
    if (size != 0) {
		for (unsigned i = 1; i <= size; i++) {
			x[i] = flags[i - 1];
		}
    }
    
    x[flags.size() + 1] = NULL; 
    pid_t pid = fork();
    
    bool result = true;
    // fork fail case
    if (pid == -1) {
        result = false;
        perror("fork() has failed");
        exit(1);
    }
    
    // fork good case
    else if (pid == 0) {  
        int checker = 0;
        
        // type 1 : the < redirection case  
        if(type == 1) {
            string text;
            command = command + " " + fileName;
            
            // based on
            // http://www.sw-at.com/blog/2011/03/23/popen-execute-shell-command-from-cc/
            FILE* one = popen(command.c_str(), "r");
            if(!one) {
                cout<< "popen error!" << endl;
                exit(1);
            }
            
            char holding[500];
            
            while(fgets(holding, sizeof(holding), one) != NULL) {
                text = text + holding;   //  text << holding;
            }
            
            // output what have inputed
            cout << text <<endl;
            pclose(one);
        }
        
        // the > redirection case  
        else if(type == 2) {
            
            // 0644 means write and read
            // https://stackoverflow.com/questions/
            // 18415904/what-does-mode-t-0644-mean
            int two = 0;
            two = creat(fileName, 0644);
            
            // https://stackoverflow.com/questions/12902627/
            // the-difference-between-stdout-and-stdout-fileno-in-linux-c
            dup2(two, STDOUT_FILENO);
            checker = execvp(command.c_str(), x);
        }
        
        // the >> redirection case 
        else if(type == 3) {
            ifstream inFS;
            inFS.open(fileName); //added after it has been graded
            string text;
            if(inFS.is_open()) {
                string line;
                
                while(getline(inFS,line)) {
                    text = text + line + "\n";
                }
                char holding[500];
                FILE* p = popen(command.c_str(), "r");
                if(!p) {
                    cout<< "popen error!" << endl;
                    exit(1);
                }
                
                while(fgets(holding, sizeof(holding), p) != NULL) {
                    //  text << holding;
                    text = text + holding;
                }
                pclose(p);
                
                ofstream outFS;
                outFS.open(fileName);
                outFS << text;
                outFS.close();
            }
            
            // no file read, so create one
            else {
                int three = creat(fileName, 0644);
                dup2(three, STDOUT_FILENO);
                checker = execvp(command.c_str(), x);
            }
        }
        
        // < > case, holding all things
        else if(type == 4) {
            
            string text;
            char holding[512];
            // char const* commandCombined = command.c_str() + fileName;
            command = command + " " + fileName;
            FILE* p = popen(command.c_str(), "r");
            if(!p) {
                cout<< "popen error!" << endl;
                exit(1);
            }
            while(fgets(holding, sizeof(holding), p) != NULL) {
                text = text + holding;   //  text << holding;
            }
            //text contains the output that needs to be put into fileName2
            ofstream outFS;
            outFS.open(fileName2);
            outFS<<text;
        }
        
        //  < >> case
        else if(type == 5) {
            ifstream inFS;
            inFS.open(fileName);
            string text;
            
            command = command + " " + fileName;
            
            // write behind it
            if(inFS.is_open()) {
                string line;
                while(getline(inFS,line)) {
                    text = text + line + "\n";
                }
                
                char holding[500];
                FILE* p = popen(command.c_str(), "r");
                if(!p) {
                    cout<< "popen error!" << endl;
                    exit(1);
                }
                while(fgets(holding, sizeof(holding), p) != NULL) {
                    text = text + holding;   //  text << holding;
                }
                pclose(p);
                
                ofstream outFS;
                outFS.open(fileName2);
                outFS << text;
                outFS.close();
            }
            
            // create file
            else {
                int five = creat(fileName2, 0644);
                dup2(five, STDOUT_FILENO);
                checker = execvp(command.c_str(), x);
            }
        }
        else {
            cout<< "Error with int type in ReCommand" <<endl;
        }
        
        if (checker == -1) {
            perror("execvp() has failed");
            exit(1);
        }
    }
    else if (pid > 0) {
        int status;
        
        if (waitpid(pid, &status, 0) == -1) {
            cout << "waitpid does not work" << endl;
            exit(1);
        }
        
        // returns a nonzero value if the child process 
        // terminated normally with exit or _exit.
        if (WIFEXITED(status) == 1 && WEXITSTATUS(status) != 0) {
            result = false;
        }
    }
    
    return result;
}


