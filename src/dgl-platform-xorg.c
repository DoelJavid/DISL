/**
  dgl-platform-xorg.c

  Written by DoelJavid.

  The DGL implementation for X.Org.
*/

#include "dgl-platform-xorg.h"
#include "disl-platform-xorg.h"
#include <stdlib.h>
#include <GL/glx.h>
#include <GL/glxext.h>

#define GLX_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB 0x2092

typedef struct {
  Display* xdisplay;
  GLXContext context;
} DGLXContext;

DGLContext* _dglCreateContextXorg(DISLDisplay* display, DGLConfig* conf) {
  _DISLDisplayXorg* handle = (_DISLDisplayXorg*)display;
  int visualAttribs[] = {
    GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
    GLX_RENDER_TYPE, GLX_RGBA_BIT,
    GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
    GLX_RED_SIZE, conf->redBits,
    GLX_GREEN_SIZE, conf->greenBits,
    GLX_BLUE_SIZE, conf->blueBits,
    GLX_ALPHA_SIZE, conf->alphaBits,
    GLX_DEPTH_SIZE, conf->depthBits,
    GLX_STENCIL_SIZE, conf->stencilBits,
    GLX_DOUBLEBUFFER, True,
    None
  };

  // GLX must be version 1.3 or higher to proceed.
  int glxMajor, glxMinor;
  if (
    !glXQueryVersion(handle->display, &glxMajor, &glxMinor) ||
    (glxMajor < 1 || (glxMajor == 1 && glxMinor < 3))
  )
    return NULL;

  int confCount;
  GLXFBConfig* fbConfs = glXChooseFBConfig(
    handle->display,
    DefaultScreen(handle->display),
    visualAttribs,
    &confCount
  );
  if (!fbConfs || confCount < 1)
    return NULL;

  int bestFbIdx = -1;
  int bestSampleCount = -1;
  for (int i = 0; i < confCount; i++) {
    XVisualInfo* visualInfo = glXGetVisualFromFBConfig(
      handle->display,
      fbConfs[i]
    );
    if (visualInfo) {
      int sampleBuffer, samples;
      glXGetFBConfigAttrib(
        handle->display, fbConfs[i], GLX_SAMPLE_BUFFERS, &sampleBuffer);
      glXGetFBConfigAttrib(
        handle->display, fbConfs[i], GLX_SAMPLES, &samples);

      if (bestFbIdx < 0 || (sampleBuffer && samples > bestSampleCount)) {
        bestFbIdx = i;
        bestSampleCount = samples;
      }
    }
  }
  GLXFBConfig bestFbConf = fbConfs[bestFbIdx];
  XFree(fbConfs);

  int contextAttribs[] = {
    GLX_CONTEXT_PROFILE_MASK_ARB, conf->compatibility ?
      GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB :
      GLX_CONTEXT_CORE_PROFILE_BIT_ARB,

    GLX_CONTEXT_MAJOR_VERSION_ARB, conf->glMajor,
    GLX_CONTEXT_MINOR_VERSION_ARB, conf->glMinor,
    None
  };


  static PFNGLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribsARB = NULL;
  if (!glXCreateContextAttribsARB) {
    glXCreateContextAttribsARB = (PFNGLXCREATECONTEXTATTRIBSARBPROC)
      glXGetProcAddressARB(
        (const GLubyte*)"glXCreateContextAttribsARB"
      );
    if (!glXCreateContextAttribsARB)
      return NULL;
  }

  DGLXContext* context = (DGLXContext*)malloc(sizeof(DGLXContext));
  context->xdisplay = handle->display;
  context->context = glXCreateContextAttribsARB(
    handle->display,
    bestFbConf,
    NULL,
    True,
    contextAttribs
  );
  if (!context->context) {
    free(context);
    return NULL;
  }

  return (DGLContext*)context;
}

DGLProc _dglGetProcAddressXorg(const char* processName) {
  return glXGetProcAddress((GLubyte*)processName);
}

bool _dglMakeCurrentXorg(DISLDisplay* display, DGLContext* context) {
  _DISLDisplayXorg* handle = (_DISLDisplayXorg*)display;
  DGLXContext* xcontext = (DGLXContext*)context;
  return glXMakeCurrent(handle->display, handle->window, xcontext->context);
}

void _dglSwapBuffersXorg(DISLDisplay* display) {
  _DISLDisplayXorg* handle = (_DISLDisplayXorg*)display;
  glXSwapBuffers(handle->display, handle->window);
}

void _dglDeleteContextXorg(DGLContext* context) {
  DGLXContext* xcontext = (DGLXContext*)context;
  glXDestroyContext(xcontext->xdisplay, xcontext->context);
  free(xcontext);
}

