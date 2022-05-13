#ifndef COLORS_H
#define COLORS_H

#include <stdint.h>

struct color {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
};

struct color rgb_to_color(char *hex_string);

#endif
