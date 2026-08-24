/**
  disl-platform-xorg.c

  Written by DoelJavid.
*/

#include "disl-platform-xorg.h"
#include "disl-internal.h"
#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <X11/keysym.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>

///////////////////////////////////////////////////////////////////////////////
// Private Utilities
///////////////////////////////////////////////////////////////////////////////

#define _NET_WM_STATE_REMOVE        0
#define _NET_WM_STATE_ADD           1
#define _NET_WM_STATE_TOGGLE        2

#define MWM_HINTS_DECORATIONS 2
#define MWM_DECOR_ALL 1

static Atom WM_DELETE_WINDOW;
static Atom WM_STATE;
static Atom NET_WM_STATE;
static Atom NET_WM_STATE_MAXIMIZED_HORZ;
static Atom NET_WM_STATE_MAXIMIZED_VERT;
static Atom NET_WM_STATE_FULLSCREEN;
static Atom MOTIF_WM_HINTS;

static DISL_KEYCODE _translateXorgInput(unsigned int scancode) {
  switch (scancode) {
  case XK_q: return DISL_KEYCODE_Q;
  case XK_w: return DISL_KEYCODE_W;
  case XK_e: return DISL_KEYCODE_E;
  case XK_r: return DISL_KEYCODE_R;
  case XK_t: return DISL_KEYCODE_T;
  case XK_y: return DISL_KEYCODE_Y;
  case XK_u: return DISL_KEYCODE_U;
  case XK_i: return DISL_KEYCODE_I;
  case XK_o: return DISL_KEYCODE_O;
  case XK_p: return DISL_KEYCODE_P;
  case XK_a: return DISL_KEYCODE_A;
  case XK_s: return DISL_KEYCODE_S;
  case XK_d: return DISL_KEYCODE_D;
  case XK_f: return DISL_KEYCODE_F;
  case XK_g: return DISL_KEYCODE_G;
  case XK_h: return DISL_KEYCODE_H;
  case XK_j: return DISL_KEYCODE_J;
  case XK_k: return DISL_KEYCODE_K;
  case XK_l: return DISL_KEYCODE_L;
  case XK_z: return DISL_KEYCODE_Z;
  case XK_x: return DISL_KEYCODE_X;
  case XK_c: return DISL_KEYCODE_C;
  case XK_v: return DISL_KEYCODE_V;
  case XK_b: return DISL_KEYCODE_B;
  case XK_n: return DISL_KEYCODE_N;
  case XK_m: return DISL_KEYCODE_M;

  case XK_Q: return DISL_KEYCODE_Q;
  case XK_W: return DISL_KEYCODE_W;
  case XK_E: return DISL_KEYCODE_E;
  case XK_R: return DISL_KEYCODE_R;
  case XK_T: return DISL_KEYCODE_T;
  case XK_Y: return DISL_KEYCODE_Y;
  case XK_U: return DISL_KEYCODE_U;
  case XK_I: return DISL_KEYCODE_I;
  case XK_O: return DISL_KEYCODE_O;
  case XK_P: return DISL_KEYCODE_P;
  case XK_A: return DISL_KEYCODE_A;
  case XK_S: return DISL_KEYCODE_S;
  case XK_D: return DISL_KEYCODE_D;
  case XK_F: return DISL_KEYCODE_F;
  case XK_G: return DISL_KEYCODE_G;
  case XK_H: return DISL_KEYCODE_H;
  case XK_J: return DISL_KEYCODE_J;
  case XK_K: return DISL_KEYCODE_K;
  case XK_L: return DISL_KEYCODE_L;
  case XK_Z: return DISL_KEYCODE_Z;
  case XK_X: return DISL_KEYCODE_X;
  case XK_C: return DISL_KEYCODE_C;
  case XK_V: return DISL_KEYCODE_V;
  case XK_B: return DISL_KEYCODE_B;
  case XK_N: return DISL_KEYCODE_N;
  case XK_M: return DISL_KEYCODE_M;

  case XK_0: return DISL_KEYCODE_0;
  case XK_1: return DISL_KEYCODE_1;
  case XK_2: return DISL_KEYCODE_2;
  case XK_3: return DISL_KEYCODE_3;
  case XK_4: return DISL_KEYCODE_4;
  case XK_5: return DISL_KEYCODE_5;
  case XK_6: return DISL_KEYCODE_6;
  case XK_7: return DISL_KEYCODE_7;
  case XK_8: return DISL_KEYCODE_8;
  case XK_9: return DISL_KEYCODE_9;

  case XK_semicolon: return DISL_KEYCODE_SEMICOLON;
  case XK_comma: return DISL_KEYCODE_COMMA;
  case XK_period: return DISL_KEYCODE_PERIOD;
  case XK_apostrophe: return DISL_KEYCODE_QUOTE;
  case XK_grave: return DISL_KEYCODE_BACKTICK;
  case XK_backslash: return DISL_KEYCODE_BACKSLASH;
  case XK_minus: return DISL_KEYCODE_MINUS;
  case XK_equal: return DISL_KEYCODE_EQUALS;
  case XK_bracketleft: return DISL_KEYCODE_LEFT_BRACKET;
  case XK_bracketright: return DISL_KEYCODE_RIGHT_BRACKET;

  case XK_KP_0: return DISL_KEYCODE_NUMPAD_0;
  case XK_KP_Insert: return DISL_KEYCODE_NUMPAD_0;
  case XK_KP_1: return DISL_KEYCODE_NUMPAD_1;
  case XK_KP_End: return DISL_KEYCODE_NUMPAD_1;
  case XK_KP_2: return DISL_KEYCODE_NUMPAD_2;
  case XK_KP_Down: return DISL_KEYCODE_NUMPAD_2;
  case XK_KP_3: return DISL_KEYCODE_NUMPAD_3;
  case XK_KP_Page_Down: return DISL_KEYCODE_NUMPAD_3;
  case XK_KP_4: return DISL_KEYCODE_NUMPAD_4;
  case XK_KP_Left: return DISL_KEYCODE_NUMPAD_4;
  case XK_KP_5: return DISL_KEYCODE_NUMPAD_5;
  case XK_KP_Begin: return DISL_KEYCODE_NUMPAD_5;
  case XK_KP_6: return DISL_KEYCODE_NUMPAD_6;
  case XK_KP_Right: return DISL_KEYCODE_NUMPAD_6;
  case XK_KP_7: return DISL_KEYCODE_NUMPAD_7;
  case XK_KP_Home: return DISL_KEYCODE_NUMPAD_7;
  case XK_KP_8: return DISL_KEYCODE_NUMPAD_8;
  case XK_KP_Up: return DISL_KEYCODE_NUMPAD_8;
  case XK_KP_9: return DISL_KEYCODE_NUMPAD_9;
  case XK_KP_Page_Up: return DISL_KEYCODE_NUMPAD_9;
  case XK_KP_Add: return DISL_KEYCODE_NUMPAD_PLUS;
  case XK_KP_Subtract: return DISL_KEYCODE_NUMPAD_MINUS;
  case XK_KP_Multiply: return DISL_KEYCODE_NUMPAD_STAR;
  case XK_KP_Divide: return DISL_KEYCODE_NUMPAD_SLASH;
  case XK_KP_Enter: return DISL_KEYCODE_NUMPAD_ENTER;
  case XK_KP_Separator: return DISL_KEYCODE_NUMPAD_DELETE;
  case XK_KP_Delete: return DISL_KEYCODE_NUMPAD_DELETE;
  case XK_Num_Lock: return DISL_KEYCODE_NUMLOCK;

  case XK_space: return DISL_KEYCODE_SPACE;
  case XK_Return: return DISL_KEYCODE_ENTER;
  case XK_Tab: return DISL_KEYCODE_TAB;
  case XK_Escape: return DISL_KEYCODE_ESCAPE;
  case XK_BackSpace: return DISL_KEYCODE_BACKSPACE;
  case XK_Delete: return DISL_KEYCODE_DELETE;
  case XK_Insert: return DISL_KEYCODE_INSERT;
  case XK_Home: return DISL_KEYCODE_HOME;
  case XK_End: return DISL_KEYCODE_END;
  case XK_Pause: return DISL_KEYCODE_PAUSE;
  case XK_Break: return DISL_KEYCODE_BREAK;
  case XK_Caps_Lock: return DISL_KEYCODE_CAPS_LOCK;
  case XK_Page_Up: return DISL_KEYCODE_PAGE_UP;
  case XK_Page_Down: return DISL_KEYCODE_PAGE_DOWN;
  case XK_Sys_Req: return DISL_KEYCODE_SYSREQ;

  case XK_Shift_R: return DISL_KEYCODE_RIGHT_SHIFT;
  case XK_Shift_L: return DISL_KEYCODE_LEFT_SHIFT;
  case XK_Control_R: return DISL_KEYCODE_RIGHT_CTRL;
  case XK_Control_L: return DISL_KEYCODE_LEFT_CTRL;
  case XK_Alt_R: return DISL_KEYCODE_RIGHT_ALT;
  case XK_Alt_L: return DISL_KEYCODE_LEFT_ALT;
  case XK_Super_R: return DISL_KEYCODE_RIGHT_SUPER;
  case XK_Super_L: return DISL_KEYCODE_LEFT_SUPER;

  case XK_Up: return DISL_KEYCODE_UP;
  case XK_Down: return DISL_KEYCODE_DOWN;
  case XK_Left: return DISL_KEYCODE_LEFT;
  case XK_Right: return DISL_KEYCODE_RIGHT;

  case XK_F1: return DISL_KEYCODE_F1;
  case XK_F2: return DISL_KEYCODE_F2;
  case XK_F3: return DISL_KEYCODE_F3;
  case XK_F4: return DISL_KEYCODE_F4;
  case XK_F5: return DISL_KEYCODE_F5;
  case XK_F6: return DISL_KEYCODE_F6;
  case XK_F7: return DISL_KEYCODE_F7;
  case XK_F8: return DISL_KEYCODE_F8;
  case XK_F9: return DISL_KEYCODE_F9;
  case XK_F10: return DISL_KEYCODE_F10;
  case XK_F11: return DISL_KEYCODE_F11;
  case XK_F12: return DISL_KEYCODE_F12;
  case XK_F13: return DISL_KEYCODE_F13;
  case XK_F14: return DISL_KEYCODE_F14;
  case XK_F15: return DISL_KEYCODE_F15;
  case XK_F16: return DISL_KEYCODE_F16;
  case XK_F17: return DISL_KEYCODE_F17;
  case XK_F18: return DISL_KEYCODE_F18;
  case XK_F19: return DISL_KEYCODE_F19;
  case XK_F20: return DISL_KEYCODE_F20;
  case XK_F21: return DISL_KEYCODE_F21;
  case XK_F22: return DISL_KEYCODE_F22;
  case XK_F23: return DISL_KEYCODE_F23;
  case XK_F24: return DISL_KEYCODE_F24;
  case XK_F25: return DISL_KEYCODE_F25;
  default: return DISL_KEYCODE_NULL;
  }
}

