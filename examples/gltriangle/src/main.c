
#include <stdio.h>
#include <GL/gl.h>
#include <disl.h>
#include <dgl.h>

void keyProc(DISLDisplay* display, DISL_KEYCODE keycode, bool pressed) {
  if (pressed)
    printf("Key %d pressed\n", (int)keycode);
}

void transformProc(DISLDisplay* display, DISLTransform transform) {
  glViewport(0, 0, transform.width, transform.height);
}

int main() {
  DISLHooks hooks = {0};
  hooks.resize = transformProc;
  hooks.key = keyProc;
  DISLDisplay* display = dislOpenDisplay(
    "DGL Triangle",
    (DISLTransform){0, 0, 800, 600},
    0,
    hooks
  );
  if (!display) {
    printf("Failed to initialize display!\n");
    return 1;
  }

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
  DGLContext* context = dglCreateContext(display, &config);
  if (!context) {
    printf("Failed to create context!\n");
    dislCloseDisplay(display);
    return 2;
  }
  dglMakeCurrent(display, context);

  glViewport(0, 0, 800, 600);
  while (display->active) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
		glBegin(GL_TRIANGLES);

    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2i(0, 1);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2i(-1, -1);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex2i(1, -1);

		glEnd();
		glFlush();

    dglSwapBuffers(display);
    dislPollEvents(display);
  }

  dglDeleteContext(context);
  dislCloseDisplay(display);
  return 0;
}

