
#include "raqm.h"

raqm_glyph_t *getGlyphs(
    FT_Face *face,
    size_t *count,
    const char *text,
    const char *direction,
    const char *language)
{
  FT_Face font_face = *face;
  if (FT_Set_Char_Size(font_face, font_face->units_per_EM, 0, 0, 0) == 0)
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
          raqm_set_freetype_face(rq, font_face) &&
          raqm_set_par_direction(rq, dir) &&
          raqm_set_language(rq, language, 0, strlen(text)) &&
          raqm_layout(rq))
      {
        raqm_glyph_t *glyphs = raqm_get_glyphs(rq, count);
        return glyphs;
      }

      raqm_destroy(rq);
    }
  }
  return NULL;
}