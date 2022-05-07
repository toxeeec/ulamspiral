#ifndef ERROR_H
#define ERROR_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#define PROGNAME "ulamspiral"

#define CHECK(x)                                                               \
	do {                                                                   \
		if ((x) != 0) {                                                \
			perror(PROGNAME);                                      \
			exit(EXIT_FAILURE);                                    \
		}                                                              \
	} while (0)

#endif
