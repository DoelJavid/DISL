/**
  dgl-platform-xorg.h

  Written by DoelJavid.

  The DGL implementation for X.Org.
*/

#ifndef DGL_PLATFORM_XORG_H
#define DGL_PLATFORM_XORG_H

#include <dgl.h>

DGLContext* _dglCreateContextXorg(DISLDisplay* display, DGLConfig* conf);
DGLProc _dglGetProcAddressXorg(const char* processName);
bool _dglMakeCurrentXorg(DISLDisplay* display, DGLContext* context);
void _dglSwapBuffersXorg(DISLDisplay* display);
void _dglDeleteContextXorg(DGLContext* context);

static const DGLPlatform _xorgGL = (DGLPlatform){
  .createContext = _dglCreateContextXorg,
  .makeCurrent = _dglMakeCurrentXorg,
  .getProcAddress = _dglGetProcAddressXorg,
  .swapBuffers = _dglSwapBuffersXorg,
  .deleteContext = _dglDeleteContextXorg
};

#endif
