
// clang-format off
#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
// clang-format on

#include "board.h"
#include "game.h"

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

// get the system message box font
HFONT getFont(void) {
  NONCLIENTMETRICSA ncm;
  // subtract sizeof iPaddedBorderWidth for winxp support
  ncm.cbSize = sizeof(ncm) - sizeof(ncm.iPaddedBorderWidth);
  if (SystemParametersInfo(SPI_GETNONCLIENTMETRICS,
                           sizeof(ncm) - sizeof(ncm.iPaddedBorderWidth), &ncm,
                           0))
    return CreateFontIndirect(&ncm.lfMessageFont);
  return NULL;
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR argument,
                   int cmdShow) {
  (void)prevInstance;
  (void)argument;

  HWND hwnd;
  MSG messages;
  WNDCLASSEX wincl;
  INITCOMMONCONTROLSEX icc;

  gameStruct game;

  icc.dwSize = sizeof(icc);
  icc.dwICC = ICC_STANDARD_CLASSES;
  InitCommonControlsEx(&icc);

  wincl.hInstance = instance;
  wincl.lpszClassName = "blox_gdi";
  wincl.lpfnWndProc = WindowProcedure;
  wincl.style = CS_DBLCLKS | CS_OWNDC;
  wincl.cbSize = sizeof(wincl);
  wincl.hIcon = NULL;   // LoadIcon(hThisInstance, MAKEINTRESOURCE(2));
  wincl.hIconSm = NULL; // LoadIcon(hThisInstance, MAKEINTRESOURCE(2));
  wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
  wincl.lpszMenuName = NULL;
  wincl.cbClsExtra = 0;
  wincl.cbWndExtra = 0;
  wincl.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);

  if (!RegisterClassEx(&wincl))
    return 0;

  hwnd = CreateWindowEx(WS_EX_LEFT, "blox_gdi", "blox_gdi",
                        WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU,
                        CW_USEDEFAULT, CW_USEDEFAULT, 255, 475, HWND_DESKTOP,
                        NULL, instance, NULL);

  if (game_init(&game) == false) {
    MessageBox(hwnd, "Failed to allocate memory for board", "blox_gdi",
               MB_ICONERROR);
    return 0;
  }

  SetLastError(0);
  if (SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&game) == 0 &&
      GetLastError() != 0) {
    MessageBox(hwnd, "SetWindowLongPtr failed", "blox_gdi", MB_ICONERROR);
    game_free(&game);
    return 0;
  }

  game_start(&game);
  game_step(&game);
  SetTimer(hwnd, 1, 300, NULL);

  ShowWindow(hwnd, cmdShow);
  while (GetMessage(&messages, NULL, 0, 0)) {
    if (!IsDialogMessage(hwnd, &messages)) {
      TranslateMessage(&messages);
      DispatchMessage(&messages);
    }
  }

  game_free(&game);

  return messages.wParam;
}

void paint(HWND hwnd, HDC dc) {
  gameStruct *game = (gameStruct *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
  if (game) {
    game_paint(game, dc);
  }
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam,
                                 LPARAM lParam) {
  PAINTSTRUCT ps;
  HDC dc;
  gameStruct *game;
  static bool left, right, up, down = false;
  switch (message) {
  case WM_GETDLGCODE:
    return DLGC_WANTALLKEYS;
  case WM_TIMER:
    game = (gameStruct *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    if (game) {
      game_step(game);
      InvalidateRect(hwnd, NULL, false);
    }
    return 0;
  case WM_PAINT:
    dc = BeginPaint(hwnd, &ps);
    paint(hwnd, dc);
    EndPaint(hwnd, &ps);
    return 0;
  case WM_KEYDOWN:
    game = (gameStruct *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    if (game == NULL) {
      return 0;
    }
    switch (wParam) {
    case VK_LEFT:
      if (!left) {
        game_user_input(game, INPUT_LEFT);
        left = true;
        InvalidateRect(hwnd, NULL, false);
      }
      break;
    case VK_RIGHT:
      if (!right) {
        game_user_input(game, INPUT_RIGHT);
        right = true;
        InvalidateRect(hwnd, NULL, false);
      }
      break;
    case VK_UP:
      if (!up) {
        game_user_input(game, INPUT_ROTATE);
        up = true;
        InvalidateRect(hwnd, NULL, false);
      }
      break;
    case VK_DOWN:
      if (!down) {
        game_user_input(game, INPUT_DROP);
        down = true;
        InvalidateRect(hwnd, NULL, false);
      }
      break;
    }
    return 0;
  case WM_KEYUP:
    switch (wParam) {
    case VK_LEFT:
      left = false;
      break;
    case VK_RIGHT:
      right = false;
      break;
    case VK_UP:
      up = false;
      break;
    case VK_DOWN:
      down = false;
      break;
    }
    return 0;
  case WM_DESTROY:
    // DeleteObject(font);
    PostQuitMessage(0);
    break;
  default:
    return DefWindowProc(hwnd, message, wParam, lParam);
  }
  return 0;
}
