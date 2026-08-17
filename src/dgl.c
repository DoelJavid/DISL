/**
  dgl.c

  Written by DoelJavid.
*/

#include <dgl.h>
#include <disl.h>

const DGLPlatform* _dgl = NULL;

#ifdef DISL_USE_XORG
#include "dgl-platform-xorg.h"
#endif

DGLContext* dglCreateContext(DISLDisplay* display, DGLConfig* conf) {
  if (_dgl) {
    return _dgl->createContext(display, conf);
  }

  switch (dislGetDisplayBackend()) {
  #ifdef DISL_USE_WIN32
  case DISL_BACKEND_WIN32:
    return NULL; // Not implemented yet...
  #endif

  #ifdef DISL_USE_XORG
  case DISL_BACKEND_XORG:
    _dgl = &_xorgGL;
    return _dglCreateContextXorg(display, conf);
  #endif

  #ifdef DISL_USE_DRM
  case DISL_BACKEND_DRM:
    return NULL; // Not implemented yet...
  #endif

  default:
    return NULL;
  }
}

