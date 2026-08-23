
#include <stdio.h>
#include <glad/gl.h>
#include <disl.h>
#include <dgl.h>

static DISLDisplay* display = NULL;
static DGLContext* context = NULL;

void drawRect(
  double x, double y,
  double width, double height,
  double r, double g, double b
) {
  glColor3f(r, g, b);
  glVertex2f(x, y);
  glVertex2f(x + width, y);
  glVertex2f(x + width, y - height);
  glVertex2f(x, y - height);
}

void draw() {
  DISL_FLAGS activeFlags = dislGetDisplayFlags(display);

  glClearColor(0.1, 0.1, 0.1, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
  glBegin(GL_QUADS);

  if (activeFlags & DISL_FLAG_MINIMIZED)
    drawRect(
      -0.7, 0.05,
      0.2, 0.2,
      1.0, 1.0, 1.0
    );
  else
    drawRect(
      -0.7, 0.05,
      0.2, 0.2,
      0.4, 0.4, 0.4
    );

  if (activeFlags & DISL_FLAG_MAXIMIZED)
    drawRect(
      -0.3, 0.05,
      0.2, 0.2,
      1.0, 1.0, 1.0
    );
  else
    drawRect(
      -0.3, 0.05,
      0.2, 0.2,
      0.4, 0.4, 0.4
    );

  if (activeFlags & DISL_FLAG_BORDERLESS)
    drawRect(
      0.1, 0.05,
      0.2, 0.2,
      1.0, 1.0, 1.0
    );
  else
    drawRect(
      0.1, 0.05,
      0.2, 0.2,
      0.4, 0.4, 0.4
    );

  if (activeFlags & DISL_FLAG_FULLSCREEN)
    drawRect(
      0.5, 0.05,
      0.2, 0.2,
      1.0, 1.0, 1.0
    );
  else
    drawRect(
      0.5, 0.05,
      0.2, 0.2,
      0.4, 0.4, 0.4
    );

  glEnd();
  glFlush();

  dglSwapBuffers(display);
}

void keyProc(DISLDisplay* display, DISL_KEYCODE keycode, bool pressed) {
  DISL_FLAGS activeFlags = dislGetDisplayFlags(display);
  if (pressed) {
    switch (keycode) {
    case DISL_KEYCODE_1:
      activeFlags ^= DISL_FLAG_MINIMIZED;
      dislSetDisplayFlags(display, activeFlags);
      printf("Display minimized\n");
      break;
    case DISL_KEYCODE_2:
      activeFlags ^= DISL_FLAG_MAXIMIZED;
      dislSetDisplayFlags(display, activeFlags);
      printf("Display maximized\n");
      break;
    case DISL_KEYCODE_3:
      activeFlags ^= DISL_FLAG_BORDERLESS;
      dislSetDisplayFlags(display, activeFlags);
      printf("Borderless display toggled\n");
      break;
    case DISL_KEYCODE_4:
      activeFlags ^= DISL_FLAG_FULLSCREEN;
      dislSetDisplayFlags(display, activeFlags);
      printf("Fullscreen toggled\n");
      break;
    default: break;
    }
  }
}

void transformProc(DISLDisplay* display, DISLTransform transform) {
  glViewport(0, 0, transform.width, transform.height);
}

int close(int code) {
  if (context) {
    dglMakeCurrent(NULL, NULL);
    dglDeleteContext(context);
    context = NULL;
  }
  if (display) {
    dislCloseDisplay(display);
    display = NULL;
  }
  return code;
}



int main() {
  DISLHooks hooks = {0};
  display = dislOpenDisplay(
    "DGL Flag Test",
    (DISLTransform){-1, -1, 800, 600},
    0,
    hooks
  );
  if (!display)
    return close(1);
  printf("Opened display\n");

  DGLConfig config = (DGLConfig){
    .glMajor = 1,
    .glMinor = 1,
    .compatibility = true,

    .redBits = 8,
    .greenBits = 8,
    .blueBits = 8,
    .alphaBits = 8,
    .depthBits = 0,
    .stencilBits = 0
  };
  context = dglCreateContext(display, &config);
  if (!context)
    return close(2);

  if (!dglMakeCurrent(display, context))
    return close(3);

  if (!gladLoadGL((GLADloadfunc)(dglGetProcAddress)))
    return close(4);

  display->hooks.resize = transformProc;
  display->hooks.key = keyProc;

  glViewport(0, 0, 800, 600);
  draw();
  dislSetDisplayFlags(display, DISL_FLAG_SHOWN);

  while (display->active) {
    draw();
    dislPollEvents(display);
  }

  return close(0);
}

