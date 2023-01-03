#ifndef ERROR_H
#define ERROR_H

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

extern int errno;

extern const char *program_name;

void set_program_name(const char *name);
void usage();

#define CHECK(x)                                                               \
	do {                                                                   \
		if ((x) == -1) {                                               \
			perror(program_name);                                  \
			exit(EXIT_FAILURE);                                    \
		}                                                              \
	} while (0)

#define CHECK_POINTER(x)                                                       \
	do {                                                                   \
		if ((x) == NULL) {                                             \
			perror(program_name);                                  \
			exit(EXIT_FAILURE);                                    \
		}                                                              \
	} while (0)

#define CHECK_WITH_CODE(x, code)                                               \
	do {                                                                   \
		if ((x) == -1) {                                               \
			errno = code;                                          \
			perror(program_name);                                  \
			exit(EXIT_FAILURE);                                    \
		}                                                              \
	} while (0)

#define THROW()                                                                \
	do {                                                                   \
		perror(program_name);                                          \
		exit(EXIT_FAILURE);                                            \
	} while (0)

#define THROW_WITH_CODE(code)                                                  \
	do {                                                                   \
		errno = code;                                                  \
		perror(program_name);                                          \
		exit(EXIT_FAILURE);                                            \
	} while (0)

#define THROW_WITH_MESSAGE(message)                                            \
	do {                                                                   \
		fprintf(stderr, "%s: %s\n", program_name, message);            \
		exit(EXIT_FAILURE);                                            \
	} while (0)

#endif
