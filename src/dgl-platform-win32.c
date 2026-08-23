/**
  dgl-platform-win32.c

  Written by DoelJavid.
*/

#include "dgl-platform-win32.h"
#include "disl-platform-win32.h"
#include <stdlib.h>
#include <assert.h>
#include <windows.h>

// Copied from khrnonos registry:
// https://registry.khronos.org/OpenGL/extensions/EXT/WGL_EXT_pixel_format.txt

#define WGL_NUMBER_PIXEL_FORMATS_EXT 0x2000
#define WGL_DRAW_TO_WINDOW_EXT       0x2001
#define WGL_DRAW_TO_BITMAP_EXT       0x2002
#define WGL_ACCELERATION_EXT         0x2003
#define WGL_NEED_PALETTE_EXT         0x2004
#define WGL_NEED_SYSTEM_PALETTE_EXT  0x2005
#define WGL_SWAP_LAYER_BUFFERS_EXT   0x2006
#define WGL_SWAP_METHOD_EXT          0x2007
#define WGL_NUMBER_OVERLAYS_EXT      0x2008
#define WGL_NUMBER_UNDERLAYS_EXT     0x2009
#define WGL_TRANSPARENT_EXT          0x200A
#define WGL_TRANSPARENT_VALUE_EXT    0x200B
#define WGL_SHARE_DEPTH_EXT          0x200C
#define WGL_SHARE_STENCIL_EXT        0x200D
#define WGL_SHARE_ACCUM_EXT          0x200E
#define WGL_SUPPORT_GDI_EXT          0x200F
#define WGL_SUPPORT_OPENGL_EXT       0x2010
#define WGL_DOUBLE_BUFFER_EXT        0x2011
#define WGL_STEREO_EXT               0x2012
#define WGL_PIXEL_TYPE_EXT           0x2013
#define WGL_COLOR_BITS_EXT           0x2014
#define WGL_RED_BITS_EXT             0x2015
#define WGL_RED_SHIFT_EXT            0x2016
#define WGL_GREEN_BITS_EXT           0x2017
#define WGL_GREEN_SHIFT_EXT          0x2018
#define WGL_BLUE_BITS_EXT            0x2019
#define WGL_BLUE_SHIFT_EXT           0x201A
#define WGL_ALPHA_BITS_EXT           0x201B
#define WGL_ALPHA_SHIFT_EXT          0x201C
#define WGL_ACCUM_BITS_EXT           0x201D
#define WGL_ACCUM_RED_BITS_EXT       0x201E
#define WGL_ACCUM_GREEN_BITS_EXT     0x201F
#define WGL_ACCUM_BLUE_BITS_EXT      0x2020
#define WGL_ACCUM_ALPHA_BITS_EXT     0x2021
#define WGL_DEPTH_BITS_EXT           0x2022
#define WGL_STENCIL_BITS_EXT         0x2023
#define WGL_AUX_BUFFERS_EXT          0x2024
#define WGL_NO_ACCELERATION_EXT      0x2025
#define WGL_GENERIC_ACCELERATION_EXT 0x2026
#define WGL_FULL_ACCELERATION_EXT    0x2027
#define WGL_SWAP_EXCHANGE_EXT        0x2028
#define WGL_SWAP_COPY_EXT            0x2029
#define WGL_SWAP_UNDEFINED_EXT       0x202A
#define WGL_TYPE_RGBA_EXT            0x202B
#define WGL_TYPE_COLORINDEX_EXT      0x202C

#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB          0x00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002
#define WGL_CONTEXT_MAJOR_VERSION_ARB             0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB             0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB              0x9126

///////////////////////////////////////////////////////////////////////////////

typedef struct {
  HGLRC hglrc;
} _DGLWContext;

typedef HGLRC (WINAPI* PFNwglCreateContextAttribsARB)(HDC hDC, HGLRC hshareContext, const int *attribList);
static PFNwglCreateContextAttribsARB wglCreateContextAttribsARB = NULL;

typedef HRESULT (APIENTRY* PFNwglChoosePixelFormatARB)(
  HDC hdc,
  const int* piAttribIList,
  const FLOAT* pfAttribFList,
  UINT nMaxFormats,
  int* piFormats,
  UINT* nNumFormats
);
static PFNwglChoosePixelFormatARB wglChoosePixelFormatARB = NULL;

