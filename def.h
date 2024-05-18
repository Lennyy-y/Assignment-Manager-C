/**************************************************************************/
/* def.h                                                                  */
/* This file contains GENERAL definitions used in several data structures */
/**************************************************************************/
#pragma once
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define MAX_LENGTH 255

// a value not supposed in stack and queue
#define		EMPTY	0x7FFFFFFF

// new types
typedef void* DATA;					// a type for data (easy to change)
typedef enum { False, True } BOOL;	// a boolean type
typedef unsigned char BYTE;

char* myStrdup(const char* c);
char* getStrExactLength(const char* msg);
char* myGets(char* buf, int size);
void generalArrayFunction(void* arr, int size, int typeSize, void(*func)(void* element));