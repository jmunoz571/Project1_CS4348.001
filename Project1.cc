
/*--------------------------------------------------------------
//Name:         Jose Munoz
//NetID:        jam151830@utdallas.edu
//Class:        CS4348
//Project:      Project 1 - Exploring Multiple Processes and IPC 
//Due:          09/29/2018
//File:         Project1.cc
//Notes:
--------------------------------------------------------------*/
#include "Project1.h"

int main(int argc, char *argv[])
{
  //Check and store command line arguments
  if(argc < 3)
    cout << "Not Enough Arguments" << endl;
  
  cout << "ARG1: " << argv[1] << endl;
  cout << "ARG2: " << argv[2] << endl;
  
  //create processes
  pid_t forkvalue;
  forkvalue = fork();
  if(forkvalue == -1){
    cerr << "\tError - Fork returned an Error. "<< endl;
    exit(1);
  }
  else if(forkvalue == 0)
  {
    //current process is the child
    cout << "this is the child - pdi: "<< getpid() << endl;
            
    //close files
    exit(1);
  }
  else{ // current processs is the parent
    printf("I am the parent: pid - %ld\n", forkvalue);
    exit(1);
  }

  return 0;
}
