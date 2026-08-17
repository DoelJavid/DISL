/**
  Display & Input System Library (DISL)

  Written by DoelJavid.
*/

#ifndef DISL_H
#define DISL_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

typedef uint8_t DISL_FLAGS;
#define DISL_FLAG_SHOWN      0b00000001
#define DISL_FLAG_MINIMIZED  0b00000010
#define DISL_FLAG_MAXIMIZED  0b00000100
#define DISL_FLAG_BORDERLESS 0b01000000
#define DISL_FLAG_FULLSCREEN 0b10000000

typedef enum {
  DISL_BACKEND_NONE = 0,
  DISL_BACKEND_XORG,
  DISL_BACKEND_WIN32,
  DISL_BACKEND_CUSTOM
} DISL_BACKEND;

typedef enum {
  DISL_KEYCODE_NULL = 0,

  DISL_KEYCODE_ESCAPE,
  DISL_KEYCODE_PRINT_SCREEN,
  DISL_KEYCODE_LEFT_CTRL,
  DISL_KEYCODE_RIGHT_CTRL,
  DISL_KEYCODE_LEFT_ALT,
  DISL_KEYCODE_RIGHT_ALT,
  DISL_KEYCODE_LEFT_SHIFT,
  DISL_KEYCODE_RIGHT_SHIFT,
  DISL_KEYCODE_TAB,
  DISL_KEYCODE_SPACE,
  DISL_KEYCODE_ENTER,
  DISL_KEYCODE_BACKSPACE,
  DISL_KEYCODE_CAPS_LOCK,
  DISL_KEYCODE_SCROLL_LOCK,
  DISL_KEYCODE_PAUSE,
  DISL_KEYCODE_BREAK,
  DISL_KEYCODE_NUMLOCK,
  DISL_KEYCODE_HOME,
  DISL_KEYCODE_END,
  DISL_KEYCODE_INSERT,
  DISL_KEYCODE_DELETE,
  DISL_KEYCODE_SYSREQ,

  DISL_KEYCODE_Q,
  DISL_KEYCODE_W,
  DISL_KEYCODE_E,
  DISL_KEYCODE_R,
  DISL_KEYCODE_T,
  DISL_KEYCODE_Y,
  DISL_KEYCODE_U,
  DISL_KEYCODE_I,
  DISL_KEYCODE_O,
  DISL_KEYCODE_P,
  DISL_KEYCODE_A,
  DISL_KEYCODE_S,
  DISL_KEYCODE_D,
  DISL_KEYCODE_F,
  DISL_KEYCODE_G,
  DISL_KEYCODE_H,
  DISL_KEYCODE_J,
  DISL_KEYCODE_K,
  DISL_KEYCODE_L,
  DISL_KEYCODE_Z,
  DISL_KEYCODE_X,
  DISL_KEYCODE_C,
  DISL_KEYCODE_V,
  DISL_KEYCODE_B,
  DISL_KEYCODE_N,
  DISL_KEYCODE_M,

  DISL_KEYCODE_1,
  DISL_KEYCODE_2,
  DISL_KEYCODE_3,
  DISL_KEYCODE_4,
  DISL_KEYCODE_5,
  DISL_KEYCODE_6,
  DISL_KEYCODE_7,
  DISL_KEYCODE_8,
  DISL_KEYCODE_9,
  DISL_KEYCODE_0,

  DISL_KEYCODE_SEMICOLON,
  DISL_KEYCODE_COMMA,
  DISL_KEYCODE_PERIOD,
  DISL_KEYCODE_QUOTE,
  DISL_KEYCODE_FORWARDSLASH,
  DISL_KEYCODE_BACKSLASH,
  DISL_KEYCODE_MINUS,
  DISL_KEYCODE_EQUALS,
  DISL_KEYCODE_BACKTICK,
  DISL_KEYCODE_LEFT_BRACKET,
  DISL_KEYCODE_RIGHT_BRACKET,

  DISL_KEYCODE_NUMPAD_0,
  DISL_KEYCODE_NUMPAD_1,
  DISL_KEYCODE_NUMPAD_2,
  DISL_KEYCODE_NUMPAD_3,
  DISL_KEYCODE_NUMPAD_4,
  DISL_KEYCODE_NUMPAD_5,
  DISL_KEYCODE_NUMPAD_6,
  DISL_KEYCODE_NUMPAD_7,
  DISL_KEYCODE_NUMPAD_8,
  DISL_KEYCODE_NUMPAD_9,

  DISL_KEYCODE_NUMPAD_PLUS,
  DISL_KEYCODE_NUMPAD_MINUS,
  DISL_KEYCODE_NUMPAD_STAR,
  DISL_KEYCODE_NUMPAD_SLASH,
  DISL_KEYCODE_NUMPAD_ENTER,
  DISL_KEYCODE_NUMPAD_DELETE,

  DISL_KEYCODE_F1,
  DISL_KEYCODE_F2,
  DISL_KEYCODE_F3,
  DISL_KEYCODE_F4,
  DISL_KEYCODE_F5,
  DISL_KEYCODE_F6,
  DISL_KEYCODE_F7,
  DISL_KEYCODE_F8,
  DISL_KEYCODE_F9,
  DISL_KEYCODE_F10,
  DISL_KEYCODE_F11,
  DISL_KEYCODE_F12,
  DISL_KEYCODE_F13,
  DISL_KEYCODE_F14,
  DISL_KEYCODE_F15,
  DISL_KEYCODE_F16,
  DISL_KEYCODE_F17,
  DISL_KEYCODE_F18,
  DISL_KEYCODE_F19,
  DISL_KEYCODE_F20,
  DISL_KEYCODE_F21,
  DISL_KEYCODE_F22,
  DISL_KEYCODE_F23,
  DISL_KEYCODE_F24,
  DISL_KEYCODE_F25,

  DISL_KEYCODE_UP,
  DISL_KEYCODE_DOWN,
  DISL_KEYCODE_LEFT,
  DISL_KEYCODE_RIGHT,
  DISL_KEYCODE_PAGE_UP,
  DISL_KEYCODE_PAGE_DOWN,

  DISL_KEYCODE_LEFT_SUPER,
  DISL_KEYCODE_RIGHT_SUPER,
  DISL_KEYCODE_MENU
} DISL_KEYCODE;