static bool _isXWindowMinimized(_DISLDisplayXorg* handle) {
  Atom actualType;
  int actualFormat;
  unsigned long numItems, bytesAfter;
  unsigned char* prop = NULL;
  bool minimized = false;

  XGetWindowProperty(
    handle->display, handle->window, WM_STATE,
    0, LONG_MAX, False,
    WM_STATE, &actualType, &actualFormat,
    &numItems, &bytesAfter, &prop
  );

  if (prop != NULL) {
    minimized = (*(unsigned long*)prop) == IconicState;
    XFree(prop);
  }
  return minimized;
}

static bool _isXWindowMaximized(_DISLDisplayXorg* handle) {
  Atom actualType;
  int actualFormat;
  unsigned long numItems, bytesAfter;
  Atom* states = NULL;

  XGetWindowProperty(
    handle->display, handle->window, NET_WM_STATE,
    0, 1, False,
    XA_ATOM, &actualType, &actualFormat,
    &numItems, &bytesAfter, (unsigned char**)&states
  );

  for (unsigned long i = 0; i < numItems; i++) {
    if (
      states[i] == NET_WM_STATE_MAXIMIZED_VERT ||
      states[i] == NET_WM_STATE_MAXIMIZED_HORZ
    ) {
      // NOTE: States shouldn't be NULL if this code is executed.
      XFree(states);
      return true;
    }
  }

  if (states)
    XFree(states);
  return false;
}

