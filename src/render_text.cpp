#include <iostream>
#include <unistd.h>
#include <GLUT/glut.h>

#include "raqm.h"

extern "C"
{
#include "raqm_utils.h"
}

void renderKhmerText()
{
    const char *text = "Khmer សោះស្តី";
    glRasterPos2f(10, 0);
    int len = strlen(text);
    int i;
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
    }
}

GLint rt_attribute_coord;
GLint rt_uniform_tex;
GLuint rt_vbo;
struct point
{
    GLfloat x;
    GLfloat y;
    GLfloat s;
    GLfloat t;
};
void render_text(const char *text, FT_Face *face,
                 float x, float y, float sx, float sy)
{
    /* Create a texture that will be used to hold one "glyph" */
    GLuint tex;
    size_t count, i;
    raqm_glyph_t raqmGlyph;
    FT_Bitmap bitmap;
    FT_GlyphSlot glyp;
    raqm_glyph_t *raqmGlyphs = getGlyphs(face, &count, text, "l", "km");

    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glUniform1i(rt_uniform_tex, 0);

    /* We require 1 byte alignment when uploading texture data */
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    /* Clamping to edges is important to prevent artifacts when scaling */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    /* Linear filtering usually looks best for text */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    /* Set up the rt_VBO for our vertex data */
    glEnableVertexAttribArray(rt_attribute_coord);
    glBindBuffer(GL_ARRAY_BUFFER, rt_vbo);
    glVertexAttribPointer(rt_attribute_coord, 4, GL_FLOAT, GL_FALSE, 0, 0);

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

    glDisableVertexAttribArray(rt_attribute_coord);
    glDeleteTextures(1, &tex);
}