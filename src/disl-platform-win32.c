/**
  disl-platform-win32.c

  Written by DoelJavid.
*/

#include "disl-platform-win32.h"
#include <stdio.h>
#include <stdlib.h>

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

static ATOM globalWindowClass = 0;

static DISL_KEYCODE _translateVK(WPARAM wparam, LPARAM lparam) {
  switch (wparam) {
  case VK_ESCAPE: return DISL_KEYCODE_ESCAPE;
  case VK_SNAPSHOT: return DISL_KEYCODE_PRINT_SCREEN;
  case VK_TAB: return DISL_KEYCODE_TAB;
  case VK_SPACE: return DISL_KEYCODE_SPACE;
  case VK_BACK: return DISL_KEYCODE_BACKSPACE;
  case VK_CAPITAL: return DISL_KEYCODE_CAPS_LOCK;
  case VK_SCROLL: return DISL_KEYCODE_SCROLL_LOCK;
  case VK_PAUSE: return DISL_KEYCODE_PAUSE;
  // case : return DISL_KEYCODE_BREAK; // Not available in Win32.
  case VK_NUMLOCK: return DISL_KEYCODE_NUMLOCK;
  case VK_HOME: return DISL_KEYCODE_HOME;
  case VK_END: return DISL_KEYCODE_END;
  case VK_INSERT: return DISL_KEYCODE_INSERT;
  case VK_DELETE: return DISL_KEYCODE_DELETE;
  // case : return DISL_KEYCODE_SYSREQ; // Not available in Win32.
  case VK_SHIFT:
    wparam = MapVirtualKeyA(LOBYTE(HIWORD(lparam)), MAPVK_VSC_TO_VK_EX);
    switch (wparam) {
    case VK_RSHIFT: return DISL_KEYCODE_RIGHT_SHIFT;
    default: return DISL_KEYCODE_LEFT_SHIFT;
    }
  case VK_CONTROL:
    if (lparam & (1 << 24))
      return DISL_KEYCODE_RIGHT_CTRL;
    else
      return DISL_KEYCODE_LEFT_CTRL;
  case VK_MENU:
    if (lparam & (1 << 24))
      return DISL_KEYCODE_RIGHT_ALT;
    else
      return DISL_KEYCODE_LEFT_ALT;
  case VK_RETURN:
    // Return is a special case. We need to handle lparam for this.
    if (lparam & (1 << 24))
      return DISL_KEYCODE_NUMPAD_ENTER;
    else
      return DISL_KEYCODE_ENTER;

  case 0x51: return DISL_KEYCODE_Q;
  case 0x57: return DISL_KEYCODE_W;
  case 0x45: return DISL_KEYCODE_E;
  case 0x52: return DISL_KEYCODE_R;
  case 0x54: return DISL_KEYCODE_T;
  case 0x59: return DISL_KEYCODE_Y;
  case 0x55: return DISL_KEYCODE_U;
  case 0x49: return DISL_KEYCODE_I;
  case 0x4F: return DISL_KEYCODE_O;
  case 0x50: return DISL_KEYCODE_P;
  case 0x41: return DISL_KEYCODE_A;
  case 0x53: return DISL_KEYCODE_S;
  case 0x44: return DISL_KEYCODE_D;
  case 0x46: return DISL_KEYCODE_F;
  case 0x47: return DISL_KEYCODE_G;
  case 0x48: return DISL_KEYCODE_H;
  case 0x4A: return DISL_KEYCODE_J;
  case 0x4B: return DISL_KEYCODE_K;
  case 0x4C: return DISL_KEYCODE_L;
  case 0x5A: return DISL_KEYCODE_Z;
  case 0x58: return DISL_KEYCODE_X;
  case 0x43: return DISL_KEYCODE_C;
  case 0x56: return DISL_KEYCODE_V;
  case 0x42: return DISL_KEYCODE_B;
  case 0x4E: return DISL_KEYCODE_N;
  case 0x4D: return DISL_KEYCODE_M;

  case 0x31: return DISL_KEYCODE_1;
  case 0x32: return DISL_KEYCODE_2;
  case 0x33: return DISL_KEYCODE_3;
  case 0x34: return DISL_KEYCODE_4;
  case 0x35: return DISL_KEYCODE_5;
  case 0x36: return DISL_KEYCODE_6;
  case 0x37: return DISL_KEYCODE_7;
  case 0x38: return DISL_KEYCODE_8;
  case 0x39: return DISL_KEYCODE_9;
  case 0x30: return DISL_KEYCODE_0;

  case VK_OEM_1: return DISL_KEYCODE_SEMICOLON;
  case VK_OEM_COMMA: return DISL_KEYCODE_COMMA;
  case VK_OEM_PERIOD: return DISL_KEYCODE_PERIOD;
  case VK_OEM_7: return DISL_KEYCODE_QUOTE;
  case VK_OEM_2: return DISL_KEYCODE_FORWARDSLASH;
  case VK_OEM_5: return DISL_KEYCODE_BACKSLASH;
  case VK_OEM_MINUS: return DISL_KEYCODE_MINUS;
  case VK_OEM_PLUS: return DISL_KEYCODE_EQUALS;
  case VK_OEM_3: return DISL_KEYCODE_BACKTICK;
  case VK_OEM_4: return DISL_KEYCODE_LEFT_BRACKET;
  case VK_OEM_6: return DISL_KEYCODE_RIGHT_BRACKET;

  case VK_NUMPAD0: return DISL_KEYCODE_NUMPAD_0;
  case VK_NUMPAD1: return DISL_KEYCODE_NUMPAD_1;
  case VK_NUMPAD2: return DISL_KEYCODE_NUMPAD_2;
  case VK_NUMPAD3: return DISL_KEYCODE_NUMPAD_3;
  case VK_NUMPAD4: return DISL_KEYCODE_NUMPAD_4;
  case VK_NUMPAD5: return DISL_KEYCODE_NUMPAD_5;
  case VK_CLEAR: return DISL_KEYCODE_NUMPAD_5; // Why is this numpad 5? I don't know...
  case VK_NUMPAD6: return DISL_KEYCODE_NUMPAD_6;
  case VK_NUMPAD7: return DISL_KEYCODE_NUMPAD_7;
  case VK_NUMPAD8: return DISL_KEYCODE_NUMPAD_8;
  case VK_NUMPAD9: return DISL_KEYCODE_NUMPAD_9;

  case VK_ADD: return DISL_KEYCODE_NUMPAD_PLUS;
  case VK_SUBTRACT: return DISL_KEYCODE_NUMPAD_MINUS;
  case VK_MULTIPLY: return DISL_KEYCODE_NUMPAD_STAR;
  case VK_DIVIDE: return DISL_KEYCODE_NUMPAD_SLASH;
  case VK_SEPARATOR: return DISL_KEYCODE_NUMPAD_DELETE;

  case VK_F1: return DISL_KEYCODE_F1;
  case VK_F2: return DISL_KEYCODE_F2;
  case VK_F3: return DISL_KEYCODE_F3;
  case VK_F4: return DISL_KEYCODE_F4;
  case VK_F5: return DISL_KEYCODE_F5;
  case VK_F6: return DISL_KEYCODE_F6;
  case VK_F7: return DISL_KEYCODE_F7;
  case VK_F8: return DISL_KEYCODE_F8;
  case VK_F9: return DISL_KEYCODE_F9;
  case VK_F10: return DISL_KEYCODE_F10;
  case VK_F11: return DISL_KEYCODE_F11;
  case VK_F12: return DISL_KEYCODE_F12;
  case VK_F13: return DISL_KEYCODE_F13;
  case VK_F14: return DISL_KEYCODE_F14;
  case VK_F15: return DISL_KEYCODE_F15;
  case VK_F16: return DISL_KEYCODE_F16;
  case VK_F17: return DISL_KEYCODE_F17;
  case VK_F18: return DISL_KEYCODE_F18;
  case VK_F19: return DISL_KEYCODE_F19;
  case VK_F20: return DISL_KEYCODE_F20;
  case VK_F21: return DISL_KEYCODE_F21;
  case VK_F22: return DISL_KEYCODE_F22;
  case VK_F23: return DISL_KEYCODE_F23;
  case VK_F24: return DISL_KEYCODE_F24;
  // case : return DISL_KEYCODE_F25; // Not available in Win32.

  case VK_UP: return DISL_KEYCODE_UP;
  case VK_DOWN: return DISL_KEYCODE_DOWN;
  case VK_LEFT: return DISL_KEYCODE_LEFT;
  case VK_RIGHT: return DISL_KEYCODE_RIGHT;
  case VK_PRIOR: return DISL_KEYCODE_PAGE_UP;
  case VK_NEXT: return DISL_KEYCODE_PAGE_DOWN;

  case VK_LWIN: return DISL_KEYCODE_LEFT_SUPER;
  case VK_RWIN: return DISL_KEYCODE_RIGHT_SUPER;
  case VK_APPS: return DISL_KEYCODE_MENU;
  default: return DISL_KEYCODE_NULL;
  }
}

