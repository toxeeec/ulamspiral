#include "primes.h"
#include "error.h"
#include <assert.h>
#include <math.h>   // sqrt
#include <stdlib.h> // malloc, free
#include <string.h> // memset

#define SMALLEST_PRIME 2

bool *isprime_range(const size_t range)
{
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

bool *isprime_spiral(const size_t width)
{
	bool *arr = isprime_range(width * width + 1);
	CHECK_POINTER(arr);

	bool *spiral = malloc(width * width);
	CHECK_POINTER(spiral);

	const size_t dirs[] = {1, -width, -1, width};

	size_t pos = width * width / 2;
	if (width > 2 && width % 2 == 0) {
		++pos;
	}

	size_t before_turn = 1;
	size_t turn_count = 1;
	size_t dir = 0;

	for (size_t i = 1; i <= width * width; ++i) {
		spiral[pos] = arr[i];
		pos += dirs[dir];
		if (i % before_turn == 0) {
			dir = (dir + 1) % 4;
			turn_count++;
			if (turn_count % 2 == 0) {
				++before_turn;
			}
		}
	}

	free(arr);

	return spiral;
}
