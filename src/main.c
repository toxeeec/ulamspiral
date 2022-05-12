#include "error.h"
#include "flags.h"
#include "png.h"
#include "primes.h"
#include <unistd.h>

int main(int argc, char **argv)
{
	set_program_name(argv[0]);

	if (argc < 3) {
		usage();
	}

	int c;

	while ((c = getopt(argc, argv, "f")) != -1) {
		switch (c) {
		case 'f': flags |= FORCE; break;
		}
	}

	if (argv[optind] == NULL || argv[optind + 1] == NULL) {
		usage();
	}

	char *file_name = argv[optind];

	CHECK_WITH_MESSAGE(!is_number(argv[optind + 1]),
			   "Width must be a number.");
	CHECK_WITH_MESSAGE(!is_positive_number(argv[optind + 1]),
			   "Width must be greater than 0.");

	uint32_t width = atoi(argv[optind + 1]);

	create_png_file(file_name, width, flags);

	return 0;
}
