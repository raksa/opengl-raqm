#include <GLUT/glut.h>
#include <string.h>

#include "funs.h"

void *font = GLUT_BITMAP_TIMES_ROMAN_24;
void *fonts[] =
    {
        GLUT_BITMAP_9_BY_15,
        GLUT_BITMAP_TIMES_ROMAN_10,
        GLUT_BITMAP_TIMES_ROMAN_24};
char defaultMessage[] = "GLUT means OpenGL.";
char *message = defaultMessage;

void selectFont(int newfont)
{
  font = fonts[newfont];
  glutPostRedisplay();
}

void selectMessage(int msg)
{
  switch (msg)
  {
  case 1:
    message = "abcdefghijklmnop";
    break;
  case 2:
    message = "ABCDEFGHIJKLMNOP";
    break;
  }
}

void selectColor(int color)
{
  switch (color)
  {
  case 1:
    glColor3f(0.0, 1.0, 0.0);
    break;
  case 2:
    glColor3f(1.0, 0.0, 0.0);
    break;
  case 3:
    glColor3f(1.0, 1.0, 1.0);
    break;
  }
  glutPostRedisplay();
}

void tick(void)
{
  glutPostRedisplay();
}

void output(int x, int y, char *string)
{
  int len, i;

  glRasterPos2f(x, y);
  len = (int)strlen(string);
  for (i = 0; i < len; i++)
  {
    glutBitmapCharacter(font, string[i]);
  }
}

void display(void)
{
  glClearColor(0.5, 0, 0.5, 1);
  glClear(GL_COLOR_BUFFER_BIT);

  // output(0, 24, "This is written in a GLUT bitmap font.");
  // output(100, 100, message);
  // output(50, 145, "(positioned in pixels with upper-left origin)");

  apply_draw_text();

  glutSwapBuffers();
}

void reshape(int w, int h)
{
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, w, h, 0);
  glMatrixMode(GL_MODELVIEW);
}