static bool _isXWindowFullscreen(_DISLDisplayXorg* handle) {
  Atom actualType;
  int actualFormat;
  unsigned long numItems, bytesAfter;
  Atom* states = NULL;

  XGetWindowProperty(
    handle->display, handle->window, NET_WM_STATE,
    0, 1, False,
    XA_ATOM, &actualType, &actualFormat,
    &numItems, &bytesAfter, (unsigned char**)&states
  );

  for (unsigned long i = 0; i < numItems; i++) {
    if (states[i] == NET_WM_STATE_FULLSCREEN) {
      XFree(states);
      return true;
    }
  }

  if (states)
    XFree(states);
  return false;
}

///////////////////////////////////////////////////////////////////////////////
// Public Platform Functions
///////////////////////////////////////////////////////////////////////////////

DISLDisplay* _dislOpenDisplayXorg(const char* title, int x, int y, int width, int height) {
  assert(title != NULL);

  _DISLDisplayXorg* handle = (_DISLDisplayXorg*)calloc(1, sizeof(_DISLDisplayXorg));
  handle->display = XOpenDisplay(NULL);
  if (!handle->display) {
    free(handle);
    return NULL;
  }

  handle->root = XDefaultRootWindow(handle->display);

  DISLTransform transform = (DISLTransform){
    .x = x > 0 ? x : 0,
    .y = y > 0 ? y : 0,
    .width = width > 0 ? width : 0,
    .height = height > 0 ? height : 0
  };

  XSetWindowAttributes windowAttribs;
  windowAttribs.override_redirect = False;
  windowAttribs.border_pixel = 0;
  windowAttribs.event_mask =
    StructureNotifyMask | KeyPressMask | KeyReleaseMask | PointerMotionMask |
    ButtonPressMask | ButtonReleaseMask | ExposureMask | FocusChangeMask |
    VisibilityChangeMask | PropertyChangeMask;

  handle->window = XCreateWindow(
    handle->display,
    handle->root,
    transform.x, transform.y,
    transform.width, transform.height,
    0,
    CopyFromParent,
    InputOutput,
    CopyFromParent,
    CWOverrideRedirect | CWBorderPixel | CWEventMask,
    &windowAttribs
  );
  XMapWindow(handle->display, handle->window);
  XStoreName(handle->display, handle->window, title);

  // Initialize atoms.
  WM_STATE =
    XInternAtom(handle->display, "WM_STATE", False);
  WM_DELETE_WINDOW =
    XInternAtom(handle->display, "WM_DELETE_WINDOW", False);
  NET_WM_STATE =
    XInternAtom(handle->display, "_NET_WM_STATE", False);
  NET_WM_STATE_MAXIMIZED_HORZ =
    XInternAtom(handle->display, "_NET_WM_STATE_MAXIMIZED_HORZ", False);
  NET_WM_STATE_MAXIMIZED_VERT =
    XInternAtom(handle->display, "_NET_WM_STATE_MAXIMIZED_VERT", False);
  NET_WM_STATE_FULLSCREEN =
    XInternAtom(handle->display, "_NET_WM_STATE_FULLSCREEN", False);
  MOTIF_WM_HINTS =
    XInternAtom(handle->display, "_MOTIF_WM_HINTS", False);
  XSetWMProtocols(handle->display, handle->window, &WM_DELETE_WINDOW, 1);

  handle->shared.pub.active = true;
  handle->shared.title = title;
  handle->shared.transform = transform;
  return (DISLDisplay*)handle;
}

