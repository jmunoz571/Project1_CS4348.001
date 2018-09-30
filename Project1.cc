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


void initMem(fstream* , int*, int);
int parseString(string, bool*);
void printArray(int *);

int main(int argc, char *argv[])
{
  //Check and store command line arguments
  if(argc < 3){
    cout << "Not Enough Arguments" << endl;
    exit(0);
  }
  
  //Variables
  int mem[2000];

  //Read instructions from file
  fstream dataFile; 
  dataFile.open(argv[1], ios::in);
  //check to see if file opened properly
  if(!dataFile){
    cout << "Error - File did not open correctly. " << endl;
    exit(0);
  }
  
  //initialize the memory array
  initMem(&dataFile, mem, 2000);

  printArray(mem);


  //swap new child process 
  pid_t pid = fork();
  switch(pid)
  {
  case -1: //fork failed
    cerr << "\tError - Fork returned an Error. "<< endl;
    exit(1);
  case 0: //current process is the child
    printf("I am the child: pid - %ld\n", (long) getpid());
    exit(1);
  default: //current process is the parent
    printf("I am the parent: pid - %ld\n", (long) pid);
    exit(1);
  }

  //close file objects
  dataFile.close();
  return 0;
}


//given a string, it returns the first integer number in the line
int parseString(string line, bool *dotFlag)
{
  int size = line.length();
  string numb = "";
  char chr;
  for( int i = 0; i < size; i++)
  {
    chr = line.at(i);
    //if current character is a digit, add it to the number string
    if(chr == '.' && i == 0)
      *dotFlag = true;
    else if(isdigit(chr)){
      numb+= chr;
    }
    //if the current char is not a digit it is the begining of a comment
    else{
      break;
    }
  }
  //convert and return the number string to an int
  if(numb == "")
    return -1;
  else
    return atoi(numb.c_str());
}

//given a file stream and an int array, the function reads and stores the contents of the file in the int array
void initMem(fstream *file, int *mem, int size)
{
  string line = "";
  int i = 0;
  int x = 0;
  bool flag = false;
  while(!file->eof() && i < size)
  {
    getline(*file, line);
    //cout << i << "-" << line << endl;
    x = parseString(line, &flag);
    mem[i] = x; 
    if(flag){
      //cout << "." << x << endl;
      flag = false;
    }
    else{
      // cout  << x << endl;
    }
    i++;
  }
}


void printArray(int *mem)
{
  for(int i= 0; i < 25; i++){
    printf("%.2d-%.2d\t%.2d-%.2d\t%.2d-%.2d\n", i, mem[i], i+25, mem[i+25], i+50, mem[i+50]); 
  }
}
