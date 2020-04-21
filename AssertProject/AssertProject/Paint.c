#include "Paint.h"

#define BTN_IGNORE 1000
#define BTN_IGNORE_ALL 1001
#define BTN_EXIT 1002


LPCSTR szClassName = "Window1";
HWND hWnd;

LRESULT CALLBACK WndProc(HWND, UINT, UINT, LONG);

ATOM registerMyClass(HINSTANCE hInstance);

int assertLine;
char* assertFileName;
char* assertMessage;
OPT option;

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
        case BTN_EXIT:
          option = EXIT;
          DestroyWindow(hWnd);
          break;

        case BTN_IGNORE_ALL:
          option = IGNORE_ALL_ASSERTS;
          DestroyWindow(hWnd);
          break;

        case BTN_IGNORE:
          option = IGNORE_ASSERT;
          DestroyWindow(hWnd);
          break;
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

int createMyWindow(HINSTANCE hInstance, int nCmdShow, char* name, ASTP a, int line, char* fileName, char* message)
{
  registerMyClass(hInstance);

  assertLine = line;
  assertFileName = fileName;
  assertMessage = message;

  hWnd = CreateWindow(szClassName, name, WS_OVERLAPPEDWINDOW, 700, 400, 400, 250, NULL, NULL, hInstance, NULL);
  HWND hWndButton1, hWndButton2, hWndButton3;
  if (a == ASSERT)
  {
    hWndButton1 = CreateWindow("button", "ignore", WS_CHILD | WS_VISIBLE, 40, 100, 70, 20, hWnd, BTN_IGNORE, NULL, NULL);
    hWndButton2 = CreateWindow("button", "ignore all", WS_CHILD | WS_VISIBLE, 120, 100, 70, 20, hWnd, BTN_IGNORE_ALL, NULL, NULL);
    hWndButton3 = CreateWindow("button", "exit", WS_CHILD | WS_VISIBLE, 200, 100, 70, 20, hWnd, 1002, BTN_EXIT, NULL);
  }
  else
    hWndButton1 = CreateWindow("button", "exit", WS_CHILD | WS_VISIBLE, 120, 100, 70, 20, hWnd, BTN_EXIT, NULL, NULL);

  if (!hWnd) { return 0; }
  ShowWindow(hWnd, nCmdShow);
  UpdateWindow(hWnd);

  MSG msg;
  while (GetMessage(&msg, 0, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return option;
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
