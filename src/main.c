#include "colors.h"
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

	// primary white, secondary black
	struct color primary = {255, 255, 255};
	struct color secondary = {0, 0, 0};

	int c;

	while ((c = getopt(argc, argv, "fp:s:")) != -1) {
		switch (c) {
		case 'f': flags |= FORCE; break;
		case 'p': {
			primary = rgb_to_color(optarg);
			break;
		}
		case 's': {
			secondary = rgb_to_color(optarg);
			break;
		}
		}
	}

	if (argv[optind] == NULL || argv[optind + 1] == NULL) {
		usage();
	}

	char *file_name = argv[optind];

	int32_t width = strtol(argv[optind + 1], NULL, 10);

	if (errno) {
		THROW();
	}

	if (width <= 0) {
		THROW_WITH_MESSAGE("Width must be greater than 0");
	}

	create_png_file(file_name, width, (struct color[]){secondary, primary},
			flags);

	return 0;
}
