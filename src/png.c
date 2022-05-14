#include "colors.h"
#include "error.h"
#include "flags.h"
#include "primes.h"
#include <arpa/inet.h> // htonl
#include <assert.h>
#include <stdio.h>  // fwrite, fopen, fclose
#include <stdlib.h> // malloc, free
#include <string.h> // memcpy
#include <unistd.h> // access
#include <zlib.h>   // crc32, compressBound, compress2

#define SIGNATURE ((char[]){137, 80, 78, 71, 13, 10, 26, 10})
#define COLOR_DEPTH 1 // 2 colors
#define COLOR_SIZE (sizeof(uint8_t) * 3)

#define CHUNK_TYPE_SIZE (sizeof(char) * 4)
#define HEADER_CHUNK_TYPE "IHDR"
#define PALETTE_CHUNK_TYPE "PLTE"
#define DATA_CHUNK_TYPE "IDAT"
#define TRAILER_CHUNK_TYPE "IEND"

struct header {
	const uint32_t width;
	const uint32_t height;
	const uint8_t bit_depth;
	const uint8_t color_type;
	const uint8_t compression_method;
	const uint8_t filter_method;
	const uint8_t interlace_method;
};

static void write_chunk_size(FILE *f, uint32_t size)
{
	assert(f != NULL);

	size = htonl(size);
	fwrite(&size, sizeof(size), 1, f);
}

static void write_chunk_type(FILE *f, const char *type)
{
	assert(f != NULL);

	fwrite(type, CHUNK_TYPE_SIZE, 1, f);
}

static void write_chunk_crc(FILE *f, const void *data, const size_t size)
{
	assert(f != NULL);

	const uint32_t chunk_crc = htonl(crc32(0, data, size));
	fwrite(&chunk_crc, sizeof(chunk_crc), 1, f);
}

static void write_chunk(FILE *f, const char *type, const void *data,
			const uint32_t data_size)
{
	assert(f != NULL);

	write_chunk_size(f, data_size);
	write_chunk_type(f, type);
	if (data == NULL) {
		write_chunk_crc(f, type, CHUNK_TYPE_SIZE);
		return;
	}
	fwrite(data, data_size, 1, f);
	const uint32_t crc_input_size = CHUNK_TYPE_SIZE + data_size;

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
	const struct header header = {
	    .width = width,
	    .height = width,
	    .bit_depth = COLOR_DEPTH,
	    .color_type = 3,	     // indexed color
	    .compression_method = 0, // deflate
	    .filter_method = 0,	     // no filter
	    .interlace_method = 0    // no interlace
	};

	const uint32_t data_size = sizeof(uint32_t) * 2 + sizeof(uint8_t) * 5;
	write_chunk(f, HEADER_CHUNK_TYPE, &header, data_size);
}

static void write_palette(FILE *f, const struct color *palette,
			  const size_t count)
{
	assert(f != NULL);

	const uint32_t chunk_size = COLOR_SIZE * count;
	write_chunk(f, PALETTE_CHUNK_TYPE, palette, chunk_size);
}

static void write_data_palette_based(FILE *f, const uint32_t width,
				     uint8_t *data)
{
	assert(f != NULL);

	uint32_t padding = 8 - width % 8;
	if (padding == 8) {
		padding = 0;
	}
	const uint32_t bytes = (width + padding) / 8;
	const uint32_t full_bytes = width / 8;
	const uint32_t scanline_size = 1 + bytes;
	uint8_t *input = malloc((scanline_size)*width);
	uint8_t *head = input;
	for (uint32_t i = 0; i < width; ++i) {
		uint8_t *scanline = malloc(scanline_size);
		CHECK_POINTER(scanline);

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
	CHECK_POINTER(data);

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

void create_png_file(const char *name, const uint32_t width,
		     const struct color *colors, const uint32_t flags)
{
	if (!(flags & FORCE)) {
		// file exists
		if (!access(name, F_OK)) {
			THROW_WITH_CODE(EEXIST);
		}
	}

	FILE *f = fopen(name, "wb");
	CHECK_POINTER(f);

	fwrite(SIGNATURE, sizeof(SIGNATURE), 1, f);

	write_header(f, width);
	write_palette(f, colors, 2);

	uint8_t *data = (uint8_t *)isprime_spiral(width);
	CHECK_POINTER(data);

	write_data_palette_based(f, width, data);

	free(data);

	write_trailer(f);

	fclose(f);
}
