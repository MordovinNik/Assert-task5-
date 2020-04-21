#include "Assert.h"

BOOLEAN ignoreAll = FALSE;
int assertsIgnored;
char** names;
int* lines;

int InitSystem(int count)
{
  lines = calloc(count, sizeof(char));

  if (!lines)
    return 1;

  names = calloc(sizeof(int)*count, sizeof(int));

  if (!lines)
  {
    free(lines);
    return 1;
  }

  assertsIgnored = 0;
  return 0;
}

void CloseSystem()
{
  free(lines);
  free(names);
}

void MyAssertFunc(unsigned int line, char* fileName, char* message)
{
  if (ignoreAll)
    return;

  for (int i = 0; i < assertsIgnored; i++)
  {
    if (!strcmp(fileName, names[i]) && lines[i] == line)
      return;
  }

  OPT option = createMyWindow(NULL, 1, "Error!", ASSERT, line, fileName, message);

  switch (option)
  {
    case EXIT:
      CloseSystem();
      exit(1);

    case IGNORE_ALL_ASSERTS:
      ignoreAll = TRUE;
      return;

    case IGNORE_ASSERT:
      names[assertsIgnored] = fileName;
      lines[assertsIgnored] = line;
      assertsIgnored++;
      return;

  }
}

void FatalErrorFunc(unsigned int line, char* fileName, char* message, int returnValue)
{
  createMyWindow(NULL, 1, "Fatal Error!", FATAL_ERR, line, fileName, message);

  CloseSystem();
  exit(returnValue);
}
