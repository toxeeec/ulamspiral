#include "colors.h"
#include "error.h"
#include "flags.h"
#include "png.h"
#include <getopt.h>
#include <stdlib.h> // strtol

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

	const struct option long_options[] = {
	    {"force", no_argument, NULL, 'f'},
	    {"primary", required_argument, NULL, 'p'},
	    {"secondary", required_argument, NULL, 's'},
	    {0, 0, 0, 0},
	};

	while ((c = getopt_long(argc, argv, "fp:s:", long_options, NULL)) !=
	       -1) {
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

	const char *file_name = argv[optind];

	const int32_t width = strtol(argv[optind + 1], NULL, 10);

	if (errno) {
		THROW();
	}

	if (width <= 0) {
		THROW_WITH_MESSAGE("Width must be greater than 0");
	}

	create_png_file(file_name, width, (struct color[]){secondary, primary},
			flags);
}
