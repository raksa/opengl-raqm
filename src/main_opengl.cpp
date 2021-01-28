#include <GLUT/glut.h>

#include "main_opengl_utils.h"

int main(int argc, char *argv[])
{

  //a basic set up...
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(640, 480);

  //create the window, the argument is the title
  glutCreateWindow("GLUT program");

  //pass the callbacks
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutIdleFunc(idle);

  glutMainLoop();

  //we never get here because glutMainLoop() is an infinite loop
  return 0;
}