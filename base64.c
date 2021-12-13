
#include <stdlib.h>

#include "base64.h"

#define PADDING 0x40

/*
 * Derives the length of the encoded base64 string from the length of the
 * original string.
 */
uint32_t encodedlen(uint32_t len) {
    uint32_t newlen = (len / 3) * 4;
    if (len % 3 != 0)
        newlen += 4;
    return newlen;
}

/*
 * Encodes an unencoded group (containing exactly three characters) into
 * base64. The resulting group contains exactly four characters.
 */
void encode_group(const uint8_t *input, uint8_t *output) {
    output[0] = input[0] >> 0x02;
    output[1] = ((input[0] & 0x03) << 0x04) | ((input[1] & 0xf0) >> 0x04);
    output[2] = ((input[1] << 0x02) & 0x3f) | (input[2] >> 0x06);
    output[3] = input[2] & 0x3f;
}

/*
 * Decodes a base64 encoded group (containing exactly four characters) into
 * the original bytes. The resulting group contains exactly four characters.
 */
void decode_group(const uint8_t *input, uint8_t *output) {
    output[0] = input[0] >> 0x02;
    output[1] = ((input[0] & 0x03) << 0x04) | ((input[1] & 0xf0) >> 0x04);
    output[2] = ((input[1] << 0x02) & 0x3f) | (input[2] >> 0x06);
    output[3] = input[2] & 0x3f;
}

/*
 * Encodes an unencoded group containing one or two characters into base64.
 * The resulting group will still only contain four characters. For all of the
 * characters that can not be derived from the original bytes a padding
 * character will be inserted.
 */
void encode_remainder(const uint8_t *input, uint32_t inputlen, uint8_t *output) {
    output[0] = input[0] >> 0x02;
    output[1] = (input[0] & 0x03) << 0x04;
    if (inputlen == 1) {
        output[2] = PADDING;
        output[3] = PADDING;
    } else {
        output[1] |= (input[1] & 0xf0) >> 0x04;
        output[2] = (input[1] << 0x02) & 0x3f;
        output[3] = PADDING;
    }
}

void to_digit(uint8_t *byte) {
    if (*byte < 26)
        *byte += 'A';
    else if (*byte < 52)
        *byte = 'a' + (*byte - 26);
    else if (*byte < 62)
        *byte = '0' + (*byte - 52);
    else if (*byte == 62)
        *byte = '+';
    else if (*byte == 63)
        *byte = '/';
    else
        *byte = '=';
}

uint8_t *base64encode(const uint8_t *input, uint32_t length) {
    
    /*
     * Calculating the length of the array that will be returned and 
     * allocating memory for it; 
     */
    uint32_t newlen = encodedlen(length);
    uint32_t k = 0;
    uint8_t *base64 = (uint8_t *) malloc(sizeof(uint8_t) * (newlen + 1));

    /*
     * Allocating memory for the arrays that will hold the individual groups;
     */
    uint8_t *original = (uint8_t *) malloc(sizeof(uint8_t) * 3);
    uint8_t *encoded = (uint8_t *) malloc(sizeof(uint8_t) * 4);

    for (int i = 0; i < length; i += 3) {

        /*
         * Determining the amount of values left to encode;
         */
        uint32_t lim = 3;
        if (i + 3 > length)
            lim = length - i;

        /*
         * Copying the input values;
         */
        for (int j = 0; j < lim; ++j)
            original[j] = input[i + j];

        /*
         * Encoding the copied values and writing these values to the
         * resulting array;
         */
        if (lim == 3)
            encode_group(original, encoded);
        else
            encode_remainder(original, lim, encoded);
        for (int j = 0; j < 4; ++j)
            base64[k++] = encoded[j];
    }

    /*
     * Freeing allocated memory;
     */
    free(original);
    free(encoded);

    /*
     * Delimiting the base64 string;
     */
    base64[newlen] = 0x00;
    for (int i = 0; i < newlen; ++i)
        to_digit(&base64[i]);
    return base64;
}

uint8_t *base64decode(const uint8_t *encoded, int length) {

}
