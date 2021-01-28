
#include "raqm.h"

const char *getGlyphs(
    const char *fontfile,
    const char *text,
    const char *direction,
    const char *language)
{
  char *result;

  FT_Library library = NULL;
  FT_Face face = NULL;

  if (FT_Init_FreeType(&library) == 0)
  {
    if (FT_New_Face(library, fontfile, 0, &face) == 0)
    {
      if (FT_Set_Char_Size(face, face->units_per_EM, 0, 0, 0) == 0)
      {
        raqm_t *rq = raqm_create();
        if (rq != NULL)
        {
          raqm_direction_t dir = RAQM_DIRECTION_DEFAULT;

          if (strcmp(direction, "r") == 0)
            dir = RAQM_DIRECTION_RTL;
          else if (strcmp(direction, "l") == 0)
            dir = RAQM_DIRECTION_LTR;

          if (raqm_set_text_utf8(rq, text, strlen(text)) &&
              raqm_set_freetype_face(rq, face) &&
              raqm_set_par_direction(rq, dir) &&
              raqm_set_language(rq, language, 0, strlen(text)) &&
              raqm_layout(rq))
          {
            size_t count, i;
            raqm_glyph_t *glyphs = raqm_get_glyphs(rq, &count);

            for (i = 0; i < count; i++)
            {
              sprintf(result, "gid#%d off: (%d, %d) adv: (%d, %d) idx: %d\n",
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

  return result;
}