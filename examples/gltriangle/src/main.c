
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



void onResize(DISLDisplay* display, DISLTransform transform) {
  printf("New size: %d, %d\n", transform.width, transform.height);
  glViewport(0, 0, transform.width, transform.height);
}

void onKeyAction(DISLDisplay* display, DISL_KEYCODE keycode, bool pressed) {
  if (pressed)
    printf("Key %d pressed\n", (int)keycode);
}



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

int cleanup(int code) {
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
  display = dislOpenDisplay("DGL Triangle", -1, -1, 800, 600);
  if (!display)
    return cleanup(1);

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
    return cleanup(2);

  if (!dglMakeCurrent(display, context))
    return cleanup(3);

  if (!gladLoadGL((GLADloadfunc)(dglGetProcAddress)))
    return cleanup(4);

  display->hooks.resize = onResize;
  display->hooks.key = onKeyAction;

  glViewport(0, 0, 800, 600);
  draw();
  dislSetDisplayFlags(display, DISL_FLAG_SHOWN);

  while (display->active) {
    draw();
    dislPollEvents(display);
  }

  return cleanup(0);
}

