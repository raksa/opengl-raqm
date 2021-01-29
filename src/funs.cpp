// https://www.opengl.org/archives/resources/code/samples/glut_examples/examples/bitfont.c
// https://github.com/themattinthehatt/tutorials/tree/master/opengl-tutorial/playground
// https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Text_Rendering_01
// https://gitlab.com/wikibooks-opengl/modern-tutorials
#include <GLUT/glut.h>
#include <string.h>

#include "load_shader.h"

extern "C"
{
#include "raqm_utils.h"
}

FT_Library ft;
FT_Face face;
GLuint program;
GLint attribute_coord;
GLint uniform_tex;
GLint uniform_color;
GLuint vbo;
struct point
{
  GLfloat x;
  GLfloat y;
  GLfloat s;
  GLfloat t;
};
void draw_text(const char *text,
               float x, float y, float sx, float sy);

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

  float sx = 2.0 / glutGet(GLUT_WINDOW_WIDTH);
  float sy = 2.0 / glutGet(GLUT_WINDOW_HEIGHT);
  draw_text("សោះស្តី", -1 + 8 * sx, 1 - 50 * sy, sx, sy);
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

int init_resources()
{

  /* Initialize the FreeType2 library */
  if (FT_Init_FreeType(&ft))
  {
    fprintf(stderr, "Could not init freetype library\n");
    return 0;
  }

  /* Load a font */
  const char *fontfilename = "/Users/raksa/Desktop/dev/opengl-raqm/assets/fonts/Battambang-Regular.ttf";
  if (FT_New_Face(ft, fontfilename, 0, &face))
  {
    fprintf(stderr, "Could not open font %s\n", fontfilename);
    return 0;
  }

  program = loadShader(
      "/Users/raksa/Desktop/dev/opengl-raqm/assets/shader/vertex.glsl",
      "/Users/raksa/Desktop/dev/opengl-raqm/assets/shader/fragment.glsl");

  const char *attribute_name;
  attribute_name = "coord";
  attribute_coord = glGetAttribLocation(program, attribute_name);
  if (attribute_coord == -1)
  {
    fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
    return 0;
  }

  const char *uniform_name;
  uniform_name = "tex";
  uniform_tex = glGetUniformLocation(program, uniform_name);
  if (uniform_tex == -1)
  {
    fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
    return 0;
  }

  uniform_name = "color";
  uniform_color = glGetUniformLocation(program, uniform_name);
  if (uniform_color == -1)
  {
    fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
    return 0;
  }

  // Create the vertex buffer object
  glGenBuffers(1, &vbo);

  return 1;
}

void free_resources()
{
  glDeleteProgram(program);
}

void draw_text(const char *text,
               float x, float y, float sx, float sy)
{
  /* Create a texture that will be used to hold one "glyph" */
  GLuint tex;
  size_t count, i;
  raqm_glyph_t raqmGlyph;
  FT_Bitmap bitmap;
  FT_GlyphSlot glyp;
  raqm_glyph_t *raqmGlyphs = getGlyphs(&face, &count, text, "l", "km");

  glActiveTexture(GL_TEXTURE0);
  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D, tex);
  glUniform1i(uniform_tex, 0);

  /* We require 1 byte alignment when uploading texture data */
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  /* Clamping to edges is important to prevent artifacts when scaling */
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  /* Linear filtering usually looks best for text */
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  /* Set up the VBO for our vertex data */
  glEnableVertexAttribArray(attribute_coord);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer(attribute_coord, 4, GL_FLOAT, GL_FALSE, 0, 0);

  /* Loop through all characters */
  for (i = 0; i < count; i++)
  {
    raqmGlyph = raqmGlyphs[i];
    bitmap = raqmGlyph.ftface->glyph->bitmap;
    glyp = raqmGlyph.ftface->glyph;
    /* Upload the "bitmap", which contains an 8-bit grayscale image, as an alpha texture */
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, bitmap.width, bitmap.rows,
                 0, GL_RED, GL_UNSIGNED_BYTE, bitmap.buffer);

    /* Calculate the vertex and texture coordinates */
    float x2 = x + glyp->bitmap_left * sx;
    float y2 = -y - glyp->bitmap_top * sy;
    float w = bitmap.width * sx;
    float h = bitmap.rows * sy;

    point box[4] = {
        {x2, -y2, 0, 0},
        {x2 + w, -y2, 1, 0},
        {x2, -y2 - h, 0, 1},
        {x2 + w, -y2 - h, 1, 1},
    };

    /* Draw the character on the screen */
    glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    /* Advance the cursor to the start of the next character */
    x += (glyp->advance.x >> 6) * sx;
    y += (glyp->advance.y >> 6) * sy;
  }

  glDisableVertexAttribArray(attribute_coord);
  glDeleteTextures(1, &tex);
}