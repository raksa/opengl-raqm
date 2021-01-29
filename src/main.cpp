// https://www.opengl.org/archives/resources/features/fontsurvey/
#include <GLUT/glut.h>
#include <string.h>

#include "main_opengl_utils.h"

int main(int argc, char **argv)
{
   int i, msg_submenu, color_submenu;

   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize(700, 500);
   glutCreateWindow("GLUT bitmap font example");
   glClearColor(0.0, 0.0, 0.0, 1.0);
   glutReshapeFunc(reshape);
   glutIdleFunc(tick);
   msg_submenu = glutCreateMenu(selectMessage);
   glutAddMenuEntry("abc", 1);
   glutAddMenuEntry("ABC", 2);
   color_submenu = glutCreateMenu(selectColor);
   glutAddMenuEntry("Green", 1);
   glutAddMenuEntry("Red", 2);
   glutAddMenuEntry("White", 3);
   glutCreateMenu(selectFont);
   glutAddMenuEntry("9 by 15", 0);
   glutAddMenuEntry("Times Roman 10", 1);
   glutAddMenuEntry("Times Roman 24", 2);
   glutAddSubMenu("Messages", msg_submenu);
   glutAddSubMenu("Color", color_submenu);
   glutAttachMenu(GLUT_RIGHT_BUTTON);
   if (init_resources())
   {
      glutDisplayFunc(display);
      glutMainLoop();
   }

   free_resources();
   return 0; /* ANSI C requires main to return int. */
}