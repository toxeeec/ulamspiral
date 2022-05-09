#include "../src/macros.h"
#include "../src/primes.h"
#include "test.h"

test_unit(test_isprime_range)
{
	struct {
		int input;
		int want;
	} cases[] = {
	    {1, 0}, {2, 0}, {3, 1}, {1001, 168}, {1000001, 78498},
	};
	for (size_t i = 0; i < ARRAY_SIZE(cases); ++i) {
		int input = cases[i].input;
		int want = cases[i].want;
		bool *arr = isprime_range(input);
		int got = 0;
		for (int j = 0; j < input; j++) {
			if (arr[j]) {
				++got;
			}
		}

		free(arr);

		test_assert(want == got, "expected: %d, got: %d", want, got);
	}
	return 0;
}

test_suite(all)
{
	test_unit_run(test_isprime_range);
	return 0;
}

test_run(all)