DGLContext* _dglCreateContextWin32(DISLDisplay* display, DGLConfig* conf) {
  assert(display != NULL);
  assert(conf != NULL);

  _DISLDisplayWin32* handle = (_DISLDisplayWin32*)display;

  if (!wglCreateContextAttribsARB) {
    WNDCLASSA dummyClass = {0};
    dummyClass.hInstance = GetModuleHandle(NULL);
    dummyClass.lpfnWndProc = DefWindowProc;
    dummyClass.lpszClassName = "_DISLDummy_";
    if (!RegisterClassA(&dummyClass))
      return NULL;

    HWND dummyWindow = CreateWindowA("_DISLDummy_", "_DISLDummy_", 0, 200, 200, 300, 300, 0, 0, dummyClass.hInstance, 0);
    if (!dummyWindow)
      return NULL;

    HDC dummyDc = GetDC(dummyWindow);
    if (!dummyDc) {
      DestroyWindow(dummyWindow);
      return NULL;
    }

    PIXELFORMATDESCRIPTOR pfd = {
      sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
			PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
			32,                   // Colordepth of the framebuffer.
			0, 0, 0, 0, 0, 0,
			0,
			0,
			0,
			0, 0, 0, 0,
			24,                   // Number of bits for the depthbuffer
			8,                    // Number of bits for the stencilbuffer
			0,                    // Number of Aux buffers in the framebuffer.
			PFD_MAIN_PLANE,
			0, 0, 0, 0
    };

    if (!SetPixelFormat(dummyDc, ChoosePixelFormat(dummyDc, &pfd), &pfd)) {
      ReleaseDC(dummyWindow, dummyDc);
      DestroyWindow(dummyWindow);
      return NULL;
    }

    HGLRC dummyContext = wglCreateContext(dummyDc);
    if (!wglMakeCurrent(dummyDc, dummyContext)) {
      wglDeleteContext(dummyContext);
      ReleaseDC(dummyWindow, dummyDc);
      DestroyWindow(dummyWindow);
      return NULL;
    }

    wglCreateContextAttribsARB = (PFNwglCreateContextAttribsARB)(void*)_dglGetProcAddressWin32("wglCreateContextAttribsARB");
    wglChoosePixelFormatARB = (PFNwglChoosePixelFormatARB)(void*)_dglGetProcAddressWin32("wglChoosePixelFormatARB");

    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(dummyContext);
    ReleaseDC(dummyWindow, dummyDc);
    DestroyWindow(dummyWindow);

    if (!wglCreateContextAttribsARB || !wglChoosePixelFormatARB)
      return NULL;
  }

  const int totalColorBits = conf->redBits + conf->blueBits + conf->greenBits + conf->alphaBits;
  const int pixelAttribs[] = {
    WGL_ACCELERATION_EXT,
    WGL_FULL_ACCELERATION_EXT,
    WGL_SUPPORT_OPENGL_EXT, 1,
    WGL_DRAW_TO_WINDOW_EXT, 1,
    WGL_PIXEL_TYPE_EXT, WGL_TYPE_RGBA_EXT,
    WGL_DOUBLE_BUFFER_EXT, 1,

    WGL_RED_BITS_EXT, conf->redBits,
    WGL_GREEN_BITS_EXT, conf->greenBits,
    WGL_BLUE_BITS_EXT, conf->blueBits,
    WGL_ALPHA_BITS_EXT, conf->alphaBits,
    WGL_DEPTH_BITS_EXT, conf->depthBits,
    WGL_STENCIL_BITS_EXT, conf->stencilBits,
    WGL_COLOR_BITS_EXT,
    totalColorBits,

    0
  };

  PIXELFORMATDESCRIPTOR pfd = {
    sizeof(PIXELFORMATDESCRIPTOR),
    1,
    PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
    PFD_TYPE_RGBA,
    32,
    8,
    PFD_MAIN_PLANE,
    24,
    8,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0
  };
  int pixelFormat;
  UINT totalFormats;

  wglChoosePixelFormatARB(handle->hdc, pixelAttribs, 0, 1, &pixelFormat, &totalFormats);
  if (!totalFormats) {
    return NULL;
  }

  DescribePixelFormat(handle->hdc, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
  if (!SetPixelFormat(handle->hdc, pixelFormat, &pfd))
    return NULL;

  _DGLWContext* context = (_DGLWContext*)calloc(1, sizeof(_DGLWContext));
  const int contextAttribs[] = {
    WGL_CONTEXT_PROFILE_MASK_ARB,
    conf->compatibility
      ? WGL_CONTEXT_CORE_PROFILE_BIT_ARB
      : WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
    WGL_CONTEXT_MAJOR_VERSION_ARB, conf->glMajor,
    WGL_CONTEXT_MINOR_VERSION_ARB, conf->glMinor,
    0, 0
  };
  context->hglrc = wglCreateContextAttribsARB(handle->hdc, NULL, contextAttribs);
  if (!context->hglrc) {
    free(context);
    return NULL;
  }

  return (DGLContext*)context;
}

DGLProc _dglGetProcAddressWin32(const char* processName) {
  void* p = (void*)wglGetProcAddress(processName);
  if (
    p == 0 ||
    (p == (void*)0x1) ||
    (p == (void*)0x2) ||
    (p == (void*)0x3) ||
    (p == (void*)-1)
  ) {
    return NULL;
  }
  return (DGLProc)p;
}

bool _dglMakeCurrentWin32(DISLDisplay* display, DGLContext* context) {
  _DISLDisplayWin32* handle = (_DISLDisplayWin32*)display;
  _DGLWContext* wcontext = (_DGLWContext*)context;
  return wglMakeCurrent(
    handle ? handle->hdc : NULL,
    wcontext ? wcontext->hglrc : NULL
  );
}

void _dglSwapBuffersWin32(DISLDisplay* display) {
  assert(display != NULL);

  _DISLDisplayWin32* handle = (_DISLDisplayWin32*)display;
  SwapBuffers(handle->hdc);
}

void _dglDeleteContextWin32(DGLContext* context) {
  assert(context != NULL);

  _DGLWContext* wcontext = (_DGLWContext*)context;
  wglDeleteContext(wcontext->hglrc);
  free(wcontext);
}