static DWORD _flagsToWin32Style(DISL_FLAGS flags) {
  DWORD style = 0;

  if (flags & DISL_FLAG_SHOWN) {
    style = WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

    if (flags & DISL_FLAG_MINIMIZED) {
      style |= WS_ICONIC;
    } else if (flags & DISL_FLAG_MAXIMIZED) {
      style |= WS_MAXIMIZE;
    }

    if (flags & (DISL_FLAG_BORDERLESS | DISL_FLAG_FULLSCREEN)) {
      style |= WS_POPUP;
    } else {
      style |= WS_OVERLAPPEDWINDOW;
    }
  }

  return style;
}

static LRESULT CALLBACK wndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
  DISLDisplay* display = (DISLDisplay*)GetPropA(hwnd, "DISLDisplay");
  _DISLDisplayWin32* handle = (_DISLDisplayWin32*)display;

  switch (umsg) {
  case WM_CREATE:
    return 0;

  case WM_PAINT:
    if (display->hooks.refresh)
      display->hooks.refresh(display);
    break;

  case WM_SIZE: {
    int newWidth = (int)LOWORD(lparam);
    int newHeight = (int)HIWORD(lparam);

    if (wparam == SIZE_MAXIMIZED && !(handle->internal.flags & DISL_FLAG_MAXIMIZED)) {
      handle->internal.flags |= DISL_FLAG_MAXIMIZED;
      if (display->hooks.maximize)
        display->hooks.maximize(display, true);

    } else if (wparam == SIZE_MINIMIZED && !(handle->internal.flags & DISL_FLAG_MINIMIZED)) {
      handle->internal.flags |= DISL_FLAG_MINIMIZED;
      if (display->hooks.iconify)
        display->hooks.iconify(display, true);

    } else if (wparam == SIZE_RESTORED) {
      if ((handle->internal.flags & DISL_FLAG_MINIMIZED) && display->hooks.iconify)
        display->hooks.iconify(display, false);

      if ((handle->internal.flags & DISL_FLAG_MAXIMIZED) && display->hooks.maximize)
        display->hooks.maximize(display, false);

      handle->internal.flags &= ~(DISL_FLAG_MINIMIZED | DISL_FLAG_MAXIMIZED);
    }

    if (
      newWidth != handle->internal.transform.width ||
      newHeight != handle->internal.transform.height
    ) {
      handle->internal.transform = (DISLTransform){
        .x = handle->internal.transform.x,
        .y = handle->internal.transform.y,
        .width = newWidth,
        .height = newHeight
      };

      if (display->hooks.resize)
        display->hooks.resize(display, handle->internal.transform);
    }
    return 0;
  }

  case WM_MOVE:
    handle->internal.transform = (DISLTransform){
      .x = GET_X_LPARAM(lparam),
      .y = GET_Y_LPARAM(lparam),
      .width = handle->internal.transform.width,
      .height = handle->internal.transform.height
    };
    return 0;

  case WM_SETFOCUS:
  case WM_KILLFOCUS:
    if (display->hooks.focus)
      display->hooks.focus(display, umsg == WM_SETFOCUS);
    break;

  case WM_SYSKEYDOWN:
  case WM_KEYDOWN:
    if (lparam & (1 << 30))
      return 0;
  case WM_SYSKEYUP:
  case WM_KEYUP:
    if (display->hooks.key)
      display->hooks.key(display, _translateVK(wparam, lparam), umsg == WM_KEYDOWN || umsg == WM_SYSKEYDOWN);
    return 0;

  case WM_MOUSEMOVE:
  case WM_MOUSEWHEEL:
  case WM_LBUTTONDOWN:
  case WM_LBUTTONUP:
  case WM_RBUTTONDOWN:
  case WM_RBUTTONUP:
  case WM_MBUTTONDOWN:
  case WM_MBUTTONUP:
    if (display->hooks.mouse) {
      DISL_MOUSE_EVENT eventType;
      switch (umsg) {
      case WM_LBUTTONDOWN:
        eventType = DISL_MOUSE_EVENT_BUTTON1DOWN;
        break;
      case WM_LBUTTONUP:
        eventType = DISL_MOUSE_EVENT_BUTTON1UP;
        break;
      case WM_RBUTTONDOWN:
        eventType = DISL_MOUSE_EVENT_BUTTON2DOWN;
        break;
      case WM_RBUTTONUP:
        eventType = DISL_MOUSE_EVENT_BUTTON2UP;
        break;
      case WM_MBUTTONDOWN:
        eventType = DISL_MOUSE_EVENT_BUTTON3DOWN;
        break;
      case WM_MBUTTONUP:
        eventType = DISL_MOUSE_EVENT_BUTTON3UP;
        break;
      case WM_MOUSEWHEEL:
        eventType = DISL_MOUSE_EVENT_SCROLL;
        break;
      default:
        eventType = DISL_MOUSE_EVENT_MOVE;
        break;
      }
      display->hooks.mouse(
        display,
        eventType,
        GET_X_LPARAM(lparam),
        GET_Y_LPARAM(lparam)
      );
    }
    return 0;

  case WM_CLOSE:
    display->active = false;
    return 0;

  default:
    break;
  }
  return DefWindowProc(hwnd, umsg, wparam, lparam);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

