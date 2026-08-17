/**
  DISL OpenGL (DGL)

  Written by DoelJavid.
*/

#ifndef DGL_H
#define DGL_H

#include <disl.h>

typedef struct DGLContext DGLContext;
typedef void (*DGLProc)(void);

typedef struct {
  int glMajor;
  int glMinor;
  bool compatibility;

  int redBits;
  int greenBits;
  int blueBits;
  int alphaBits;
  int depthBits;
  int stencilBits;
} DGLConfig;

typedef struct {
  DGLContext* (*createContext)(DISLDisplay* display, DGLConfig* conf);
  bool (*makeCurrent)(DISLDisplay* display, DGLContext* context);
  DGLProc (*getProcAddress)(const char* processName);
  void (*swapBuffers)(DISLDisplay* display);
  void (*deleteContext)(DGLContext* context);
} DGLPlatform;

extern const DGLPlatform* _dgl;

/**
  Creates an OpenGL context for the given display using the given
  configuration. You must make the context current in order to use it.
*/
DGLContext* dglCreateContext(DISLDisplay* display, DGLConfig* conf);

/**
  Makes the given OpenGL context current for the given display, allowing you to
  use it.
*/
#define dglMakeCurrent(display, context) _dgl->makeCurrent(display, context)

/**
  Swaps the display's buffers, displaying anything drawn by OpenGL.
*/
#define dglSwapBuffers(display) _dgl->swapBuffers(display)

/**
  Deletes the given OpenGL context.
*/
#define dglDeleteContext(context) _dgl->deleteContext(context)

#endif
