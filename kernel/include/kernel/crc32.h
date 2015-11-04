#ifndef CRC32_H
#define CRC32_H

#include <sys/cdefs.h>
#include <stdint.h>

uint32_t crc32_byte(uint8_t *p, uint32_t bytelength);

#endif
