#include "crc.h"
#include <limits.h>
#include <stdbool.h>

// https://www.w3.org/TR/2003/REC-PNG-20031110/#D-CRCAppendix

#define TABLE_SIZE 256
#define CRC_32_POLYNOMIAL_LSB 0xEDB88320

static uint32_t crc_table[TABLE_SIZE];
static bool crc_table_computed = 0;

static void make_crc_table(void)
{
	for (uint32_t i = 0; i < 256; i++) {
		uint32_t c = i;
		for (int k = 0; k < 8; ++k) {
			if (c & 1) {
				c = CRC_32_POLYNOMIAL_LSB ^ (c >> 1);
			} else {
				c >>= 1;
			}
		}
		crc_table[i] = c;
	}
	crc_table_computed = 1;
}

static uint32_t update_crc(uint32_t crc, uint8_t *buf, size_t len)
{
	uint32_t c = crc;
	if (!crc_table_computed) make_crc_table();
	for (size_t i = 0; i < len; ++i) {
		c = crc_table[(c ^ buf[i]) & UINT8_MAX] ^ (c >> 8);
	}
	return c;
}

uint32_t crc(uint8_t *buf, size_t len)
{
	return update_crc(UINT32_MAX, buf, len) ^ UINT32_MAX;
}
