#ifndef ERROR_H
#define ERROR_H

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

extern int errno;

extern char *program_name;

void set_program_name(char *name);
void usage();
bool is_number(char *c);
bool is_positive_number(char *c);

#define CHECK(x)                                                               \
	do {                                                                   \
		if ((x) != 0) {                                                \
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
		if ((x) != 0) {                                                \
			errno = code;                                          \
			perror(program_name);                                  \
			exit(EXIT_FAILURE);                                    \
		}                                                              \
	} while (0)

#define CHECK_WITH_MESSAGE(x, message)                                         \
	do {                                                                   \
		if ((x) != 0) {                                                \
			fprintf(stderr, "%s: %s\n", program_name, message);    \
			exit(EXIT_FAILURE);                                    \
		}                                                              \
	} while (0)

#endif
