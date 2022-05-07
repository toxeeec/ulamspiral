#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <stdlib.h>

#define PROGNAME "ulamspiral"

#define PANIC()                                                                \
	do {                                                                   \
		perror(PROGNAME);                                              \
		exit(EXIT_FAILURE);                                            \
	} while (0)

#endif
