#include "../src/crc.h"
#include "../src/macros.h"
#include "test.h"
#include <inttypes.h>

test_unit(test_crc)
{
	struct {
		uint8_t *input;
		size_t size;
		uint32_t want;
	} cases[] = {
	    {(uint8_t *)"123456789", 9, 0xcbf43926},
	};
	for (size_t i = 0; i < ARRAY_SIZE(cases); ++i) {
		uint8_t *input = cases[i].input;
		size_t size = cases[i].size;
		uint32_t want = cases[i].want;
		uint32_t got = crc(input, size);
		test_assert(want == got, "%d", 2);
	}
	return 0;
}

test_suite(all)
{
	test_unit_run(test_crc);
	return 0;
}

test_run(all)
