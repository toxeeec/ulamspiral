#include "error.h"
#include <stdio.h>
#include <stdlib.h>

const char *program_name;

void set_program_name(const char *name)
{
	program_name = name;
}

void usage()
{
	printf("usage: %s [-f | --force] [-p color | --primary=color] "
	       "[-s color | --secondary=color] file_name width\n",
	       program_name);
	exit(EXIT_FAILURE);
}
