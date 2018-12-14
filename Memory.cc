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
void Memory::display(int opt){
  switch(opt)
    {
    case 2: //prints 2 colums
      for(int i= 0; i < 25; i++){
	printf("   0x%.3X - %3d   0x%.3X - %3d   0x%.3X - %3d   0x%.3X - %3d\n", 
	       i, mem[i], i+(25*1), mem[i+(25*1)], i+(25*2), mem[i+(25*2)], i+(25*3), mem[i+(25*3)]);
      }
      break;
    case 4:
      for(int i= 0; i < 250; i++){
	printf(" 0x%.3X - %2d   0x%.3X - %2d   0x%.3X - %2d   0x%.3X - %2d\n", 
	       i, mem[i], i+(250*1), mem[i+(250*1)], i+(250*2), mem[i+(250*2)], i+(250*3), mem[i+(250*3)]);
      }
      break;
    case 8:
      for(int i= 0; i < 250; i++){
	printf(" 0x%.3X - %2d   0x%.3X - %2d   0x%.3X - %2d   0x%.3X - %2d   0x%.3X - %2d   0x%.3X - %2d   0x%.3X - %2d   0x%.3X - %2d\n", 
	       i, mem[i], i+(250*1), mem[i+(250*1)], i+(250*2), mem[i+(250*2)], i+(250*3), mem[i+(250*3)], 
	       i+(250*4), mem[i+(250*4)], i+(250*5), mem[i+(250*5)], i+(250*6), mem[i+(250*6)], i+(250*7), mem[i+(250*7)]);
      }
      break;
    default:
      for(int i= 0; i < 250; i++){
	printf(" 0x%.3X - %2d   0x%.3X - %2d   0x%.3X - %2d   0x%.3X - %2d   0x%.3X - %2d   0x%.3X - %2d   0x%.3X - %2d   0x%.3X - %2d\n", 
	       i, mem[i], i+(250*1), mem[i+(250*1)], i+(250*2), mem[i+(250*2)], i+(250*3), mem[i+(250*3)], 
	       i+(250*4), mem[i+(250*4)], i+(250*5), mem[i+(250*5)], i+(250*6), mem[i+(250*6)], i+(250*7), mem[i+(250*7)]);
      }
      break;
    }
}
