#pragma once
#include <Windows.h>
#pragma warning(disable:4996)
typedef enum
{
  ASSERT, FATAL_ERR
}ASTP;

int createMyWindow(HINSTANCE hInstance, int nCmdShow, char* name, ASTP a, int line, char* filename, char* message);

typedef enum
{
  EXIT, IGNORE_ASSERT, IGNORE_ALL_ASSERTS
}OPT;