typedef enum {
  DISL_MOUSE_EVENT_MOVE = 0,
  DISL_MOUSE_EVENT_SCROLL,
  DISL_MOUSE_EVENT_BUTTON1DOWN,
  DISL_MOUSE_EVENT_BUTTON1UP,
  DISL_MOUSE_EVENT_BUTTON2DOWN,
  DISL_MOUSE_EVENT_BUTTON2UP,
  DISL_MOUSE_EVENT_BUTTON3DOWN,
  DISL_MOUSE_EVENT_BUTTON3UP
} DISL_MOUSE_EVENT;

typedef struct DISLTransform DISLTransform;
typedef struct DISLHooks DISLHooks;
typedef struct DISLDisplay DISLDisplay;

typedef void (*DISLRefreshHook)(DISLDisplay* display);
typedef void (*DISLTransformHook)(DISLDisplay* display, DISLTransform transform);
typedef void (*DISLFocusHook)(DISLDisplay* display, bool focused);
typedef void (*DISLMaximizeHook)(DISLDisplay* display, bool maximized);
typedef void (*DISLIconifyHook)(DISLDisplay* display, bool visible);
typedef void (*DISLKeyHook)(DISLDisplay* display, DISL_KEYCODE keycode, bool pressed);
typedef void (*DISLMouseHook)(DISLDisplay* display, DISL_MOUSE_EVENT action, int x, int y);

struct DISLTransform {
  int x, y, width, height;
};

struct DISLHooks {
  DISLRefreshHook refresh;
  DISLTransformHook resize;
  DISLFocusHook focus;
  DISLMaximizeHook maximize;
  DISLIconifyHook iconify;
  DISLKeyHook key;
  DISLMouseHook mouse;
};

struct DISLDisplay {
  bool active;
  // const char* title;
  // DISLTransform transform;
  DISLHooks hooks;
  void* userdata;
  // void* handle;
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

typedef struct {
  DISLDisplay* (*openDisplay)(const char* title, DISLTransform transform, DISL_FLAGS flags, DISLHooks procs);
  void (*retitleDisplay)(DISLDisplay* display, const char* title);
  void (*transformDisplay)(DISLDisplay* display, DISLTransform transform);
  void (*setDisplayFlags)(DISLDisplay* display, DISL_FLAGS flags);
  void (*pollEvents)(DISLDisplay* display);
  void (*closeDisplay)(DISLDisplay* display);
} DISLPlatform;

extern const DISLPlatform* _disl;

/**
  Returns the currently active display backend.
*/
DISL_BACKEND dislGetDisplayBackend();

/**
  Creates a new display with the given information. If no display backend is
  currently active, this function will automatically select a backend based on
  the current platform.
*/
DISLDisplay* dislOpenDisplay(const char* title, DISLTransform transform, DISL_FLAGS flags, DISLHooks procs);

/**
  Retrieves the title of the given display.
*/
const char* dislGetDisplayTitle(DISLDisplay* display);

/**
  Retrieves the transform of the given display.
*/
DISLTransform dislGetDisplayTransform(DISLDisplay* display);

/**
  Retrieves the currently active display flags in the given display.
*/
DISL_FLAGS dislGetDisplayFlags(DISLDisplay* display);

/**
  Sets the display flags within the given display. This can be used to toggle
  certain settings within the display, like whether or not the display can be
  resized, or to make the window borderless.
*/
#define dislRetitleDisplay(display, title) _disl->setDisplayTitle(display, title)

/**
  Moves and/or resizes the given display using the given transform struct. If
  any transform component is negative, that component will be ignored.
*/
#define dislTransformDisplay(display, transform) _disl->transformDisplay(display, transform)

/**
  Applies the given display flags within the given display. This can be used to
  toggle certain settings within the display, like whether or not the display
  can be resized, or to make the window borderless.
*/
#define dislSetDisplayFlags(display, flags) _disl->setDisplayFlags(display, flags)

/**
  Polls the display for any incoming events, then fires each event's dedicated
  hook. Event hooks can be set through the `hooks` property within the display
  struct.
*/
#define dislPollEvents(display) _disl->pollEvents(display)

/**
  Closes the given display, freeing all resources related to it.
*/
#define dislCloseDisplay(display) _disl->closeDisplay(display)

#endif
