#ifndef TEST_H
#define TEST_H

#include "../src/error.h"
#include <stdio.h>
#include <stdlib.h>

#define test_unit(name) static int name(void)
#define test_suite(name) static int name(void)

#define test_assert(cond, msg, ...)                                            \
	do {                                                                   \
		if (!(cond)) {                                                 \
			char *msgf;                                            \
			CHECK(asprintf(&msgf, msg, __VA_ARGS__));              \
			printf("FAIL: %s\n", __func__);                        \
			printf("\t%s:%d: %s\n", __FILE__, __LINE__, msgf);     \
			free(msgf);                                            \
			return 1;                                              \
		}                                                              \
	} while (0)

#define test_unit_run(name)                                                    \
	do {                                                                   \
		int result = name();                                           \
		if (result) {                                                  \
			return result;                                         \
		}                                                              \
	} while (0)

#define test_run(name)                                                         \
	int main(int argc, char **argv)                                        \
	{                                                                      \
		(void)argc;                                                    \
		set_program_name(argv[0]);                                     \
		int result = name();                                           \
		if (!result) {                                                 \
			printf("PASS: %s\n", __FILE__);                        \
		}                                                              \
		return result != 0;                                            \
	}

#endif
