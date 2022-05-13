#include "png.h"
#include "error.h"
#include "flags.h"
#include "primes.h"
#include <arpa/inet.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <zlib.h>

#define SIGNATURE 0x89504E470D0A1A0A
#define COLOR_DEPTH 1 // 2 colors
#define COLOR_SIZE (sizeof(uint8_t) * 3)

#define CHUNK_TYPE_SIZE (4 * sizeof(char))
#define HEADER_CHUNK_TYPE "IHDR"
#define PALETTE_CHUNK_TYPE "PLTE"
#define DATA_CHUNK_TYPE "IDAT"
#define TRAILER_CHUNK_TYPE "IEND"

struct header {
	uint32_t width;
	uint32_t height;
	uint8_t bit_depth;
	uint8_t color_type;
	uint8_t compression_method;
	uint8_t filter_method;
	uint8_t interlace_method;
};

static void write_chunk_size(FILE *f, uint32_t size)
{
	assert(f != NULL);

	size = htonl(size);
	fwrite(&size, sizeof(size), 1, f);
}

static void write_chunk_type(FILE *f, char *type)
{
	assert(f != NULL);

	fwrite(type, CHUNK_TYPE_SIZE, 1, f);
}

static void write_chunk_crc(FILE *f, void *data, size_t size)
{
	assert(f != NULL);

	uint32_t chunk_crc = htonl(crc32(0, data, size));
	fwrite(&chunk_crc, sizeof(chunk_crc), 1, f);
}

static void write_chunk(FILE *f, char *type, void *data, uint32_t data_size)
{
	assert(f != NULL);

	write_chunk_size(f, data_size);
	write_chunk_type(f, type);
	if (data == NULL) {
		write_chunk_crc(f, type, CHUNK_TYPE_SIZE);
		return;
	}
	fwrite(data, data_size, 1, f);
	uint32_t crc_input_size = CHUNK_TYPE_SIZE + data_size;

	uint8_t *crc_input = malloc(crc_input_size);
	CHECK_POINTER(crc_input);

	memcpy(crc_input, type, CHUNK_TYPE_SIZE);
	memcpy(crc_input + CHUNK_TYPE_SIZE, data, data_size);
	write_chunk_crc(f, crc_input, crc_input_size);

	free(crc_input);
}

static void write_header(FILE *f, uint32_t width)
{
	assert(f != NULL);

	width = htonl(width);
	struct header header = {
	    .width = width,
	    .height = width,
	    .bit_depth = COLOR_DEPTH,
	    .color_type = 3,	     // indexed color
	    .compression_method = 0, // deflate
	    .filter_method = 0,	     // no filter
	    .interlace_method = 0    // no interlace
	};

	uint32_t data_size = sizeof(uint32_t) * 2 + sizeof(uint8_t) * 5;
	write_chunk(f, HEADER_CHUNK_TYPE, &header, data_size);
}

static void write_palette(FILE *f, struct color *palette, size_t count)
{
	assert(f != NULL);

	uint32_t chunk_size = COLOR_SIZE * count;
	write_chunk(f, PALETTE_CHUNK_TYPE, palette, chunk_size);
}

static void write_data_palette_based(FILE *f, uint32_t width, uint8_t *data)
{
	assert(f != NULL);

	uint32_t padding = 8 - width % 8;
	if (padding == 8) {
		padding = 0;
	}
	uint32_t bytes = (width + padding) / 8;
	uint32_t full_bytes = width / 8;
	uint32_t scanline_size = 1 + bytes;
	uint8_t *input = malloc((scanline_size)*width);
	uint8_t *head = input;
	for (uint32_t i = 0; i < width; ++i) {
		uint8_t *scanline = malloc(scanline_size);
		scanline[0] = 0; // filter method

		// full bytes without padding
		for (uint32_t j = 0; j < full_bytes; ++j) {
			uint8_t byte = 0;
			for (uint32_t k = 0; k < 8; ++k) {
				byte <<= 1;
				byte |= *(data++) & 1;
			}
			scanline[1 + j] = byte;
		}

		if (padding) {
			uint8_t byte = 0;
			for (uint32_t j = 0; j < 8 - padding; ++j) {
				byte <<= 1;
				byte |= *(data++) & 1;
			}
			byte <<= padding;
			scanline[bytes] = byte;
		}
		memcpy(input, scanline, scanline_size);

		free(scanline);

		input += scanline_size;
	}

	uint32_t bound = compressBound(scanline_size * width);
	data = malloc(bound);
	compress2(data, (uLongf *)&bound, head, scanline_size * width, 0);

	free(head);

	data[0] = 8;	// zlib compression method/flags code
	data[1] = 0x1d; // Additional flags/check bits

	write_chunk(f, DATA_CHUNK_TYPE, data, bound);

	free(data);
}

static void write_trailer(FILE *f)
{
	assert(f != NULL);

	write_chunk(f, TRAILER_CHUNK_TYPE, NULL, 0);
}

void create_png_file(char *name, uint32_t width, struct color *colors,
		     uint32_t flags)
{
	if (!(flags & FORCE)) {
		// file exists
		if (!access(name, F_OK)) {
			THROW_WITH_CODE(EEXIST);
		}
	}

	FILE *f = fopen(name, "wb");
	CHECK_POINTER(f);

	uint64_t signature = htonll(SIGNATURE);
	fwrite(&signature, sizeof(signature), 1, f);

	write_header(f, width);
	write_palette(f, colors, 2);
	uint8_t *data = (uint8_t *)isprime_spiral(width);
	write_data_palette_based(f, width, data);

	free(data);

	write_trailer(f);

	fclose(f);
}
