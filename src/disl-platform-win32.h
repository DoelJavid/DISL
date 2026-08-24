/**
  disl-platform-win32.h

  Written by DoelJavid.

  The DISL implementation for Win32.
*/

// TODO: Add Win32 support.

#ifndef DISL_PLATFORM_WIN32_H
#define DISL_PLATFORM_WIN32_H

#include <disl.h>
#include <Windows.h>
#include <Windowsx.h>
#include "disl-internal.h"

typedef struct {
  _DISLDisplayI internal;
  HWND hwnd;
  HDC hdc;
} _DISLDisplayWin32;

DISLDisplay* _dislOpenDisplayWin32(const char* title, int x, int y, int width, int height);
void _dislRetitleDisplayWin32(DISLDisplay* display, const char* title);
void _dislTransformDisplayWin32(DISLDisplay* display, int x, int y, int width, int height);
void _dislSetDisplayFlagsWin32(DISLDisplay* display, DISL_FLAGS flags);
void _dislPollEventsWin32(DISLDisplay* display);
void _dislCloseDisplayWin32(DISLDisplay* display);

// I hate you Windows...
static DISLPlatform _win32Platform = {
  .openDisplay = _dislOpenDisplayWin32,
  .retitleDisplay = _dislRetitleDisplayWin32,
  .transformDisplay = _dislTransformDisplayWin32,
  .setDisplayFlags = _dislSetDisplayFlagsWin32,
  .pollEvents = _dislPollEventsWin32,
  .closeDisplay = _dislCloseDisplayWin32
};

#endif
