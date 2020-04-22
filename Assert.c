#include "Assert.h"

BOOLEAN ignoreAll = FALSE;
int assertsIgnored;
struct ASSERTINFO
{
  char* name;
  int line;
}*ignoreAssertInfo;


int InitSystem(int count)
{
  ignoreAssertInfo = calloc(count, sizeof(ignoreAssertInfo));

  if (!ignoreAssertInfo)
    return 1;

  assertsIgnored = 0;
  return 0;
}

void CloseSystem()
{
  free(ignoreAssertInfo);
}

void MyAssertFunc(unsigned int line, char* fileName, char* message)
{
  if (ignoreAll)
    return;

  for (int i = 0; i < assertsIgnored; i++)
  {
    if (!strcmp(fileName, ignoreAssertInfo[i].name) && ignoreAssertInfo[i].line == line)
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
      ignoreAssertInfo[assertsIgnored].name = fileName;
      ignoreAssertInfo[assertsIgnored].line = line;
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
