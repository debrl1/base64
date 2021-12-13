
#ifndef B64_BASE64_H
#define B64_BASE64_H

#include <stdint.h>


/**
 * Returns the amount of characters that will be in the base64 encoded string
 * for a length with len `len`.
 */
uint32_t encodedlen(uint32_t len);

/**
 * Returns the amount of values that will be in the array that will be returned
 * once a string with length `len` is decoded from base64.
 */
uint32_t decodedlen(uint32_t len);

/**
 * Encodes the given array of values into a base64 string.
 */
uint8_t *base64encode(const uint8_t *input, unsigned int length);

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
uint8_t *base64decode(const uint8_t *encoded, int length);

#endif //B64_BASE64_H
