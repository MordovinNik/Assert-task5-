#include "Assert.h"
#include <stdlib.h>

int main()
{
  InitSystem(10);

  for (int i = 0; i < 10; i++)
    MyAssert(0, "message1");

  MyAssert(0, "message2");
  MyAssert(1, "message3");

  FatalError(1, "Fatal Error2", 10);
  FatalError(0, "Fatal Error1", 10);

  system("pause");
}

