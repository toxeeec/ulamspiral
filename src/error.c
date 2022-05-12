#include "error.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *program_name;

void set_program_name(char *name)
{
	program_name = name;
}

void usage()
{
	printf("usage: %s file_name width \n", program_name);
	exit(EXIT_FAILURE);
}

bool is_number(char *s)
{
	size_t len = strlen(s);
	if (len == 0) {
		return false;
	}

	for (size_t i = 0; i < len; i++) {
		if (!isdigit(s[i])) {
			return false;
		}
	}
	return true;
}

bool is_positive_number(char *s)
{
	return is_number(s) && atoi(s) > 0;
}
