#include "../src/colors.h"
#include "../src/macros.h"
#include "test.h"

test_unit(test_rgb_to_color)
{
	struct {
		const char *input;
		const struct color want;
	} cases[] = {
	    {"FFF", (struct color){255, 255, 255}},
	    {"ffffff", (struct color){255, 255, 255}},
	    {"000", (struct color){0, 0, 0}},
	    {"000000", (struct color){0, 0, 0}},
	    {"0f0", (struct color){0, 255, 0}},
	    {"0000FF", (struct color){0, 0, 255}},
	};
	for (size_t i = 0; i < ARRAY_SIZE(cases); ++i) {
		const char *input = cases[i].input;
		const struct color want = cases[i].want;
		const struct color got = rgb_to_color(input);
		const bool condition = (want.red == got.red) &&
				       (want.green == got.green) &&
				       (want.blue == got.blue);
		test_assert(
		    condition,
		    "case[%zu], expected: {%d, %d, %d}, got: {%d, %d, %d}", i,
		    want.red, want.green, want.blue, got.red, got.green,
		    got.blue);
	}
	return 0;
}

test_suite(all)
{
	test_unit_run(test_rgb_to_color);
	return 0;
}

test_run(all)
