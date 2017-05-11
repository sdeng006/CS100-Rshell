# Rshell file - UCR CS100


### Authors
Shuhuai Deng (Danny) - sdeng006@ucr.edu

Hui Gu (Whitney) - hgu004@ucr.edu

## Summary

  This program is a command shell which perform the same task as a bash shell. This could perform mutiple commands using "||", "&&" or ";", and exit by typing "exit".This program perfrom the commands such as -ls, echo, etc., but not cd. 
      
      - "||": would execute the next command if the first command fails
      
      - "&&": would exectue both command
      
      - ";" : would exectue the command before it
      
      
## Examples:
    
        $ ls -a
        $ echo hello
        $ mkdir test

        is equivalent to:

        $ ls -a; echo hello; mkdir test
        
        
## To run the program:
  
      - go to terminal
      
      - git clone https://github.com/sdeng006/rshell.git
      
      - cd rshell
      
      - git checkout hw2
      
      - make
      
      - bin/rshell
      
## Known bugs

      - The Exit.h and Exit.cpp are not working, so we replace a loop to check if input is "exit" for now
      
      - The function will display '$' a couple times after the tests file
  
