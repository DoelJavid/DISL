/**
  disl.c

  Written by DoelJavid.
*/

#include <disl.h>
#include "disl-internal.h"

#ifdef DISL_USE_XORG
#include "disl-platform-xorg.h"
#endif

#ifdef DISL_USE_WIN32
#include "disl-platform-win32.h"
#endif

static DISL_BACKEND _dislBackend = DISL_BACKEND_NONE;
const DISLPlatform* _disl = NULL;

DISL_BACKEND dislGetDisplayBackend() {
  return _dislBackend;
}

DISLDisplay* dislOpenDisplay(const char* title, DISLTransform transform, DISL_FLAGS flags, DISLHooks hooks) {
  if (_disl)
    return _disl->openDisplay(title, transform, flags, hooks);

  DISLDisplay* display = NULL;

  #ifdef DISL_USE_WIN32
  // Not implemented yet...
  #endif

  #ifdef DISL_USE_XORG
  display = _dislOpenDisplayXorg(title, transform, flags, hooks);
  if (display) {
    _dislBackend = DISL_BACKEND_XORG;
    _disl = &_xorgPlatform;
    return display;
  }
  #endif

  return display;
}

const char* dislGetDisplayTitle(DISLDisplay* display) {
  return ((_DISLDisplayI*)display)->title;
}

DISLTransform dislGetDisplayTransform(DISLDisplay* display) {
  return ((_DISLDisplayI*)display)->transform;
}

DISL_FLAGS dislGetDisplayFlags(DISLDisplay* display) {
  return ((_DISLDisplayI*)display)->flags;
}

