#include "png.h"
#include <stdio.h>

int main()
{
	remove("test.png");
	create_png_file("test.png", 10);
	return 0;
}
