/*--------------------------------------------------------------
//Name:         Jose Munoz
//NetID:        jam151830@utdallas.edu
//Class:        CS4348
//Project:      Project 1 - Exploring Multiple Processes and IPC 
//Due:          09/29/2018
//File:         Memory.cc
//Notes:        Simple Memory object to store the instructions
--------------------------------------------------------------*/
#include "Memory.h"
using namespace std; 

//Prints the contents of the mem array
void Memory::display(){
  for(int i= 0; i < 250; i++){
    printf("[%.4d]-%.2d\t[%.4d]-%.2d\t[%.4d]-%.2d\t[%.4d]-%.2d\t[%.4d]-%.2d\t[%.4d]-%.2d\t[%.4d]-%.2d\t[%.4d]-%.2d\t", 
    i, mem[i], i+(250*1), mem[i+(250*1)], i+(250*2), mem[i+(250*2)], i+(250*3), mem[i+(250*3)], i+(250*4), mem[i+(250*4)], i+(250*5), mem[i+(250*5)], i+(250*6), mem[i+(250*6)], i+(250*7), mem[i+(250*7)]);
  }
}
