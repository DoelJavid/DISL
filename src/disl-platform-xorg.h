/**
  disl-platform-xorg.h

  Written by DoelJavid.

  The DISL implementation for X.Org.
*/

#ifndef DISL_PLATFORM_XORG_H
#define DISL_PLATFORM_XORG_H

#include <disl.h>
#include <X11/Xlib.h>
#include "disl-internal.h"

typedef struct {
  _DISLDisplayI shared;
  Display* display;
  Window root;
  Window window;
} _DISLDisplayXorg;

DISLDisplay* _dislOpenDisplayXorg(const char* title, DISLTransform transform, DISL_FLAGS flags, DISLHooks procs);
void _dislRetitleDisplayXorg(DISLDisplay* display, const char* title);
void _dislTransformDisplayXorg(DISLDisplay* display, DISLTransform transform);
void _dislSetDisplayFlagsXorg(DISLDisplay* display, DISL_FLAGS flags);
void _dislPollEventsXorg(DISLDisplay* display);
void _dislCloseDisplayXorg(DISLDisplay* display);

static const DISLPlatform _xorgPlatform = (DISLPlatform){
  .openDisplay = _dislOpenDisplayXorg,
  .retitleDisplay = _dislRetitleDisplayXorg,
  .transformDisplay = _dislTransformDisplayXorg,
  .setDisplayFlags = _dislSetDisplayFlagsXorg,
  .pollEvents = _dislPollEventsXorg,
  .closeDisplay = _dislCloseDisplayXorg
};

#endif
