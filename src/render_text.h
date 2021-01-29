#ifndef RENDER_TEXT
#define RENDER_TEXT

#include "raqm.h"

void renderKhmerText();
void render_text(const char *text, FT_Face *face,
                 float x, float y, float sx, float sy);

#endif