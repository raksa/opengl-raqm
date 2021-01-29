#include <iostream>
#include <unistd.h>
#include <GLUT/glut.h>

#include "raqm.h"

extern "C"
{
#include "utils.h"
}

void renderKhmerText()
{
    char buf[PATH_MAX];
    std::string font_file = std::string(getcwd(buf, sizeof(buf))) + "/assets/fonts/Battambang-Regular.ttf";
    std::string text = "Khmer text សោះស្តី";
    std::string direction = "r";
    std::string language = "km";

    FT_Library library = NULL;
    FT_Face face = NULL;

    if (FT_Init_FreeType(&library) == 0)
    {
        if (FT_New_Face(library, font_file.c_str(), 0, &face) == 0)
        {
            if (FT_Set_Char_Size(face, face->units_per_EM, 0, 0, 0) == 0)
            {
                raqm_t *rq = raqm_create();
                if (rq != NULL)
                {
                    raqm_direction_t dir = RAQM_DIRECTION_DEFAULT;

                    if (direction == "r")
                        dir = RAQM_DIRECTION_RTL;
                    else if (direction == "l")
                        dir = RAQM_DIRECTION_LTR;

                    if (raqm_set_text_utf8(rq, text.c_str(), text.length()) &&
                        raqm_set_freetype_face(rq, face) &&
                        raqm_set_par_direction(rq, dir) &&
                        raqm_set_language(rq, language.c_str(), 0, text.length()) &&
                        raqm_layout(rq))
                    {
                        size_t count, i;
                        raqm_glyph_t *glyphs = raqm_get_glyphs(rq, &count);

                        glRasterPos2f(10, 0);
                        int len = text.length();
                        for (i = 0; i < len; i++)
                        {
                            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
                        }

                        // printf("glyph count: %zu\n", count);
                        // for (i = 0; i < count; i++)
                        // {
                        //     printf("gid#%d off: (%d, %d) adv: (%d, %d) idx: %d\n",
                        //            glyphs[i].index,
                        //            glyphs[i].x_offset,
                        //            glyphs[i].y_offset,
                        //            glyphs[i].x_advance,
                        //            glyphs[i].y_advance,
                        //            glyphs[i].cluster);
                        // }
                    }

                    raqm_destroy(rq);
                }
            }

            FT_Done_Face(face);
        }

        FT_Done_FreeType(library);
    }
}

GLint attribute_coord;
GLint uniform_tex;
FT_Face face;
FT_Library ft;
GLuint vbo;
struct point
{
    GLfloat x;
    GLfloat y;
    GLfloat s;
    GLfloat t;
};
void render_text(const char *text, const char *fontfilename,
                 float x, float y, float sx, float sy)
{
    /* Initialize the FreeType2 library */
    if (FT_Init_FreeType(&ft))
    {
        fprintf(stderr, "Could not init freetype library\n");
        return;
    }
    glGenBuffers(1, &vbo);
    if (FT_New_Face(ft, fontfilename, 0, &face))
    {
        fprintf(stderr, "Could not open font %s\n", fontfilename);
        return;
    }

    /* Create a texture that will be used to hold one "glyph" */
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

    size_t count, i;
    raqm_glyph_t raqmGlyph;
    raqm_glyph_t *raqmGlyphs = getGlyphs(&face, &count, text, "l", "km");
    FT_Bitmap bitmap;
    FT_GlyphSlot glyp;

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