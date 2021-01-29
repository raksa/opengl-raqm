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
// const char *fontfilename = "/Users/raksa/Desktop/dev/opengl-raqm/assets/fonts/Battambang-Regular.ttf";
const char *fontfilename = "/Users/raksa/Desktop/dev/opengl-raqm/assets/fonts/POPPINS-BOLD.TTF";
const char *vertShader = "/Users/raksa/Desktop/dev/opengl-raqm/assets/shader/vertex.glsl";
const char *fragShader = "/Users/raksa/Desktop/dev/opengl-raqm/assets/shader/fragment.glsl";

int init_resources()
{

  /* Initialize the FreeType2 library */
  if (FT_Init_FreeType(&ft))
  {
    fprintf(stderr, "Could not init freetype library\n");
    return 0;
  }

  /* Load a font */
  if (FT_New_Face(ft, fontfilename, 0, &face))
  {
    fprintf(stderr, "Could not open font %s\n", fontfilename);
    return 0;
  }

  FT_Set_Pixel_Sizes(face, 0, 48);

  program = loadShader(vertShader, fragShader);

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

void draw_text(const char *text, float x, float y, float sx, float sy)
{
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  GLuint tex;

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
  FT_GlyphSlot glyph = face->glyph;
  const char *p;
  for (p = text; *p; p++)
  {
    if (FT_Load_Char(face, *p, FT_LOAD_RENDER))
    {
      fprintf(stderr, "Could not load character 'X'\n");
      return;
    }
    /* Upload the "bitmap", which contains an 8-bit grayscale image, as an alpha texture */
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, glyph->bitmap.width, glyph->bitmap.rows,
                 0, GL_RED, GL_UNSIGNED_BYTE, glyph->bitmap.buffer);

    /* Calculate the vertex and texture coordinates */
    float x2 = x + glyph->bitmap_left * sx;
    float y2 = -y - glyph->bitmap_top * sy;
    float w = glyph->bitmap.width * sx;
    float h = glyph->bitmap.rows * sy;

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
    x += (glyph->advance.x >> 6) * sx;
    y += (glyph->advance.y >> 6) * sy;
  }

  glDisableVertexAttribArray(attribute_coord);
  glDeleteTextures(1, &tex);
}

void draw_text_raqm(const char *text, float x, float y, float sx, float sy)
{
  /* Create a texture that will be used to hold one "glyph" */
  GLuint tex;
  size_t count, i;
  raqm_glyph_t raqmGlyph;
  FT_Bitmap bitmap;
  FT_GlyphSlot glyph;
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
    glyph = raqmGlyph.ftface->glyph;
    /* Upload the "bitmap", which contains an 8-bit grayscale image, as an alpha texture */
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, bitmap.width, bitmap.rows,
                 0, GL_RED, GL_UNSIGNED_BYTE, bitmap.buffer);

    /* Calculate the vertex and texture coordinates */
    float x2 = x + glyph->bitmap_left * sx;
    float y2 = -y - glyph->bitmap_top * sy;
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
    x += (glyph->advance.x >> 6) * sx;
    y += (glyph->advance.y >> 6) * sy;
  }

  glDisableVertexAttribArray(attribute_coord);
  glDeleteTextures(1, &tex);
}

void apply_draw_text()
{
  float sx = 2.0 / glutGet(GLUT_WINDOW_WIDTH);
  float sy = 2.0 / glutGet(GLUT_WINDOW_HEIGHT);

  glUseProgram(program);

  /* Enable blending, necessary for our alpha texture */
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  GLfloat black[4] = {0, 0, 0, 1};
  GLfloat red[4] = {1, 0, 0, 1};
  GLfloat transparent_green[4] = {0, 1, 0, 0.5};

  /* Set font size to 48 pixels, color to black */
  FT_Set_Pixel_Sizes(face, 0, 48);
  glUniform4fv(uniform_color, 1, black);

  /* Effects of alignment */
  draw_text("The Quick Brown Fox Jumps Over The Lazy Dog", -1 + 8 * sx, 1 - 50 * sy, sx, sy);
  draw_text("The Misaligned Fox Jumps Over The Lazy Dog", -1 + 8.5 * sx, 1 - 100.5 * sy, sx, sy);

  /* Scaling the texture versus changing the font size */
  draw_text("The Small Texture Scaled Fox Jumps Over The Lazy Dog", -1 + 8 * sx, 1 - 175 * sy, sx * 0.5, sy * 0.5);
  FT_Set_Pixel_Sizes(face, 0, 24);
  draw_text("The Small Font Sized Fox Jumps Over The Lazy Dog", -1 + 8 * sx, 1 - 200 * sy, sx, sy);
  FT_Set_Pixel_Sizes(face, 0, 48);
  draw_text("The Tiny Texture Scaled Fox Jumps Over The Lazy Dog", -1 + 8 * sx, 1 - 235 * sy, sx * 0.25, sy * 0.25);
  FT_Set_Pixel_Sizes(face, 0, 12);
  draw_text("The Tiny Font Sized Fox Jumps Over The Lazy Dog", -1 + 8 * sx, 1 - 250 * sy, sx, sy);
  FT_Set_Pixel_Sizes(face, 0, 48);

  /* Colors and transparency */
  draw_text("The Solid Black Fox Jumps Over The Lazy Dog", -1 + 8 * sx, 1 - 430 * sy, sx, sy);

  glUniform4fv(uniform_color, 1, red);
  draw_text("The Solid Red Fox Jumps Over The Lazy Dog", -1 + 8 * sx, 1 - 330 * sy, sx, sy);
  draw_text("The Solid Red Fox Jumps Over The Lazy Dog", -1 + 28 * sx, 1 - 450 * sy, sx, sy);

  glUniform4fv(uniform_color, 1, transparent_green);
  draw_text("The Transparent Green Fox Jumps Over The Lazy Dog", -1 + 8 * sx, 1 - 380 * sy, sx, sy);
  draw_text("The Transparent Green Fox Jumps Over The Lazy Dog", -1 + 18 * sx, 1 - 440 * sy, sx, sy);
}