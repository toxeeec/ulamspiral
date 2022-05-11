#include "png.h"
#include "primes.h"
#include <stdio.h>

int main()
{
	remove("test.png");
	create_png_file("test.png", 200);
	return 0;
}
