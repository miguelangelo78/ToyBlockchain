#include "util.h"

uint32_t swapByteOrder(uint32_t n) {
	return (n >> 24) | ((n << 8) & 0x00FF0000) | ((n >> 8) & 0x0000FF00) | (n << 24);
}
