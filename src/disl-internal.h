/**
  DISL Internals

  Written by DoelJavid.

  Contains the private display definition used by DISL.
*/

#ifndef DISL_INTERNAL_H
#define DISL_INTERNAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <disl.h>

typedef struct _DISLDisplayI {
  DISLDisplay pub;
  const char* title;
  DISLTransform transform;
  DISL_FLAGS flags;
} _DISLDisplayI;

#ifdef __cplusplus
}
#endif

#endif
