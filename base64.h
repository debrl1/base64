
#ifndef B64_BASE64_H
#define B64_BASE64_H

#include <stdint.h>

/**
 * Encodes the given array of values into a base64 string.
 */
unsigned char *base64encode(const uint8_t *original, unsigned int length);

/**
 * Decodes the given base64 string into the original array of values.
 *
 * @param encoded
 * The base64 encoded string.
 *
 * @param length
 * The length of the base64 encoded string.
 *
 * @returns
 * A pointer to the array holding the decoded values will be returned.
 */
uint8_t * base64decode(const unsigned char *encoded, int length);

void test();

#endif //B64_BASE64_H