void _dislRetitleDisplayXorg(DISLDisplay* display, const char* title) {
  _DISLDisplayXorg* handle = (_DISLDisplayXorg*)display;
  XStoreName(handle->display, handle->window, title);
  handle->shared.title = title;
}

void _dislTransformDisplayXorg(DISLDisplay* display, int x, int y, int width, int height) {
  _DISLDisplayXorg* handle = (_DISLDisplayXorg*)display;
  XWindowAttributes attribs;
  XGetWindowAttributes(handle->display, handle->window, &attribs);

  DISLTransform transform = (DISLTransform){
    .x = x >= 0 ? x : attribs.x,
    .y = y >= 0 ? y : attribs.y,
    .width = width >= 0 ? width : attribs.width,
    .height = height >= 0 ? height : attribs.height
  };

  XMoveResizeWindow(
    handle->display,
    handle->window,
    transform.x, transform.y,
    transform.width, transform.height
  );
  handle->shared.transform = transform;
}

void _dislSetDisplayFlagsXorg(DISLDisplay* display, DISL_FLAGS flags) {
  _DISLDisplayXorg* handle = (_DISLDisplayXorg*)display;

  if (flags & DISL_FLAG_SHOWN) {
    XMapWindow(handle->display, handle->window);
  } else {
    XUnmapWindow(handle->display, handle->window);
    return;
  }

  // Minimize is applied first.
  if (flags & DISL_FLAG_MINIMIZED) {
    if (!_isXWindowMinimized(handle))
      XIconifyWindow(handle->display, handle->window, XDefaultScreen(handle->display));

    XFlush(handle->display);
    handle->shared.flags = flags;
    return;
  } else if (_isXWindowMinimized(handle)) {
    XMapWindow(handle->display, handle->window);
  }

  if (flags & DISL_FLAG_FULLSCREEN) {
    if (!_isXWindowFullscreen(handle)) {
      XEvent event = { ClientMessage };
      event.xclient.window = handle->window;
      event.xclient.format = 32;
      event.xclient.message_type = NET_WM_STATE;
      event.xclient.data.l[0] = _NET_WM_STATE_ADD;
      event.xclient.data.l[1] = NET_WM_STATE_FULLSCREEN;
      event.xclient.data.l[2] = 0;
      event.xclient.data.l[3] = 1;
      event.xclient.data.l[4] = 0;
      XSendEvent(
        handle->display,
        handle->root,
        False,
        SubstructureNotifyMask | SubstructureRedirectMask,
        &event
      );
    }
    XFlush(handle->display);
    handle->shared.flags = flags;
    return;
  } else if (_isXWindowFullscreen(handle)) {
    XEvent event = { ClientMessage };
    event.xclient.window = handle->window;
    event.xclient.format = 32;
    event.xclient.message_type = NET_WM_STATE;
    event.xclient.data.l[0] = _NET_WM_STATE_REMOVE;
    event.xclient.data.l[1] = NET_WM_STATE_FULLSCREEN;
    event.xclient.data.l[2] = 0;
    event.xclient.data.l[3] = 1;
    event.xclient.data.l[4] = 0;
    XSendEvent(
      handle->display,
      handle->root,
      False,
      SubstructureNotifyMask | SubstructureRedirectMask,
      &event
    );
  }

  {
    unsigned long hints[] = {
      MWM_HINTS_DECORATIONS,
      0,
      (flags & DISL_FLAG_BORDERLESS) ? 0 : MWM_DECOR_ALL,
      0,
      0
    };
    XChangeProperty(
      handle->display,
      handle->window,
      MOTIF_WM_HINTS,
      MOTIF_WM_HINTS,
      32,
      PropModeReplace,
      (unsigned char*)&hints,
      sizeof(hints) / sizeof(long)
    );
  }

  if (flags & DISL_FLAG_MAXIMIZED && !_isXWindowMaximized(handle)) {
    XEvent event = {0};
    event.type = ClientMessage;
    event.xclient.window = handle->window;
    event.xclient.message_type = NET_WM_STATE;
    event.xclient.format = 32;
    event.xclient.data.l[0] = _NET_WM_STATE_ADD;
    event.xclient.data.l[1] = NET_WM_STATE_MAXIMIZED_HORZ;
    event.xclient.data.l[2] = NET_WM_STATE_MAXIMIZED_VERT;
    XSendEvent(
      handle->display,
      handle->root,
      False,
      SubstructureNotifyMask | SubstructureRedirectMask,
      &event
    );
  } else if (_isXWindowMaximized(handle)) {
    XEvent event = {0};
    event.type = ClientMessage;
    event.xclient.window = handle->window;
    event.xclient.message_type = NET_WM_STATE;
    event.xclient.format = 32;
    event.xclient.data.l[0] = _NET_WM_STATE_REMOVE;
    event.xclient.data.l[1] = NET_WM_STATE_MAXIMIZED_HORZ;
    event.xclient.data.l[2] = NET_WM_STATE_MAXIMIZED_VERT;
    XSendEvent(
      handle->display,
      handle->root,
      False,
      SubstructureNotifyMask | SubstructureRedirectMask,
      &event
    );
  }

  XFlush(handle->display);
  handle->shared.flags = flags;
}

