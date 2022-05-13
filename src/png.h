#ifndef PNG_H
#define PNG_H

#include "colors.h"
#include <stdlib.h>

void create_png_file(char *name, uint32_t size, struct color *colors,
		     uint32_t flags);

#endif
