#include "tsh.h"

using namespace std;

void simple_shell::parse_command(char* cmd, char** cmdTokens) {
  // TODO: tokenize the command string into arguments

  //set cmdTokens to look at beginning of cmd
  *cmdTokens = cmd;
  //tokenize the cmd to get first word 
  *cmdTokens = strtok(cmd," \t\n"); //split based on only delimeters one sees in command line calls  //TODO : need cmdTokens++
  //loop until all args of command retrieved, use ++ to get to next ptr referenc
  while((*cmdTokens++ = strtok(NULL," \t\n")));  //stops at NULL
  
  
  /*//set up counter for loop iterations
  int count = 0;
  // break up the command string
  for var in $cmd; 
	    do
	      //for each arg place into cmdTokens array of chars 
	      cmdTokens[count] = "$var";
              //increment count
              count++;
	      done*/
}

void simple_shell::exec_command(char **argv)  //ERROR HERE Losing 10 points
{
  // TODO: fork a child process to execute the command.
  // parent process should wait for the child process to complete and reap it
  
  //create variables for pid, status (to check when child process done).
  //create child process by forking
  int pid = fork();
  int status;

  //check for failed fork
  if(pid < 0){
    //fork failed
    cout << "ERROR: fork() failed to create child process.";
    //quit code
    exit(1);
  }
  //check for successful fork
  else if(pid == 0){
    //success
    //execute arg
    execvp(*argv,argv);
  }
  else{
    //parent waiting for child proccess to finish to reap it
    while(wait(&status) != pid);
  }
}

bool simple_shell::isQuit(char *cmd){
  // TODO: check for the command "quit" that terminates the shell

  //set up boolean variable for tracking res
  bool res = false;
  //check for the command quit
  if(strcmp(cmd,"quit") == 0){  //or strcmp(arg1, arg2) == 0 ????
    //change value to indicate that quit was called
    res = true;
  }
  return res;
}
