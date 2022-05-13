#include "colors.h"
#include "error.h"
#include <stdlib.h>
#include <string.h>

#define SHORT_HEX_LENGTH 3 // FFF
#define LONG_HEX_LENGTH 6  // FFFFFF

static uint8_t hextou8(char *hex_string)
{
	char s[3];
	memcpy(s, hex_string, 2);
	s[2] = 0;
	uint8_t val = strtol(s, NULL, 16);
	if (errno) {
		THROW_WITH_MESSAGE("Color must be a valid hex number");
	}
	return val;
}

struct color rgb_to_color(char *hex_string)
{
	size_t len = strlen(hex_string);
	if ((len != SHORT_HEX_LENGTH) && (len != LONG_HEX_LENGTH)) {
		THROW_WITH_MESSAGE(
		    "Color must be either in format FFF or FFFFFF");
	}

	if (len == SHORT_HEX_LENGTH) {
		char new[LONG_HEX_LENGTH] = {
		    hex_string[0], hex_string[0], hex_string[1],
		    hex_string[1], hex_string[2], hex_string[2],
		};
		hex_string = new;
	}

	struct color c;
	c.red = hextou8(&hex_string[0]);
	c.green = hextou8(&hex_string[2]);
	c.blue = hextou8(&hex_string[4]);
	return c;
}
