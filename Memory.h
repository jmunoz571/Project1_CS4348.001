/*-----------------------------------------------------------------
//Name:         Jose Munoz
//NetID:        jam151830@utdallas.edu
//Class:        CS4348
//Project:      Program 1  
//File:         Memory.h
//Notes:	Contains function prototypes, extern vars, etc
------------------------------------------------------------------*/
#ifndef MEMORY_H
#define MEOMRY_H
#include <iostream>
#include <fstream>
using namespace std;

class Memory
{
private:
  int mem[2000]; //array of 2000 integers to store the instructions

public:
  //Default Constructor
  Memory(){
    //inialize the array to zeroes
    for(int i = 0; i < 2000; i++){
      mem[i] = 0;
    }
  }

  //Accessors and Mutators

  //returns the value at the given address
  int read(int address){
    //if the address is in the valid range
    if(address < 2000){
      return mem[address]; //return the value at that address
    }else
      return -1; //otherwise return -1 to signify an error
  }

  //given an address, it updates the value at that address
  void write(int address, int data){
    //if the given address is in the valid range
    if(address < 2000){
      mem[address] = data;//update the entry
    }
  }
  //Methods
  void display();//prints the contents of the mem array

};

#endif //MOMORY_H
