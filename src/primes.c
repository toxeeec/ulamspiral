#include "primes.h"
#include "error.h"
#include <assert.h>
#include <math.h>
#include <string.h>

#define SMALLEST_PRIME 2

bool *isprime_range(size_t range)
{
	assert(range > 0);
	bool *arr = malloc(sizeof(bool) * (range));
	CHECK_POINTER(arr);
	memset(arr, 1, sizeof(bool) * range);
	arr[0] = false;
	arr[1] = false;
	for (size_t i = 2; i < sqrt(range); ++i) {
		if (!arr[i]) {
			continue;
		}
		for (size_t j = i * i; j < range; j += i) {
			arr[j] = false;
		}
	}
	return arr;
}
