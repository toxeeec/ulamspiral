#include "error.h"
#include <stdio.h>  // printf
#include <stdlib.h> // exit

const char *program_name;

void set_program_name(const char *name)
{
	program_name = name;
}

void usage()
{
	printf("usage: %s file_name width \n", program_name);
	exit(EXIT_FAILURE);
}