void _dislPollEventsXorg(DISLDisplay* display) {
  assert(display != NULL);

  _DISLDisplayXorg* handle = (_DISLDisplayXorg*)display;
  XEvent event;

  XPending(handle->display);
  while (QLength(handle->display)) {
    XNextEvent(handle->display, &event);
    switch (event.type) {
    case Expose:
      if (display->hooks.refresh)
        display->hooks.refresh(display);
      break;

    case FocusIn:
    case FocusOut:
      if (display->hooks.focus)
        display->hooks.focus(display, event.xfocus.type == FocusIn);
      break;

    case KeyPress:
      if (display->hooks.key) {
        KeySym key = XkbKeycodeToKeysym(
          handle->display,
          event.xkey.keycode,
          0,
          event.xkey.state & ShiftMask ? 1 : 0
        );
        display->hooks.key(display, _translateXorgInput(key), true);
      }
      break;

    case KeyRelease:
      if (display->hooks.key) {
        // To prevent inputs from being repeated...
        if (QLength(handle->display)) {
          XEvent nextEvent;
          XPeekEvent(handle->display, &nextEvent);

          if (
            (nextEvent.type == KeyPress &&
            nextEvent.xkey.time == event.xkey.time) &&
            nextEvent.xkey.keycode == event.xkey.keycode
          ) {
            XNextEvent(handle->display, &event);
            break;
          }
        }

        KeySym key = XkbKeycodeToKeysym(
          handle->display,
          event.xkey.keycode,
          0,
          event.xkey.state & ShiftMask ? 1 : 0
        );
        display->hooks.key(display, _translateXorgInput(key), false);
      }
      break;

    case MotionNotify:
      if (display->hooks.mouse)
        display->hooks.mouse(
          display,
          DISL_MOUSE_EVENT_MOVE,
          event.xmotion.x,
          event.xmotion.y
        );
      break;

    case ButtonPress:
    case ButtonRelease:
      if (display->hooks.mouse) {
        bool isDown = event.xbutton.type == ButtonPress;

        if (event.xbutton.state & Button1) {
          display->hooks.mouse(
            display,
            isDown ? DISL_MOUSE_EVENT_BUTTON1DOWN : DISL_MOUSE_EVENT_BUTTON1UP,
            event.xbutton.x,
            event.xbutton.y
          );
        } else if (event.xbutton.state & Button2) {
          display->hooks.mouse(
            display,
            isDown ? DISL_MOUSE_EVENT_BUTTON2DOWN : DISL_MOUSE_EVENT_BUTTON2UP,
            event.xbutton.x,
            event.xbutton.y
          );
        } else if (event.xbutton.state & Button3) {
          display->hooks.mouse(
            display,
            isDown ? DISL_MOUSE_EVENT_BUTTON3DOWN : DISL_MOUSE_EVENT_BUTTON3UP,
            event.xbutton.x,
            event.xbutton.y
          );
        }
      }
      break;

    case ConfigureNotify: {
      DISLTransform transform = (DISLTransform){
        .x = event.xconfigure.x,
        .y = event.xconfigure.y,
        .width = event.xconfigure.width,
        .height = event.xconfigure.height
      };
      bool sizeChanged =
        handle->shared.transform.width != transform.width ||
        handle->shared.transform.height != transform.height;
      bool posChanged =
        handle->shared.transform.x != transform.x ||
        handle->shared.transform.y != transform.y;

      if (sizeChanged || posChanged)
        handle->shared.transform = transform;

      if (display->hooks.resize && sizeChanged)
        display->hooks.resize(display, transform);
    } break;

    case PropertyNotify: {
      if (event.xproperty.atom == WM_STATE) {
        bool minimized = _isXWindowMinimized(handle);

        if (minimized && !(handle->shared.flags & DISL_FLAG_MINIMIZED)) {
          handle->shared.flags |= DISL_FLAG_MINIMIZED;
          if (display->hooks.iconify)
            display->hooks.iconify(display, true);

        } else if (!minimized && (handle->shared.flags & DISL_FLAG_MINIMIZED)) {
          handle->shared.flags &= ~DISL_FLAG_MINIMIZED;
          if (display->hooks.iconify)
            display->hooks.iconify(display, false);

        }
      } else if (event.xproperty.atom == NET_WM_STATE) {
        bool maximized = _isXWindowMaximized(handle);

        if (maximized && !(handle->shared.flags & DISL_FLAG_MAXIMIZED)) {
          handle->shared.flags |= DISL_FLAG_MAXIMIZED;
          if (display->hooks.maximize)
            display->hooks.maximize(display, maximized);

        } else if (!maximized && (handle->shared.flags & DISL_FLAG_MAXIMIZED)) {
          handle->shared.flags &= ~DISL_FLAG_MAXIMIZED;
          if (display->hooks.maximize)
            display->hooks.maximize(display, maximized);

        }
      }
    } break;

    case ClientMessage: {
      XClientMessageEvent* xmsg = (XClientMessageEvent*)&event;
      if ((Atom)xmsg->data.l[0] == WM_DELETE_WINDOW)
        display->active = false;
    } break;
    default:
      break;
    }
  }
  XFlush(handle->display);
}

void _dislCloseDisplayXorg(DISLDisplay* display) {
  assert(display != NULL);

  _DISLDisplayXorg* handle = (_DISLDisplayXorg*)display;

  if (handle->window) {
    XUnmapWindow(handle->display, handle->window);
    XDestroyWindow(handle->display, handle->window);
  }

  XFlush(handle->display);
  XCloseDisplay(handle->display);
  free(handle);
}

