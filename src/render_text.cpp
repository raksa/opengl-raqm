#include <iostream>
#include <GLUT/glut.h>

#include "raqm.h"

void renderKhmerText()
{
    std::string font_file = "./assets/fonts/Battambang-Regular.ttf";
    std::string text = "hello សោះស្តី";
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

                        printf("glyph count: %zu\n", count);
                        for (i = 0; i < count; i++)
                        {
                            printf("gid#%d off: (%d, %d) adv: (%d, %d) idx: %d\n",
                                   glyphs[i].index,
                                   glyphs[i].x_offset,
                                   glyphs[i].y_offset,
                                   glyphs[i].x_advance,
                                   glyphs[i].y_advance,
                                   glyphs[i].cluster);
                        }
                    }

                    raqm_destroy(rq);
                }
            }

            FT_Done_Face(face);
        }

        FT_Done_FreeType(library);
    }
}