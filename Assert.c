#include <windows.h>
#include "Assert.h"

LPCSTR szClassName = "Window1";
HWND hWnd;
char* assertMessage;
char* assertFileName;
int assertLine;
BOOLEAN ignoreAll = FALSE;


struct {
  int assertsIgnored;
  char** names;
  int* lines;
}ignore;

int createMyWindow(HINSTANCE hInstance, int nCmdShow, char*);

int InitSystem(int count)
{
  ignore.lines = calloc(count, sizeof(char));

  if (!ignore.lines)
    return 1;

  ignore.names = calloc(sizeof(int)*count, sizeof(int));

  if (!ignore.lines)
  {
    free(ignore.lines);
    return 2;
  }

  ignore.assertsIgnored = 0;
  return 0;
}

void CloseSystem()
{
  free(ignore.lines);
  free(ignore.names);
}

void MyAssertFunc(unsigned int line, char* fileName, char* message)
{
  if (ignoreAll)
    return;

  for (int i = 0; i < ignore.assertsIgnored; i++)
  {
    if (!strcmp(fileName, ignore.names[i]) && ignore.lines[i] == line)
      return;
  }

  assertMessage = message;
  assertFileName = fileName;
  assertLine = line;

  createMyWindow(NULL, 1, "Error!");

  HWND hWndButton1 = CreateWindow("button", "ignore", WS_CHILD | WS_VISIBLE, 40, 100, 70, 20, hWnd, (HMENU)1000, NULL, NULL);
  HWND hWndButton2 = CreateWindow("button", "ignore all", WS_CHILD | WS_VISIBLE, 120, 100, 70, 20, hWnd, (HMENU)1001, NULL, NULL);
  HWND hWndButton3 = CreateWindow("button", "exit", WS_CHILD | WS_VISIBLE, 200, 100, 70, 20, hWnd, (HMENU)1002, NULL, NULL);


  MSG msg;
  while (GetMessage(&msg, 0, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return;

}

void FatalErrorFunc(unsigned int line, char* fileName, char* message, int returnValue)
{
  assertMessage = message;
  assertFileName = fileName;
  assertLine = line;

  createMyWindow(NULL, 1, "Fatal Error!");

  HWND hWndButton = CreateWindow("button", "exit", WS_CHILD | WS_VISIBLE, 120, 100, 70, 20, hWnd, (HMENU)1003, NULL, NULL);

  MSG msg;
  while (GetMessage(&msg, 0, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  exit(returnValue);
}

/// Winapi functions

LRESULT CALLBACK WndProc(HWND, UINT, UINT, LONG);

ATOM registerMyClass(HINSTANCE hInstance);

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  switch (msg)
  {
    case WM_DESTROY:
    {
      PostQuitMessage(0);
      break;
    }
    case WM_COMMAND:
    {
      switch (LOWORD(wParam))
      {
        case 1002:
          exit(0);

        case 1001:
          ignoreAll = TRUE;
          DestroyWindow(hWnd);

        case 1000:
          ignore.lines[ignore.assertsIgnored] = assertLine;
          ignore.names[ignore.assertsIgnored] = assertFileName;
          ignore.assertsIgnored++;
          DestroyWindow(hWnd);

        case 1003:
          DestroyWindow(hWnd);
      }

      break;
    }
    case WM_PAINT:
    {
      PAINTSTRUCT ps;
      HDC hdc = BeginPaint(hWnd, &ps);

      TextOut(hdc, 20, 20, assertMessage, strlen(assertMessage));

      char str[200];
      str[0] = 0;
      strcat(str, "File: ");
      strcat(str, assertFileName);

      TextOut(hdc, 20, 40, str, strlen(str));

      str[0] = 0;
      strcat(str, "Line: ");
      char strLine[100];
      itoa(assertLine, strLine, 10);
      strcat(str, strLine);

      TextOut(hdc, 20, 60, str, strlen(str));

      EndPaint(hWnd, &ps);
    }
  }
  return DefWindowProc(hWnd, msg, wParam, lParam);
}

int createMyWindow(HINSTANCE hInstance, int nCmdShow, char* name)
{
  registerMyClass(hInstance);

  hWnd = CreateWindow(szClassName, name, WS_OVERLAPPEDWINDOW, 700, 400, 400, 250, NULL, NULL, hInstance, NULL);

  if (!hWnd) { return 0; }
  ShowWindow(hWnd, nCmdShow);
  UpdateWindow(hWnd);
  return 0;
}

ATOM registerMyClass(HINSTANCE hInstance)
{
  WNDCLASS wc;
  wc.cbClsExtra = wc.cbWndExtra = 0;
  wc.style = 0;
  wc.lpfnWndProc = WndProc;
  wc.hInstance = hInstance;
  wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wc.lpszMenuName = NULL;
  wc.lpszClassName = szClassName;

  return RegisterClass(&wc);
}
