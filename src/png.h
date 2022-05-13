#ifndef PNG_H
#define PNG_H

#include "colors.h"

void create_png_file(const char *name, const uint32_t size,
		     const struct color *colors, const uint32_t flags);

#endif