DISLDisplay* _dislOpenDisplayWin32(const char* title, DISLTransform transform, DISL_FLAGS flags, DISLHooks hooks) {
  _DISLDisplayWin32* handle = (_DISLDisplayWin32*)calloc(1, sizeof(_DISLDisplayWin32));

  if (!globalWindowClass) {
    WNDCLASSEXA windowClass = {0};
    windowClass.cbSize = sizeof(WNDCLASSEXA);
    windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    windowClass.hInstance = GetModuleHandle(NULL);
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.lpfnWndProc = wndProc;
    windowClass.lpszClassName = "_DISLDisplayClass_";

    globalWindowClass = RegisterClassExA(&windowClass);
    if (!globalWindowClass) {
      printf("Failed to register class\n");
      free(handle);
      return NULL;
    }
  }

  handle->hwnd = CreateWindowExA(
    WS_EX_APPWINDOW,
    "_DISLDisplayClass_",
    title,
    _flagsToWin32Style(flags),
    transform.x >= 0 ? transform.x : CW_USEDEFAULT,
    transform.y >= 0 ? transform.y : CW_USEDEFAULT,
    transform.width >= 0 ? transform.width : CW_USEDEFAULT,
    transform.height >= 0 ? transform.height : CW_USEDEFAULT,
    NULL,
    NULL,
    GetModuleHandle(NULL),
    NULL
  );
  if (!handle->hwnd) {
    printf("Failed to create window\n");
    free(handle);
    return NULL;
  }

  if (!SetPropA(handle->hwnd, "DISLDisplay", (HANDLE)handle)) {
    printf("Failed to set prop\n");
    free(handle);
    return NULL;
  }

  if (flags & DISL_FLAG_SHOWN) {
    ShowWindow(handle->hwnd, SW_SHOW);
    UpdateWindow(handle->hwnd);
  }

  handle->hdc = GetDC(handle->hwnd);
  handle->internal.pub.active = true;
  handle->internal.pub.hooks = hooks;
  handle->internal.title = title;
  handle->internal.transform = transform;
  handle->internal.flags = flags;
  return (DISLDisplay*)handle;
}

