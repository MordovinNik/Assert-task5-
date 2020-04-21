#pragma once
#include <string.h>
#include "AssertProject/AssertProject/Paint.h"

#ifdef _DEBUG
#define MyAssert(condition, message) if(!(condition))MyAssertFunc(__LINE__,__FILE__,(message));;
#else
#define MyAssert(condition,message)
#endif // DEBUG
#define FatalError(condition,message,returnValue) if(!(condition))FatalErrorFunc(__LINE__,__FILE__,(message),(returnValue));;

void MyAssertFunc(unsigned int line, char* fileName, char* message);

int InitSystem(int count);

void FatalErrorFunc(unsigned int line, char* fileName, char* message, int returnValue);

void CloseSystem();