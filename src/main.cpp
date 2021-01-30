#include <GLUT/glut.h>
#include <string.h>

#include "funs.h"
#include "utils.h"

int main(int argc, char **argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_RGB);
   glutInitWindowSize(700, 500);
   glutCreateWindow("OpenGL+Raqm");
   glutReshapeFunc(reshape);
   glutIdleFunc(tick);

   if (init_resources())
   {
      glutDisplayFunc(display);
      glutMainLoop();
   }

   free_resources();
   return 0; /* ANSI C requires main to return int. */
}