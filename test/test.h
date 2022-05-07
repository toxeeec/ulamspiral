#ifndef TEST_H
#define TEST_H

#include <stdio.h>
#include <stdlib.h>

#define test_unit(name) static int name(void)
#define test_suite(name) static int name(void)

#define test_assert(cond, msg, ...)                                            \
	do {                                                                   \
		if (!(cond)) {                                                 \
			char *msgf;                                            \
			if (0 > asprintf(&msgf, msg, __VA_ARGS__)) {           \
				exit(EXIT_FAILURE);                            \
			}                                                      \
			printf("FAIL: %s\n", __func__);                        \
			printf("%s:%d: %s\n", __FILE__, __LINE__, msgf);       \
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
	int main(void)                                                         \
	{                                                                      \
		int result = name();                                           \
		if (!result) {                                                 \
			printf("PASS: %s\n", __FILE__);                        \
		}                                                              \
		return result != 0;                                            \
	}

#endif
