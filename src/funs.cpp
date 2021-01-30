// http://madig.github.io/freetype-web/documentation/tutorial/
// https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Text_Rendering_01
#include <GLUT/glut.h>
#include <string.h>

#include "load_shader.h"

extern "C"
{
#include "raqm_utils.h"
}

FT_Library ft;
FT_Face defaultFontFace;
FT_Face enFontFace;
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
bool isKhmer = true;
const char *kmFontfilename = "/Users/raksa/Desktop/dev/opengl-raqm/assets/fonts/Battambang-Regular.ttf";
const char *emFontfilename = "/Users/raksa/Desktop/dev/opengl-raqm/assets/fonts/POPPINS-BOLD.TTF";
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
  const char *fontfilename = isKhmer ? kmFontfilename : emFontfilename;
  if (FT_New_Face(ft, fontfilename, 0, &defaultFontFace))
  {
    fprintf(stderr, "Could not open font %s\n", fontfilename);
    return 0;
  }

  FT_Set_Pixel_Sizes(defaultFontFace, 0, 48);

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

void draw_text(FT_Face *fontFace, const char *text, float x, float y, float sx, float sy)
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
  size_t count, i;
  FT_GlyphSlot glyph = (*fontFace)->glyph;
  raqm_glyph_t *raqmGlyphs = getGlyphs(fontFace, &count, text, "l", "km");
  const char *p;
  for (i = 0; i < count; i++)
  {
    if (FT_Load_Glyph(*fontFace, raqmGlyphs[i].index, FT_LOAD_DEFAULT))
    {
      fprintf(stderr, "Could not load glyph\n");
      return;
    }
    if (FT_Render_Glyph(glyph, FT_RENDER_MODE_NORMAL))
    {
      fprintf(stderr, "Could not render glyph\n");
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
  FT_Set_Pixel_Sizes(defaultFontFace, 0, 48);
  glUniform4fv(uniform_color, 1, black);

  /* Effects of alignment */
  draw_text(&defaultFontFace, isKhmer ? "កញ្ជ្រោងពណ៌ត្នោតរហ័សលោតលើឆ្កែខ្ជិល" : "The Quick Brown Fox Jumps Over The Lazy Dog",
            -1 + 8 * sx, 1 - 50 * sy, sx, sy);
  draw_text(&defaultFontFace, isKhmer ? "កញ្ជ្រោងឆ្កែច្របូកច្របល់លោតលើឆ្កែខ្ជិល" : "The Misaligned Fox Jumps Over The Lazy Dog",
            -1 + 8.5 * sx, 1 - 100.5 * sy, sx, sy);

  /* Scaling the texture versus changing the font size */
  draw_text(&defaultFontFace, isKhmer ? "កញ្ជ្រោងវាយនភាពខ្នាតតូចលោតលើឆ្កែខ្ជិល" : "The Small Texture Scaled Fox Jumps Over The Lazy Dog",
            -1 + 8 * sx, 1 - 175 * sy, sx * 0.5, sy * 0.5);
  FT_Set_Pixel_Sizes(defaultFontFace, 0, 24);
  draw_text(&defaultFontFace, isKhmer ? "ហ្វកតូចខ្នាតតូចលោតលើឆ្កែខ្ជិល" : "The Small Font Sized Fox Jumps Over The Lazy Dog",
            -1 + 8 * sx, 1 - 200 * sy, sx, sy);
  FT_Set_Pixel_Sizes(defaultFontFace, 0, 48);
  draw_text(&defaultFontFace, isKhmer ? "វាយនភាពខ្នាតតូចធ្វើឱ្យកញ្ជ្រោងលោតលើឆ្កែខ្ជិល" : "The Tiny Texture Scaled Fox Jumps Over The Lazy Dog",
            -1 + 8 * sx, 1 - 235 * sy, sx * 0.25, sy * 0.25);
  FT_Set_Pixel_Sizes(defaultFontFace, 0, 12);
  draw_text(&defaultFontFace, isKhmer ? "ខ្នាតតូចខ្នាតតូចអក្សរហ្វកលោតលើសត្វឆ្កែខ្ជិល" : "The Tiny Font Sized Fox Jumps Over The Lazy Dog",
            -1 + 8 * sx, 1 - 250 * sy, sx, sy);
  FT_Set_Pixel_Sizes(defaultFontFace, 0, 48);

  /* Colors and transparency */
  draw_text(&defaultFontFace, isKhmer ? "កញ្ជ្រោងខ្មៅរឹងលោតលើឆ្កែខ្ជិល" : "The Solid Black Fox Jumps Over The Lazy Dog",
            -1 + 8 * sx, 1 - 430 * sy, sx, sy);

  glUniform4fv(uniform_color, 1, red);
  draw_text(&defaultFontFace, isKhmer ? "កញ្ជ្រោងក្រហមរឹងលោតលើឆ្កែខ្ជិល" : "The Solid Red Fox Jumps Over The Lazy Dog",
            -1 + 8 * sx, 1 - 330 * sy, sx, sy);
  draw_text(&defaultFontFace, isKhmer ? "កញ្ជ្រោងក្រហមរឹងលោតលើឆ្កែខ្ជិល" : "The Solid Red Fox Jumps Over The Lazy Dog",
            -1 + 28 * sx, 1 - 450 * sy, sx, sy);

  glUniform4fv(uniform_color, 1, transparent_green);
  draw_text(&defaultFontFace, isKhmer ? "សត្វកញ្ជ្រោងបៃតងថ្លាលោតលើឆ្កែខ្ជិល" : "The Transparent Green Fox Jumps Over The Lazy Dog",
            -1 + 8 * sx, 1 - 380 * sy, sx, sy);
  draw_text(&defaultFontFace, isKhmer ? "សត្វកញ្ជ្រោងបៃតងថ្លាលោតលើឆ្កែខ្ជិល តន្ត្រី" : "The Transparent Green Fox Jumps Over The Lazy Dog",
            -1 + 18 * sx, 1 - 440 * sy, sx, sy);

  if (FT_New_Face(ft, emFontfilename, 0, &enFontFace))
  {
    fprintf(stderr, "Could not open font %s\n", emFontfilename);
    return;
  }
  FT_Set_Pixel_Sizes(enFontFace, 0, 48);
  glUniform4fv(uniform_color, 1, red);
  draw_text(&enFontFace, "The Quick Brown Fox Jumps Over The Lazy Dog",
            -1 + 8 * sx, 1 - 50 * sy, sx, sy);
}