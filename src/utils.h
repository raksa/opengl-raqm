#ifndef UTILS
#define UTILS

#include "raqm.h"

raqm_glyph_t *getGlyphs(FT_Face *face, size_t *count, const char *text, const char *direction, const char *language);

#endif