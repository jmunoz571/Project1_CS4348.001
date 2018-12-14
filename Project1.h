/*-----------------------------------------------------------------
//Name:         Jose Munoz
//NetID:        jam151830@utdallas.edu
//Class:        CS4348
//Project:      Program 1 - 
//File:         Program1.h
//Notes:	Contains function prototypes, extern vars, etc
------------------------------------------------------------------*/
#ifndef PROJECT_1_H
#define PROJECT_1_H
#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h> /* needed for fork() */
#include <stdio.h>  /* needed for printf() and perror() */
#include <stdlib.h>
#include <regex>
#include "Memory.h"//Memory object
using namespace std;

//Function Prototypes
void initMem(fstream* , Memory*, int);
int parseString(string, bool*);
int proccessCPU( int*, int);
int fetch(int);
void store(int, int);
bool isInstruction(int);
//Extern Vars

#endif 
