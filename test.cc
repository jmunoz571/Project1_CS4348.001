/*--------------------------------------------------------------
//Name:         Jose Munoz
//NetID:        jam151830@utdallas.edu
//Class:        CS4348
//Project:      Project 1 - Exploring Multiple Processes and IPC 
//Due:          09/29/2018
//File:         Project1 Pipe Testing
//Notes:
--------------------------------------------------------------*/
#include "Project1.h"

void rw();

int main(int argc, char *argv[])
{
  //setup PIC - pipes
  int pipefdMS[2];
  int pipefdSM[2];
  
  //initialize pipes and check for failure
  if( pipe(pipefdMS) < 0 ||  pipe(pipefdSM) < 0){
    write(STDERR_FILENO, "Pipe Failed\n", 14);
    exit(1);
  }
  
  char chr;
  char s;
  //swap new child process 
  pid_t pid = fork();
  switch(pid)
  {
  case -1: //fork failed
    cerr << "\tError - Fork returned an Error. "<< endl;
    write(STDERR_FILENO, "Fork Failed\n", 14);
    exit(2);
  
  case 0: //current process is the child
      printf("I am the child: pid - %ld\n", (long) getpid());
      //set up two pipes to be used in stdin and stdout
      dup2(pipefdMS[0], STDIN_FILENO);
      dup2(pipefdSM[1], STDOUT_FILENO);
      //Close the unsued pipes
      close(pipefdMS[0]);
      close(pipefdSM[1]);
      close(pipefdSM[0]);
      close(pipefdMS[1]);
      while(true){
	//read
	//read(pipefdMS[0], &s, 1);
	rw();
	//s = rw(s);
	//write(pipefdSM[1], &s, 1);
      }
      exit(1);

  default: //current process is the parent
    printf("I am the parent: pid - %ld\n", (long) pid);
    //close unused pipes
    close(pipefdMS[0]);
    close(pipefdSM[1]);
    while(true){
      if(chr == 'X')
	break;
      cin >>  chr;
      printf("Sent %c\n", chr);
      write(pipefdMS[1], &chr, 1);
      wait();
      read(pipefdSM[0], &chr, 1);
      printf("Read %c\n", chr);
    }
    exit(3);
  }
  //close file objects
  return 0;
}


void rw()
{
  char val;
  cin >> val;
  val++;
  cout << val;
}
