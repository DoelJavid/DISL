
#include <stdio.h>
#include <math.h>
#include <glad/gl.h>
#include <disl.h>
#include <dgl.h>

#define PI2 6.283185307179586476925286766559
#define TRI_SIZE 0.5

static DISLDisplay* display = NULL;
static DGLContext* context = NULL;
static double rotation = 0.0;

void draw() {
  rotation += 0.003;

  glClearColor(0.0, 0.0333, 0.0667, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
  glBegin(GL_TRIANGLES);

  glColor3f(1.0f, 0.0f, 0.0f);
  glVertex2f(sin(rotation) * TRI_SIZE, cos(rotation) * TRI_SIZE);

  glColor3f(0.0f, 1.0f, 0.0f);
  glVertex2f(sin(rotation + (PI2 * 0.333)) * TRI_SIZE, cos(rotation + (PI2 * 0.333)) * TRI_SIZE);

  glColor3f(0.0f, 0.0f, 1.0f);
  glVertex2f(sin(rotation + (PI2 * 0.667)) * TRI_SIZE, cos(rotation + (PI2 * 0.667)) * TRI_SIZE);

  glEnd();
  glFlush();

  dglSwapBuffers(display);
}

void keyProc(DISLDisplay* display, DISL_KEYCODE keycode, bool pressed) {
  if (pressed)
    printf("Key %d pressed\n", (int)keycode);
}

void transformProc(DISLDisplay* display, DISLTransform transform) {
  printf("New size: %d, %d\n", transform.width, transform.height);
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
    "DGL Triangle",
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

