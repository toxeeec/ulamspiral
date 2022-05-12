#include "../src/error.h"
#include "../src/macros.h"
#include "test.h"

test_unit(test_isnumber)
{
	struct {
		char *input;
		bool want;
	} cases[] = {
	    {"", false},
	    {"123", true},
	    {"abc", false},
	    {"123}", false},
	};
	for (size_t i = 0; i < ARRAY_SIZE(cases); ++i) {
		char *input = cases[i].input;
		bool want = cases[i].want;
		bool got = is_number(input);
		test_assert(want == got, "case[%zu]: expected: %d, got: %d", i,
			    want, got);
	}
	return 0;
}

test_suite(all)
{
	test_unit_run(test_isnumber);
	return 0;
}

test_run(all)
