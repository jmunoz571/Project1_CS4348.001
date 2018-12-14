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
enum pipeCmds{MODE, ADD, INS, VAL};

int pipeMemToCpu[2]; 
int pipeCpuToMem[2];
int buff[4] = {0,0,0,0}; // M = read, Add - 0, Ins & Val are arbitrary vals for now

int main(int argc, char *argv[])
{
  //Check and store command line arguments
  if(argc < 3){
    cout << "Not Enough Arguments." << endl;
    exit(0);
  }
  //Setup PIC pipes
  
  //int buffer for pipe
    
  //call pipe() and check to see if pipes sucessfully opened
  if(pipe(pipeMemToCpu) < 0 || pipe(pipeCpuToMem) < 0) {
    write(STDERR_FILENO, "Pipe(s) Failed\n", 14);
    exit(1);
  }

  //Variables
  Memory mem; //memory object to store the instructions 
  bool done = false;
  //buff = {0,0,0,0};
  // int registers[6];
  int x = atoi(argv[2]); //store X value
  cout << x << endl;
  //Read instructions from file
  fstream dataFile; 
  dataFile.open(argv[1], ios::in);
  //check to see if file opened properly
  if(!dataFile){
    cout << "Error - File did not open correctly. " << endl;
    exit(0);
  }
  
  //initialize the memory array
  initMem(&dataFile, &mem, 2000);
  //close file object
  dataFile.close();
  mem.display(2);
  
  //setup PIC - pipes
  //spawn new process 
  pid_t pid = fork();
  switch(pid)
  {
  case -1: //fork failed
    cerr << "\tError - Fork returned an Error. "<< endl;
    write(STDERR_FILENO, "Fork Failed\n", 14);
    exit(2);
  case 0: //current process is the child (CPU Process)
    {
    printf("I am the child: this.pid = %ld\n", (long) getpid());
    //Close uncessary ends of both pipes
    close(pipeMemToCpu[W]);
    close(pipeCpuToMem[R]);
    int i = 0;
    //CPU working 
    while(i < 4){
      //Fetch the first instruction ( PC = 0) by writing a request to Memory
      int u = fetch(i);
      printf("Add Request: %d. --> ", u);
      //printf("Received: M=%d, A=%d, I=%d, V=%d. \n", buff[0], buff[1],  buff[2], buff[3]);
      store(i+5, 123);
      i++;
      //buff[ADD] = i;
    }
    done = true;
    }
  default: //current process is the parent (Memory Process)
    printf("I am the parent: this.pid - %ld, child.pid - %ld\n",(long) getpid(), (long) pid);
    //close unused pipes
    close(pipeMemToCpu[R]);
    close(pipeCpuToMem[W]);

    //Mem does work
    while(true){ 
      //Wait for CPU to write a request in order to un-block this process
      read(pipeCpuToMem[R], &buff, sizeof(buff));
      //if MODE = 0 (read from given address from mem)
      if(buff[MODE] == R){
	//store the value at the given address in the 'value' element of the int buff array
	buff[INS] = mem.read(buff[ADD]);
	buff[VAL] = mem.read(buff[ADD]+1);
      }else{ // MODE == 1 (write to mem), update the given address w/ the given 'value'
	mem.write(buff[ADD], buff[VAL]);
      }
      //printf("Current: M=%d, A=%d, I=%d, V=%d. \n", buff[0], buff[1],  buff[2], buff[3] );
      //write back the updated buffer / the same buffer to the pipe
      write(pipeMemToCpu[W], &buff, sizeof(buff));
      if(done == true){
	break; 
      }
      
    }
    wait();
  }
  //mem.display(2);
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
int processCPU(int*reg, int value){
  switch(reg[IR])
    {
    case 1: //=Load value
      //Load the value into the AC
      reg[AC] = value;
      break;
    case 2: //=Load addr
      //Load the value at the address into the AC
      reg[AC] = fetch(value);
      break;
    case 3: //=LoadInd addr   
      //Load the value from the address found in the given address into the AC
      //(for example, if LoadInd 500, and 500 contains 100, then load from 100).
      reg[AC] = fetch(fetch(value));
      break;
    case 4: //=LoadIdxX addr
      //Load the value at (address+X) into the AC. 
      //(for example, if LoadIdxX 500, and X contains 10, then load from 510).
      reg[AC] = fetch(value + reg[X]);
      break;
    case 5: //=LoadIdxY addr
      //Load the value at (address+Y) into the AC
      reg[AC] = fetch(value + reg[Y]);
      break;
    case 6: //=LoadSpX
      //Load from (Sp+X) into the AC (if SP is 990, and X is 1, load from 991).
      reg[AC] = fetch(reg[SP] + reg[X]);
      break;
    case 7: //=Store addr
      //Store the value in the AC into the address
      store(value, reg[AC]);
      break;
    case 8: //=Get
      //Gets a random int from 1 to 100 into the AC
      break;
    case 9: //=Put port
      //If port=1, writes AC as an int to the screen
      //If port=2, writes AC as an char to the screen
      if(value == 1)
	printf("%d", reg[AC]);
      else if(value == 2)
	printf("%c", reg[AC]);
      break;
    case 10: //AddX
      //Add the value in X to the AC
      reg[AC] += reg[X];
      break;
    case 11: //AddY
      //Add the value in Y to the AC
      reg[AC] += reg[Y];
      break;
    case 12: //SubX
      //Subtract the value in X from the AC
      reg[AC] -= reg[X];
      break;
    case 13: //SubY
      //Subtract the value in Y from the AC
      reg[AC] -= reg[Y];
      break;
    case 14: //CopyToX
      //Copy the value in the AC to X
      reg[X] = reg[AC];
      break;
    case 15: //CopyFromX
      //Copy the value in X to the AC
      reg[AC] = reg[X];
      break;
    case 16: //CopyToY
      //Copy the value in the AC to Y
      reg[Y] = reg[AC];
      break;
    case 17: //CopyFromY
      //Copy the value in Y to the AC
      reg[AC] = reg[Y];
      break;
    case 18: //CopyToSp
      //Copy the value in AC to the SP
      reg[SP] = reg[AC];
      break;
    case 19: //CopyFromSp
      //Copy the value in SP to the AC 
      reg[AC] = reg[SP];
      break;
    case 20: //Jump addr
      //Jump to the address
      reg[PC] = value;
      break;
    case 21: //JumpIfEqual addr
      //Jump to the address only if the value in the AC is zero
      if(reg[AC] == 0)
	reg[PC] = value;
      break;
    case 22: //JumpIfNotEqual addr
      //Jump to the address only if the value in the AC is not zero
      if(reg[AC] != 0)
	reg[PC] = value;
      break;
    case 23: //Call addr
      //Push return address onto stack, jump to the address
      store(reg[SP], reg[PC]);
      reg[SP]--;
      reg[PC] = value;
      break;
    case 24: //Ret
      //Pop return address from the stack, jump to the address
      reg[PC] = fetch(reg[SP]);
      reg[SP]++;
      break;
    case 25: //IncX
      //Increment the value in X
      reg[X]++;
      break;
    case 26: //DecX
      //Decrement the value in X
      reg[X]--;
      break;
    case 27: //Push
      //Push AC onto stack
      store(reg[SP], reg[AC]);
      reg[SP]--;
      break;
    case 28: //Pop
      //Pop from stack into AC
      reg[AC] = fetch(reg[SP]);
      reg[SP]++; 
      break;
    case 29: //Int
      //Perform system call
      
      break;
    case 30: //IRet
      //Return from system call

      break;
    case 50: //=End
      //End execution
      return 0; 
      break;
    default: //-1
      return 0;
      //invalid instruction
      break;
    }
  return 0;
}


//Given an address, it write a request to the memory process via the two pipes
int fetch(int address){
  buff[MODE] = R;
  buff[ADD] = address;
  write(pipeCpuToMem[W], &buff, sizeof(buff));
  read(pipeMemToCpu[R], &buff, sizeof(buff));
  return buff[VAL];
}

void store(int address, int data){
  buff[MODE] = W;
  buff[ADD] = address;
  buff[VAL] = data;
  write(pipeCpuToMem[W], &buff, sizeof(buff));
  //printf("Request Sent: M=%d, A=%d,I=%d, V=%d. --> ", buff[0], buff[1],  buff[2], buff[3] );
  read(pipeMemToCpu[R], &buff, sizeof(buff));
}

bool isInstruction(int ins){
  if( (ins > 0 && ins <= 30) || ins == 50 )
    return true;
  else
    return false;
}
