/**
  dgl-platform-win32.h

  Written by DoelJavid.

  The DGL implementation for Win32.
*/

// TODO: Add Win32 support.

#ifndef DGL_PLATFORM_WIN32_H
#define DGL_PLATFORM_WIN32_H

#include <dgl.h>
#include <Windows.h>

DGLContext* _dglCreateContextWin32(DISLDisplay* display, DGLConfig* conf);
DGLProc _dglGetProcAddressWin32(const char* processName);
bool _dglMakeCurrentWin32(DISLDisplay* display, DGLContext* context);
void _dglSwapBuffersWin32(DISLDisplay* display);
void _dglDeleteContextWin32(DGLContext* context);

static DGLPlatform _win32GL = {
  .createContext = _dglCreateContextWin32,
  .makeCurrent = _dglMakeCurrentWin32,
  .getProcAddress = _dglGetProcAddressWin32,
  .swapBuffers = _dglSwapBuffersWin32,
  .deleteContext = _dglDeleteContextWin32
};

#endif
