#ifndef ERROR_H
#define ERROR_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#define PROGNAME "ulamspiral"

extern int errno;

#define CHECK(x)                                                               \
	do {                                                                   \
		if ((x) != 0) {                                                \
			perror(PROGNAME);                                      \
			exit(EXIT_FAILURE);                                    \
		}                                                              \
	} while (0)

#define CHECK_POINTER(x)                                                       \
	do {                                                                   \
		if ((x) == NULL) {                                             \
			perror(PROGNAME);                                      \
			exit(EXIT_FAILURE);                                    \
		}                                                              \
	} while (0)

#define CHECK_WITH_CODE(x, code)                                               \
	do {                                                                   \
		if ((x) != 0) {                                                \
			errno = code;                                          \
			perror(PROGNAME);                                      \
			exit(EXIT_FAILURE);                                    \
		}                                                              \
	} while (0)

#endif
