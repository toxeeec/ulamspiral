#include "../src/macros.h"
#include "../src/primes.h"
#include "test.h"

test_unit(test_isprime_range)
{
	struct {
		const size_t input;
		const size_t want;
	} cases[] = {
	    {1, 0}, {2, 0}, {3, 1}, {1001, 168}, {1000001, 78498},
	};
	for (size_t i = 0; i < ARRAY_SIZE(cases); ++i) {
		const size_t input = cases[i].input;
		const size_t want = cases[i].want;

		bool *arr = isprime_range(input);
		CHECK_POINTER(arr);

		size_t got = 0;
		for (size_t j = 0; j < input; j++) {
			if (arr[j]) {
				++got;
			}
		}

		free(arr);

		test_assert(want == got, "case[%zu], expected: %zu, got: %zu",
			    i, want, got);
	}
	return 0;
}

test_unit(test_isprime_spiral)
{
	struct {
		const size_t input;
		const bool *want;
	} cases[] = {
	    {2, (bool[]){0, 1, 0, 1}},
	    {3, (bool[]){1, 0, 1, 0, 0, 1, 1, 0, 0}},
	    {4, (bool[]){0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0}}};

	for (size_t i = 0; i < ARRAY_SIZE(cases); ++i) {
		size_t got = 0;
		const size_t input = cases[i].input;
		const bool *want = cases[i].want;

		bool *arr = isprime_spiral(input);
		CHECK_POINTER(arr);

		for (size_t j = 0; j < input * input; ++j) {
			if (want[j] == arr[j]) {
				++got;
			}
		}

		free(arr);

		test_assert(input * input == got,
			    "case[%zu]: expected: %zu, got: %zu", i,
			    input * input, got);
	}
	return 0;
}

test_suite(all)
{
	test_unit_run(test_isprime_range);
	test_unit_run(test_isprime_spiral);
	return 0;
}

test_run(all)
