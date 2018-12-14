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

enum modes{R,W};
enum registNames{PC, SP, IR, AC, X, Y};
enum pipeCmds{MODE, ADD, VAL};

int main(int argc, char *argv[])
{
  //Check and store command line arguments
  if(argc < 3){
    cout << "Not Enough Arguments" << endl;
    exit(0);
  }
  //Setup PIC pips
  int pipeMemToCpu[2];
  int pipeCpuToMem[2];

  //int buffer for pipe
  int buff[3];
  //  int buff2[3];
    
  //check to see if pipes sucessfully opened
  if(pipe(pipeMemToCpu) < 0 || pipe(pipeCpuToMem) < 0) {
    write(STDERR_FILENO, "Pipe(s) Failed\n", 14);
    exit(1);
  }

  //Variables
  Memory mem; //memory object to store the instructions 
  // int registers[6];


  //Read instructions from file
  fstream dataFile; 
  dataFile.open(argv[1], ios::in);
  //check to see if file opened properly
  if(!dataFile){
    cout << "Error - File did not open correctly. " << endl;
    exit(0);
  }
  
  //initialize the memory array
  //initMem(&dataFile, &mem, 2000);

  //mem.display(2);
  
  //setup PIC - pipes

  //swap new child process 
  pid_t pid = fork();
  switch(pid)
  {
  case -1: //fork failed
    cerr << "\tError - Fork returned an Error. "<< endl;
    write(STDERR_FILENO, "Fork Failed\n", 14);
    exit(2);
  case 0: //current process is the child (CPU Process)
    printf("I am the child: this.pid = %ld\n", (long) getpid());
    /*
    //set up two pipes to be used in stdin and stdou of child's
    dup2(pipeMemToCpu[0], 3);
    dup2(pipeCpuToMem[1], 4);
    */
    //Close uncessary pipes
    close(pipeMemToCpu[1]);
    close(pipeCpuToMem[0]);
 
    //CPU working 
    while(true){
      read(pipeMemToCpu[R], &buff, sizeof(buff));
      cout << "CPU: "<< buff[0];
      buff[2] = buff[0] + 1;
      write(pipeCpuToMem[W], &buff, sizeof(buff));
    }
    exit(1);
  default: //current process is the parent (Memory Process)
    printf("I am the parent: this.pid - %ld, child.pid - %ld\n",(long) getpid(), (long) pid);
    //close unused pipes
    close(pipeMemToCpu[R]);
    close(pipeCpuToMem[W]);

    //Mem does work
    int i = 0;
    while(i < 3){
      scanf("%d", &buff[0]);
      buff[1] = 0;
      buff[2] = 0;
      printf("Sent: %d, %d, %d. \n", buff[0], buff[1],  buff[2] );
      write(pipeMemToCpu[W], &buff, sizeof(buff));
      wait();
      read(pipeCpuToMem[R], &buff, sizeof(buff));
      printf("Received: %d, %d, %d. \n", buff[0], buff[1],  buff[2] );
      i++;
    }
    exit(3);
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
    }//if the current char is not a digit it is the begining of a comment
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
void initMem(fstream *file, Memory *mem, int size)
{
  string line = "";
  int i = 0;
  int x = 0;
  bool flag = false;
  while(!file->eof() && i < size)
  {
    getline(*file, line);
    x = parseString(line, &flag);
    //--cout << i << " - " << x << endl;
    if(flag){
      //--cout << "." << x << endl;
      //if "." is at the begining of the line, then the following number is the new index
      i = x; 
      //reset the dot flag and do not increment the index
      flag = false;
    }
    else{
      //store the value x at the current index i, and increment the index
      //--cout  << x << endl;
      if(x > -1){ //if the current line is not a space
	mem->write(i, x);
	i++;
      }
    }
  }
}

void rw(){
  cout << "\n" << endl;
}

int processCPU(int*registers, int value, Memory*mem){
  registers[0] = 0;
  mem->read(0);
  value++;
  switch(registers[IR])
    {
    case 1: //=Load value
      //Load the value into the AC
      
      break;
    case 2: //=Load addr
      //Load the value at the address into the AC
      break;
    case 3: //=LoadInd addr   
      //Load the value from the address found in the given address into the AC
      //(for example, if LoadInd 500, and 500 contains 100, then load from 100).
      break;
    case 4: //=LoadIdxX addr
      //Load the value at (address+X) into the AC. 
      //(for example, if LoadIdxX 500, and X contains 10, then load from 510).
      break;
    case 5: //=LoadIdxY addr
      //Load the value at (address+Y) into the AC
      break;
    case 6: //=LoadSpX
      //Load from (Sp+X) into the AC (if SP is 990, and X is 1, load from 991).
      break;
    case 7: //=Store addr
      break;
    case 8: //=Get
      //Gets a random int from 1 to 100 into the AC
      break;
    case 9: //=Put port
      //If port=1, writes AC as an int to the screen
      //If port=2, writes AC as an char to the screen
      break;
    case 10: //AddX
      //Add the value in X to the AC
      break;
    case 11: //AddY
      //Add the value in Y to the AC
      break;
    case 12: //SubX
      //Subtract the value in X from the AC
      break;
    case 13: //SubY
      //Subtract the value in Y from the AC
      break;
    case 14: //CopyToX
      //Copy the value in the AC to X
      break;
    case 15: //CopyFromX
      //Copy the value in X to the AC
      break;
    case 16: //CopyToY
      //Copy the value in the AC to Y

      break;
    case 17: //CopyFromY
      //Copy the value in Y to the AC
      break;
    case 18: //CopyToSp
      //Copy the value in AC to the SP

      break;
    case 19: //CopyFromSp
      //Copy the value in SP to the AC 

      break;
    case 20: //Jump addr
      //Jump to the address

      break;
    case 21: //JumpIfEqual addr
      //Jump to the address only if the value in the AC is zero

      break;
    case 22: //JumpIfNotEqual addr
      //Jump to the address only if the value in the AC is not zero

      break;
    case 23: //Call addr
      //Push return address onto stack, jump to the address

      break;
    case 24: //Ret
      //Pop return address from the stack, jump to the address

      break;
    case 25: //IncX
      //Increment the value in X

      break;
    case 26: //DecX
      //Decrement the value in X

      break;
    case 27: //Push
      //Push AC onto stack

      break;
    case 28: //Pop
      //Pop from stack into AC

      break;
    case 29: //Int
      //Perform system call

      break;
    case 30: //IRet
      //Return from system call

      break;
    case 50: //=End
      //End execution
       
      break;

    default: //-1
      //invalid instruction
      break;
    }
  return 0;
}
