
#ifndef B64_BASE64_H
#define B64_BASE64_H

#include <stdint.h>


/**
 * Returns the amount of characters that will be in the base64 encoded string
 * for a length with len `len`.
 *
 * The base64 encoding of an array that holds two values will always holds four
 * characters. Something which is also the case for an original array of three
 * characters. In both cases, this method will return 4.
 */
uint32_t base64_elen(uint32_t len);

/**
 * Returns the amount of values that will be in the array that will be returned
 * once a string with length `len` is decoded from base64.
 */
uint32_t base64_dlen(const uint8_t *input, uint32_t len);

/**
 * Encodes the given array of values into a base64 string. The method will
 * return a pointer to the array holding the encoded values.
 *
 * To determine the length of the array that is returned, one can call
 * `base64_elen` with the length of the original string.
 */
uint8_t *base64_encode(const uint8_t *input, uint32_t length);

/**
 * Decodes the given base64 string into the original array of values. The
 * method will return a pointer to the array holding the decoded values.
 *
 * To determine the length of the array that is returned, one can call
 * `base64_dlen` with the encoded string and the length of said string.
 */
uint8_t *base64decode(const uint8_t *input, uint32_t length);

#endif //B64_BASE64_H