void _dislRetitleDisplayWin32(DISLDisplay* display, const char* title) {
  _DISLDisplayWin32* handle = (_DISLDisplayWin32*)display;
  if (SetWindowTextA(handle->hwnd, title))
    handle->internal.title = title;
}

void _dislTransformDisplayWin32(DISLDisplay* display, DISLTransform transform) {
  _DISLDisplayWin32* handle = (_DISLDisplayWin32*)display;
  DISLTransform internalTransform = handle->internal.transform;
  transform = (DISLTransform){
    .x = transform.x >= 0 ? transform.x : internalTransform.x,
    .y = transform.y >= 0 ? transform.y : internalTransform.y,
    .width = transform.width >= 0 ? transform.width : internalTransform.width,
    .height = transform.height >= 0 ? transform.height : internalTransform.height
  };
  MoveWindow(
    handle->hwnd,
    transform.x,
    transform.y,
    transform.width,
    transform.height,
    true
  );
  handle->internal.transform = transform;
}

void _dislSetDisplayFlagsWin32(DISLDisplay* display, DISL_FLAGS flags) {
  _DISLDisplayWin32* handle = (_DISLDisplayWin32*)display;

  if (flags & DISL_FLAG_SHOWN) {
    if (flags & DISL_FLAG_MINIMIZED) {
      ShowWindow(handle->hwnd, SW_MINIMIZE);
    } else if (flags & (DISL_FLAG_MAXIMIZED | DISL_FLAG_FULLSCREEN)) {
      ShowWindow(handle->hwnd, SW_MAXIMIZE);
    } else {
      if (handle->internal.flags & (DISL_FLAG_MAXIMIZED | DISL_FLAG_MINIMIZED))
        ShowWindow(handle->hwnd, SW_RESTORE);
      else
        ShowWindow(handle->hwnd, SW_SHOWNORMAL);
    }
  } else {
    ShowWindow(handle->hwnd, SW_HIDE);
  }

  SetWindowLongPtrA(handle->hwnd, GWL_STYLE, _flagsToWin32Style(flags));

  if (flags & DISL_FLAG_FULLSCREEN) {
    HMONITOR monitor = MonitorFromWindow(handle->hwnd, MONITOR_DEFAULTTONEAREST);
    MONITORINFO monitorInfo = { sizeof(monitorInfo) };
    GetMonitorInfo(monitor, & monitorInfo);
    // Note: Screen tearing occurs on OpenGL when the size of the display
    // equals the size of the monitor.
    SetWindowPos(
      handle->hwnd,
      HWND_TOP,
      monitorInfo.rcMonitor.left - 1, monitorInfo.rcMonitor.top - 1,
      monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left + 1,
      monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top + 1,
      SWP_NOZORDER | SWP_FRAMECHANGED
    );
  } else {
    SetWindowPos(
      handle->hwnd,
      NULL,
      0, 0,
      0, 0,
      SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED
    );
  }

  handle->internal.flags = flags;
}

void _dislPollEventsWin32(DISLDisplay* display) {
  _DISLDisplayWin32* handle = (_DISLDisplayWin32*)display;
  MSG msg = {0};
  while (PeekMessageA(&msg, handle->hwnd, 0, 0, PM_REMOVE)) {
    TranslateMessage(&msg);
    DispatchMessageA(&msg);
  }
}

void _dislCloseDisplayWin32(DISLDisplay* display) {
  _DISLDisplayWin32* handle = (_DISLDisplayWin32*)display;
  ReleaseDC(handle->hwnd, handle->hdc);
  RemovePropA(handle->hwnd, "DISLDisplay");
  DestroyWindow(handle->hwnd);
  free(handle);
}

